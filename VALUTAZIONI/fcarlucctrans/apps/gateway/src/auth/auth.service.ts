import { Inject, Injectable, Logger } from '@nestjs/common';
import { ClientProxy } from '@nestjs/microservices';
import { ConfigService } from '@nestjs/config';
import { lastValueFrom } from 'rxjs';

export interface OAuthCallbackResult {
	status: 'success' | 'error';
	accessToken?: string;
	refreshToken?: string;
	user?: {
		login: string;
		email: string;
		image?: string;
	};
}

export interface OAuthLoginUrlResponse {
	loginUrl: string;
}

@Injectable()
export class AuthService {
	private readonly logger = new Logger(AuthService.name);

	constructor(
		@Inject('AUTH_SERVICE') private readonly authClient: ClientProxy,
		private readonly configService: ConfigService,
	) {}

	getOAuthLoginUrl(): OAuthLoginUrlResponse {
		const clientId = this.configService.get<string>('CLIENT_ID')?.trim();
		const redirectUri = this.configService.get<string>('REDIRECT_URI')?.trim();

		if (!clientId || !redirectUri) {
			this.logger.error('Missing OAuth configuration: CLIENT_ID or REDIRECT_URI');
			throw new Error('OAuth configuration missing');
		}

		const loginUrl = new URL('https://api.intra.42.fr/oauth/authorize');
		loginUrl.searchParams.set('client_id', clientId);
		loginUrl.searchParams.set('redirect_uri', redirectUri);
		loginUrl.searchParams.set('response_type', 'code');
		loginUrl.searchParams.set('scope', 'public');

		return { loginUrl: loginUrl.toString() };
	}

	async handleOAuthCallback(code: string): Promise<OAuthCallbackResult> {
		if (!code) {
			this.logger.warn('OAuth callback received without code');
			throw new Error('Authorization code is required');
		}

		try {
			const result = await lastValueFrom<OAuthCallbackResult>(
				this.authClient.send({ cmd: 'validate_42_user' }, { code }),
			);

			if (result.status !== 'success') {
				this.logger.warn(`OAuth validation failed for code: ${code}`);
				throw new Error('Authentication failed');
			}

			this.logger.log(`User ${result.user?.login} authenticated successfully`);
			return result;
		} catch (error) {
			this.logger.error('Auth microservice error:', error);
			throw error;
		}
	}

	async revokeAccessSession(accessToken: string) {
		return this.revokeSession({ accessToken });
	}

	async revokeSession(input: { accessToken?: string; refreshToken?: string }) {
		try {
			return await lastValueFrom(
				this.authClient.send({ cmd: 'session_revoke' }, input),
			);
		} catch (error) {
			this.logger.error('Auth microservice error (revoke session):', error);
			return { status: 'error', message: 'Failed to revoke session' };
		}
	}

	async isUserActive(userId: string): Promise<boolean> {
		try {
			const result = await lastValueFrom<{ status: string; isActive?: boolean }>(
				this.authClient.send({ cmd: 'user_is_active' }, { userId }),
			);
			return result.status === 'success' && Boolean(result.isActive);
		} catch (error) {
			this.logger.error('Auth microservice error (is active):', error);
			return false;
		}
	}
}