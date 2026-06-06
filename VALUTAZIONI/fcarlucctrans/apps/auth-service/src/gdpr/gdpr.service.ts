import { Injectable, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import crypto from 'crypto';
import { UserRepository } from '../database/user.repository';
import { SessionRepository } from '../database/session.repository';
import { RefreshSessionRepository } from '../database/refresh-session.repository';
import { DataRequestRepository } from '../database/data-request.repository';
import { PrismaService } from '../database/prisma.service';
import { MailerService } from '../email/mailer.service';
import {
	deletionRequestEmail,
	deletionCompletedEmail,
	anonymizationRequestEmail,
	anonymizationCompletedEmail,
	exportNotificationEmail,
} from '../email/templates';
import type { DataRequestType } from '../generated/prisma/index.js';

const TOKEN_TTL_MIN = 60;

type GdprResult<T = Record<string, unknown>> = T & {
	status: 'success' | 'error';
	message?: string;
};

@Injectable()
export class GdprService {
	private readonly logger = new Logger(GdprService.name);

	constructor(
		private readonly config: ConfigService,
		private readonly mailer: MailerService,
		private readonly userRepository: UserRepository,
		private readonly sessionRepository: SessionRepository,
		private readonly refreshSessionRepository: RefreshSessionRepository,
		private readonly dataRequestRepository: DataRequestRepository,
		private readonly prisma: PrismaService,
	) {}

	/**
	 * GDPR "right of access". Builds a JSON snapshot of everything stored in
	 * auth-service for `userId`, plus a confirmation email so the user has an
	 * audit trail of when they pulled their data.
	 */
	async exportAuthData(userId: string): Promise<GdprResult<{ data?: unknown }>> {
		const user = await this.userRepository.findByUuid(userId);
		if (!user) return { status: 'error', message: 'User not found' };

		const [sessions, refreshSessions, dataRequests] = await Promise.all([
			this.prisma.userSession.findMany({
				where: { userId },
				select: { id: true, accessJti: true, expiresAt: true, createdAt: true },
			}),
			this.prisma.refreshSession.findMany({
				where: { userId },
				select: {
					id: true,
					expiresAt: true,
					createdAt: true,
					replacedById: true,
				},
			}),
			this.prisma.dataRequest.findMany({
				where: { userId },
				select: {
					id: true,
					type: true,
					status: true,
					expiresAt: true,
					createdAt: true,
					consumedAt: true,
				},
			}),
		]);

		const data = {
			profile: {
				uuid: user.uuid,
				login: user.login,
				email: user.email,
				image: user.image,
				first_name: user.first_name,
				last_name: user.last_name,
				displayname: user.displayname,
				pool_month: user.pool_month,
				pool_year: user.pool_year,
				location: user.location,
				wallet: user.wallet,
				correction_point: user.correction_point,
				grade: user.grade,
				kind: user.kind,
				status: user.status,
				createdAt: user.createdAt,
				updatedAt: user.updatedAt,
			},
			activeSessions: sessions,
			refreshSessions,
			dataRequests,
		};

		// Best-effort confirmation email; export is already streamed to the client.
		this.mailer.dispatch({
			to: user.email,
			...exportNotificationEmail({
				login: user.login,
				when: new Date().toISOString(),
			}),
		});

		return { status: 'success', data };
	}

	async requestDeletion(userId: string): Promise<GdprResult<{ delivered?: boolean }>> {
		return this.requestConfirmation(userId, 'DELETE_ACCOUNT');
	}

	async requestAnonymization(userId: string): Promise<GdprResult<{ delivered?: boolean }>> {
		return this.requestConfirmation(userId, 'ANONYMIZE');
	}

	private async requestConfirmation(
		userId: string,
		type: DataRequestType,
	): Promise<GdprResult<{ delivered?: boolean }>> {
		const user = await this.userRepository.findByUuid(userId);
		if (!user) return { status: 'error', message: 'User not found' };

		await this.dataRequestRepository.deleteExpired();
		await this.dataRequestRepository.cancelPendingForUser(userId, type);

		const token = crypto.randomBytes(32).toString('hex');
		const expiresAt = new Date(Date.now() + TOKEN_TTL_MIN * 60 * 1000);
		await this.dataRequestRepository.create(userId, type, token, expiresAt);

		const webappUrl = this.config.get<string>('WEBAPP_URL') ?? 'http://localhost:3000';
		const path = type === 'DELETE_ACCOUNT' ? 'delete' : 'anonymize';
		const confirmUrl = `${webappUrl}/settings/privacy/confirm/${path}?token=${token}`;

		this.logger.log(
			`GDPR ${type} confirmation for ${user.login} (expires ${expiresAt.toISOString()}): ${confirmUrl}`,
		);

		const ctx = {
			login: user.login,
			confirmUrl,
			expiresMinutes: TOKEN_TTL_MIN,
		};
		const message =
			type === 'DELETE_ACCOUNT'
				? deletionRequestEmail(ctx)
				: anonymizationRequestEmail(ctx);

		const { attempted } = this.mailer.dispatch({ to: user.email, ...message });
		return { status: 'success', delivered: attempted };
	}

	async confirmDeletion(token: string): Promise<
		GdprResult<{ userId?: string; login?: string; email?: string }>
	> {
		const verified = await this.verifyToken(token, 'DELETE_ACCOUNT');
		if (!verified.ok) return { status: 'error', message: verified.message };

		const user = verified.user;
		const login = user.login;
		const email = user.email;

		// Deleting the User row cascades UserSession, RefreshSession, DataRequest
		// thanks to the foreign-key ON DELETE CASCADE we set up in the migrations.
		await this.userRepository.delete(user.uuid);

		this.mailer.dispatch({
			to: email,
			...deletionCompletedEmail({ login, when: new Date().toISOString() }),
		});

		return { status: 'success', userId: user.uuid, login, email };
	}

	async confirmAnonymization(token: string): Promise<
		GdprResult<{ userId?: string; login?: string; previousLogin?: string }>
	> {
		const verified = await this.verifyToken(token, 'ANONYMIZE');
		if (!verified.ok) return { status: 'error', message: verified.message };

		const user = verified.user;
		const newLogin = `anon-${user.uuid.slice(0, 8)}`;
		const newEmail = `${newLogin}@anonymized.local`;

		await this.userRepository.anonymize(user.uuid, {
			login: newLogin,
			email: newEmail,
		});

		await Promise.all([
			this.sessionRepository.deleteAllForUser(user.uuid),
			this.refreshSessionRepository.deleteAllForUser(user.uuid),
		]);

		await this.dataRequestRepository.markCompleted(verified.requestId);

		this.mailer.dispatch({
			to: user.email,
			...anonymizationCompletedEmail({
				login: user.login,
				when: new Date().toISOString(),
			}),
		});

		return { status: 'success', userId: user.uuid, login: newLogin, previousLogin: user.login };
	}

	private async verifyToken(token: string, expectedType: DataRequestType) {
		if (!token || typeof token !== 'string') {
			return { ok: false as const, message: 'Missing token' };
		}

		await this.dataRequestRepository.deleteExpired();
		const request = await this.dataRequestRepository.findActiveByToken(token);

		if (!request) {
			return { ok: false as const, message: 'Invalid or expired token' };
		}
		if (request.type !== expectedType) {
			return { ok: false as const, message: 'Token type mismatch' };
		}
		if (request.status !== 'PENDING' || request.expiresAt <= new Date()) {
			return { ok: false as const, message: 'Token already used or expired' };
		}

		const user = await this.userRepository.findByUuid(request.userId);
		if (!user) return { ok: false as const, message: 'User not found' };

		return { ok: true as const, user, requestId: request.id };
	}
}
