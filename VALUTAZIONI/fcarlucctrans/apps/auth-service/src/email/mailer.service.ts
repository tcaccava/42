import { Injectable, Logger, OnModuleInit } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { google } from 'googleapis';
import * as nodemailer from 'nodemailer';
import type { Transporter } from 'nodemailer';

export type MailMessage = {
	to: string;
	subject: string;
	text: string;
	html: string;
};

type MailMode = 'gmail-api' | 'smtp' | 'fallback';

type GmailApiConfig = {
	user: string;
	clientId: string;
	clientSecret: string;
	refreshToken: string;
};

const SMTP_TIMEOUT_MS = 10_000;

/**
 * Mailer abstraction for GDPR notifications (delete / anonymize requests + confirmations).
 *
 * Priority:
 * 1. Gmail API (HTTPS) — MAIL_USERNAME + MAIL_CLIENT_ID + MAIL_SECRET + MAIL_REFRESH_TOKEN
 *    Uses port 443, works when SMTP ports 587/465 are blocked on the network.
 * 2. Generic SMTP — SMTP_HOST (+ SMTP_PORT, SMTP_USER, SMTP_PASS, SMTP_SECURE, MAIL_FROM)
 * 3. jsonTransport fallback — logs rendered emails (local dev without mail credentials)
 */
@Injectable()
export class MailerService implements OnModuleInit {
	private readonly logger = new Logger(MailerService.name);
	private transporter: Transporter | null = null;
	private gmailApi: GmailApiConfig | null = null;
	private from = 'no-reply@ft-transcendence.local';
	private mode: MailMode = 'fallback';
	private initialized = false;

	constructor(private readonly config: ConfigService) {}

	onModuleInit() {
		this.initialize();
	}

	private initialize() {
		if (this.initialized) return;

		const forceFallback =
			(this.config.get<string>('MAIL_FORCE_FALLBACK') ?? '').toLowerCase() ===
			'true';
		if (forceFallback) {
			this.useFallback();
			this.initialized = true;
			return;
		}

		if (this.tryGmailApi()) {
			this.initialized = true;
			return;
		}
		if (this.trySmtp()) {
			this.initialized = true;
			return;
		}
		this.useFallback();
		this.initialized = true;
	}

	private tryGmailApi(): boolean {
		const user = this.config.get<string>('MAIL_USERNAME');
		const clientId = this.config.get<string>('MAIL_CLIENT_ID');
		const clientSecret = this.config.get<string>('MAIL_SECRET');
		const refreshToken = this.config.get<string>('MAIL_REFRESH_TOKEN');

		if (!user || !clientId || !clientSecret || !refreshToken) {
			return false;
		}

		this.from =
			this.config.get<string>('MAIL_FROM') ?? `"42Share" <${user}>`;
		this.gmailApi = { user, clientId, clientSecret, refreshToken };
		this.mode = 'gmail-api';
		this.logger.log(
			`Mailer ready: Gmail API over HTTPS (${user}). Refresh token must include gmail.send scope.`,
		);
		return true;
	}

	private trySmtp(): boolean {
		const host = this.config.get<string>('SMTP_HOST');
		if (!host) return false;

		this.from =
			this.config.get<string>('MAIL_FROM') ?? 'no-reply@ft-transcendence.local';

		const port = Number(this.config.get<string>('SMTP_PORT') ?? 587);
		const secure =
			(this.config.get<string>('SMTP_SECURE') ?? '').toLowerCase() === 'true';
		const user = this.config.get<string>('SMTP_USER');
		const pass = this.config.get<string>('SMTP_PASS');

		this.transporter = nodemailer.createTransport({
			host,
			port,
			secure,
			connectionTimeout: SMTP_TIMEOUT_MS,
			greetingTimeout: SMTP_TIMEOUT_MS,
			socketTimeout: SMTP_TIMEOUT_MS,
			dnsOptions: { family: 4 },
			...(user && pass ? { auth: { user, pass } } : {}),
		});
		this.mode = 'smtp';
		this.logger.log(`Mailer ready: SMTP ${host}:${port} secure=${secure}`);
		return true;
	}

	private useFallback() {
		this.from =
			this.config.get<string>('MAIL_FROM') ?? 'no-reply@ft-transcendence.local';
		this.transporter = nodemailer.createTransport({ jsonTransport: true });
		this.mode = 'fallback';
		this.logger.warn(
			'Mail not configured — using jsonTransport fallback. Set Gmail OAuth (MAIL_*) or SMTP (SMTP_HOST) to send real emails.',
		);
	}

	private encodeMime(message: MailMessage): string {
		const boundary = 'ft-transcendence-mail-boundary';
		const mime = [
			`From: ${this.from}`,
			`To: ${message.to}`,
			`Subject: ${message.subject}`,
			'MIME-Version: 1.0',
			`Content-Type: multipart/alternative; boundary="${boundary}"`,
			'',
			`--${boundary}`,
			'Content-Type: text/plain; charset=UTF-8',
			'Content-Transfer-Encoding: 7bit',
			'',
			message.text,
			'',
			`--${boundary}`,
			'Content-Type: text/html; charset=UTF-8',
			'Content-Transfer-Encoding: 7bit',
			'',
			message.html,
			'',
			`--${boundary}--`,
		].join('\r\n');

		return Buffer.from(mime)
			.toString('base64')
			.replace(/\+/g, '-')
			.replace(/\//g, '_')
			.replace(/=+$/, '');
	}

	private async sendViaGmailApi(message: MailMessage): Promise<{ delivered: boolean }> {
		if (!this.gmailApi) {
			return { delivered: false };
		}

		const { clientId, clientSecret, refreshToken } = this.gmailApi;
		const oauth2 = new google.auth.OAuth2(clientId, clientSecret);
		oauth2.setCredentials({ refresh_token: refreshToken });

		const gmail = google.gmail({ version: 'v1', auth: oauth2 });
		const result = await gmail.users.messages.send({
			userId: 'me',
			requestBody: {
				raw: this.encodeMime(message),
			},
		});

		this.logger.log(
			`Email sent to ${message.to}: ${message.subject} (Gmail API id=${result.data.id})`,
		);
		return { delivered: true };
	}

	/**
	 * Queue an email without blocking the caller. GDPR actions must stay fast
	 * even when outbound mail is slow or unreachable.
	 */
	dispatch(message: MailMessage): { attempted: boolean } {
		this.initialize();

		if (this.mode === 'fallback') {
			void this.send(message);
			return { attempted: false };
		}

		void this.send(message);
		return { attempted: true };
	}

	async send(message: MailMessage): Promise<{ delivered: boolean }> {
		this.initialize();

		try {
			if (this.mode === 'gmail-api') {
				return await this.sendViaGmailApi(message);
			}

			if (!this.transporter) {
				this.logger.error('Mailer transporter unavailable after initialization');
				return { delivered: false };
			}

			const info = await this.transporter.sendMail({
				from: this.from,
				to: message.to,
				subject: message.subject,
				text: message.text,
				html: message.html,
			});

			if (this.mode === 'fallback') {
				this.logger.log(
					`Email (fallback) → ${message.to} | ${message.subject}\n${info.message?.toString?.() ?? ''}`,
				);
				return { delivered: false };
			}

			this.logger.log(
				`Email sent to ${message.to}: ${message.subject} (${info.messageId})`,
			);
			return { delivered: true };
		} catch (err) {
			this.logger.error('Email send failed', err);
			return { delivered: false };
		}
	}
}
