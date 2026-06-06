import { Injectable, Logger } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { HttpService } from '@nestjs/axios';
import { ConfigService } from '@nestjs/config';
import { lastValueFrom } from 'rxjs';
import crypto from 'crypto';
import { UserRepository } from './database/user.repository';
import { SessionRepository } from './database/session.repository';
import { RefreshSessionRepository } from './database/refresh-session.repository';
import { getAccessTokenExpiresInSec, getAccessTokenTtlMs } from './auth/token-ttl';

const REFRESH_TOKEN_TTL_MS = 14 * 24 * 60 * 60 * 1000;

function hashToken(token: string): string {
	return crypto.createHash('sha256').update(token).digest('hex');
}

@Injectable()
export class AppService {
	private readonly logger = new Logger(AppService.name);

	constructor(
		private readonly jwtService: JwtService,
		private readonly httpService: HttpService,
		private readonly configService: ConfigService,
		private readonly userRepository: UserRepository,
		private readonly sessionRepository: SessionRepository,
		private readonly refreshSessionRepository: RefreshSessionRepository,
	) {}

	async validate42User(code: string) {
		const clientId = this.configService.get<string>('CLIENT_ID')?.trim();
		const clientSecret = this.configService.get<string>('CLIENT_SECRET')?.trim();
		const redirectUri = this.configService.get<string>('REDIRECT_URI')?.trim();

		if (!clientId || !clientSecret || !redirectUri) {
			this.logger.error('Missing OAuth env: CLIENT_ID, CLIENT_SECRET, or REDIRECT_URI');
			return { status: 'error', message: 'OAuth configuration missing' };
		}

		const tokenBody = new URLSearchParams({
			grant_type: 'authorization_code',
			client_id: clientId,
			client_secret: clientSecret,
			code,
			redirect_uri: redirectUri,
		});

		try {
			const { data } = await lastValueFrom(
				this.httpService.post(
					'https://api.intra.42.fr/oauth/token',
					tokenBody.toString(),
					{
						headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
					},
				),
			);

			const userRes = await lastValueFrom(
				this.httpService.get('https://api.intra.42.fr/v2/me', {
					headers: { Authorization: `Bearer ${data.access_token}` },
				}),
			);

			const poolYearRaw = userRes.data.pool_year;
			const user = {
				login: userRes.data.login,
				email: userRes.data.email,
				image:
					userRes.data.image_url ??
					userRes.data.image?.link ??
					null,
				first_name: userRes.data.first_name ?? null,
				last_name: userRes.data.last_name ?? null,
				displayname: userRes.data.displayname ?? null,
				pool_month: userRes.data.pool_month ?? null,
				pool_year:
					typeof poolYearRaw === 'number'
						? poolYearRaw
						: typeof poolYearRaw === 'string'
							? (parseInt(poolYearRaw, 10) || null)
							: null,
				location: userRes.data.location ?? null,
				wallet: userRes.data.wallet ?? null,
				correction_point: userRes.data.correction_point ?? null,
				grade: userRes.data.grade ?? null,
				kind: userRes.data.kind ?? null,
				status: userRes.data.status ?? null,
			};

			const savedUser = await this.userRepository.upsertUser(user);

			// Issue tokens (access + refresh)
			const { accessToken, refreshToken } = await this.issueTokens(savedUser);

			return {
				status: 'success',
				accessToken,
				refreshToken,
				user: savedUser
			};
		} catch (error: any) {
			const apiError = error.response?.data;
			this.logger.error(
				'42 OAuth error',
				typeof apiError === 'object' ? JSON.stringify(apiError) : apiError ?? error.message,
			);

			return {
				status: 'error',
				message:
					apiError?.error_description ??
					apiError?.error ??
					'OAuth validation failed',
			};
		}
	}

	async verifyJwtToken(token: string) {
		try {
			const decoded = this.jwtService.verify(token);
			return { status: 'success', decoded };
		} catch {
			return { status: 'error', message: 'Invalid or expired token' };
		}
	}

	private async issueTokens(user: Record<string, any>) {
		if (!user?.uuid) {
			throw new Error('Cannot issue tokens: missing user uuid');
		}
		const kid = this.configService.get('JWT_KID');
		const issuer = this.configService.get('JWT_ISSUER');
		const audience = this.configService.get('JWT_AUDIENCE');

		const accessJti = crypto.randomUUID();
		const refreshJti = crypto.randomUUID();
		const accessTtlMs = getAccessTokenTtlMs(this.configService);
		const accessExpiresInSec = getAccessTokenExpiresInSec(this.configService);
		const accessExpiresAt = new Date(Date.now() + accessTtlMs);
		const refreshExpiresAt = new Date(Date.now() + REFRESH_TOKEN_TTL_MS);

		const accessTokenPayload = {
			sub: user.uuid,
			jti: accessJti,
			login: user.login,
			email: user.email,
			image: user.image ?? null,
			first_name: user.first_name ?? null,
			last_name: user.last_name ?? null,
			displayname: user.displayname ?? null,
			pool_month: user.pool_month ?? null,
			pool_year: user.pool_year ?? null,
			location: user.location ?? null,
			wallet: user.wallet ?? null,
			correction_point: user.correction_point ?? null,
			grade: user.grade ?? null,
			kind: user.kind ?? null,
			status: user.status ?? null,
		};
		const accessToken = this.jwtService.sign(accessTokenPayload, {
			algorithm: 'RS256',
			expiresIn: accessExpiresInSec,
			keyid: kid,
			issuer,
			audience,
		});

		await this.sessionRepository.create(user.uuid, accessJti, accessExpiresAt);

		const refreshToken = this.jwtService.sign(
			{ sub: user.uuid, jti: refreshJti, token_type: 'refresh' },
			{ algorithm: 'RS256', expiresIn: '14d', keyid: kid, issuer, audience },
		);

		await this.refreshSessionRepository.create(
			user.uuid,
			hashToken(refreshToken),
			refreshExpiresAt,
		);

		return { accessToken, refreshToken, refreshSessionInfo: { hash: hashToken(refreshToken) } };
	}

	async validateAccessSession(accessJti: string) {
		if (!accessJti) {
			return { status: 'error', message: 'Missing session id' };
		}

		await this.sessionRepository.deleteExpired();
		const session = await this.sessionRepository.findByAccessJti(accessJti);
		if (!session || session.expiresAt <= new Date()) {
			return { status: 'error', message: 'Session not found or expired' };
		}

		return { status: 'success' };
	}

	async revokeAccessSession(accessToken: string) {
		if (!accessToken) {
			return { status: 'success' };
		}

		try {
			const decoded = this.jwtService.verify(accessToken, { algorithms: ['RS256'] });
			const jti = decoded['jti'];
			if (typeof jti === 'string') {
				await this.sessionRepository.deleteByAccessJti(jti);
			}
		} catch {
			const decoded = this.jwtService.decode(accessToken) as { jti?: string } | null;
			if (decoded?.jti) {
				await this.sessionRepository.deleteByAccessJti(decoded.jti);
			}
		}

		return { status: 'success' };
	}

	async revokeRefreshSession(refreshToken: string) {
		if (!refreshToken) {
			return { status: 'success' };
		}
		await this.refreshSessionRepository.deleteByTokenHash(hashToken(refreshToken));
		return { status: 'success' };
	}

	async revokeAllSessions(userId: string) {
		if (!userId) return { status: 'error', message: 'userId is required' };
		await Promise.all([
			this.sessionRepository.deleteAllForUser(userId),
			this.refreshSessionRepository.deleteAllForUser(userId),
		]);
		return { status: 'success' };
	}

	async isUserActive(userId: string) {
		if (!userId) {
			return { status: 'error', message: 'userId is required' };
		}

		await this.sessionRepository.deleteExpired();
		const session = await this.sessionRepository.hasActiveSession(userId);
		return { status: 'success', isActive: Boolean(session) };
	}

	async refreshTokens(refreshToken: string) {
		const issuer = this.configService.get<string>('JWT_ISSUER');
		const audience = this.configService.get<string>('JWT_AUDIENCE');

		try {
			const decoded = this.jwtService.verify(refreshToken, {
				algorithms: ['RS256'],
				issuer,
				audience,
				clockTolerance: 5,
			});
			if (decoded['token_type'] !== 'refresh') {
				return { status: 'error', message: 'Invalid token type' };
			}

			const userUuid = decoded['sub'];
			if (!userUuid || typeof userUuid !== 'string') {
				return { status: 'error', message: 'Invalid refresh payload' };
			}

			await this.refreshSessionRepository.deleteExpired();
			const tokenHash = hashToken(refreshToken);
			const session = await this.refreshSessionRepository.findByTokenHash(tokenHash);

			if (!session) {
				this.logger.warn(
					`Refresh attempt with unknown token for user ${userUuid} — possible reuse, revoking all sessions`,
				);
				await this.revokeAllSessions(userUuid);
				return { status: 'error', message: 'Refresh token revoked' };
			}

			if (session.replacedById) {
				this.logger.warn(
					`Replay of rotated refresh token detected for user ${userUuid} — revoking all sessions`,
				);
				await this.revokeAllSessions(userUuid);
				return { status: 'error', message: 'Refresh token reuse detected' };
			}

			if (session.expiresAt <= new Date()) {
				await this.refreshSessionRepository.deleteByTokenHash(tokenHash);
				return { status: 'error', message: 'Refresh token expired' };
			}

			const user = await this.userRepository.findByUuid(userUuid);
			if (!user) {
				return { status: 'error', message: 'User not found' };
			}

			const { accessToken, refreshToken: newRefreshToken, refreshSessionInfo } =
				await this.issueTokens(user);

			const newSession = await this.refreshSessionRepository.findByTokenHash(
				refreshSessionInfo.hash,
			);
			if (newSession) {
				await this.refreshSessionRepository.markReplaced(session.id, newSession.id);
			}

			return { status: 'success', accessToken, refreshToken: newRefreshToken };
		} catch (err) {
			this.logger.warn('Refresh token verification failed', err);
			return { status: 'error', message: 'Invalid or expired refresh token' };
		}
	}

	async listDirectory(excludeUuid?: string, pageNumber = 0, pageSize = 6) {
		const [users, totalCount] = await this.userRepository.listDirectory(
			excludeUuid,
			pageNumber,
			pageSize,
		);
		const pageCount = totalCount === 0 ? 0 : Math.ceil(totalCount / pageSize);
		return {
			status: 'success',
			users: users.map((u) => ({
				userId: u.uuid,
				login: u.login,
				email: u.email,
				image: u.image,
				displayname: u.displayname,
				first_name: u.first_name,
				last_name: u.last_name,
				location: u.location,
				campus: u.location,
			})),
			pageCount,
			totalCount,
		};
	}

	async searchUsers(
		query: string,
		excludeUuid?: string,
		pageNumber = 0,
		pageSize = 6,
	) {
		const trimmed = query?.trim();
		if (!trimmed || trimmed.length < 2) {
			return { status: 'success', users: [], pageCount: 0, totalCount: 0 };
		}

		const [users, totalCount] = await this.userRepository.searchUsers(
			trimmed,
			excludeUuid,
			pageNumber,
			pageSize,
		);
		const pageCount = totalCount === 0 ? 0 : Math.ceil(totalCount / pageSize);
		return {
			status: 'success',
			users: users.map((u) => ({
				userId: u.uuid,
				login: u.login,
				email: u.email,
				image: u.image,
				displayname: u.displayname,
				first_name: u.first_name,
				last_name: u.last_name,
				location: u.location,
				campus: u.location,
			})),
			pageCount,
			totalCount,
		};
	}

	async findPublicUserByLogin(login: string) {
		const user = await this.userRepository.findPublicByLogin(login);
		if (!user) {
			return { status: 'error', message: 'User not found' };
		}

		return {
			status: 'success',
			user: {
				userId: user.uuid,
				login: user.login,
				email: user.email,
				image: user.image,
				displayname: user.displayname,
				first_name: user.first_name,
				last_name: user.last_name,
				pool_month: user.pool_month,
				pool_year: user.pool_year,
				location: user.location,
				grade: user.grade,
				kind: user.kind,
				status: user.status,
			},
		};
	}

	async findUsersByUuids(uuids: string[]) {
		const users = await this.userRepository.findManyByUuids(uuids);
		return {
			status: 'success',
			users: users.map((u) => ({
				userId: u.uuid,
				login: u.login,
				image: u.image,
				displayname: u.displayname,
				first_name: u.first_name,
				last_name: u.last_name,
			})),
		};
	}
}
