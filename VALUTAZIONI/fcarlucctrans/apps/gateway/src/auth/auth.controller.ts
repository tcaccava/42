import { Controller, Get, Post, Query, Res, UseGuards, Req, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { AuthGuard } from './auth.guard';
import { AuthService, OAuthCallbackResult } from './auth.service';
import type { Response, Request } from 'express';
import { lastValueFrom } from 'rxjs';
import {
	accessCookieOptions,
	refreshCookieOptions,
	clearAccessCookieOptions,
	clearRefreshCookieOptions,
} from './cookie.options';

@Controller('api/auth')
export class AuthController {
	private readonly logger = new Logger(AuthController.name);

	constructor(
		private readonly authService: AuthService,
		private readonly configService: ConfigService,
	) {}

	@Get('login')
	getLoginUrl(@Res() res: Response) {
		try {
			const { loginUrl } = this.authService.getOAuthLoginUrl();
			return res.json({ loginUrl });
		} catch (error) {
			this.logger.error('Failed to generate login URL:', error);
			return res.status(500).json({
				status: 'error',
				message: 'Failed to generate login URL',
			});
		}
	}

	@Get('callback')
	async handleCallback(
		@Query('code') code: string,
		@Query('error') error: string,
		@Query('error_description') errorDescription: string,
		@Res() res: Response,
	) {
		if (error) {
			this.logger.error(
				`42 OAuth denied or failed: ${error}${errorDescription ? ` — ${errorDescription}` : ''}`,
			);
			return res.redirect(`${process.env.WEBAPP_URL}/?error=auth_failed`);
		}

		try {
			const result: OAuthCallbackResult = await this.authService.handleOAuthCallback(code);

			if (result.accessToken) {
				res.cookie('access_token', result.accessToken, accessCookieOptions());
			}
			if (result.refreshToken) {
				res.cookie('refresh_token', result.refreshToken, refreshCookieOptions());
			}

			this.logger.log(`User ${result.user?.login} logged in successfully`);
			return res.redirect(`${process.env.WEBAPP_URL}`);
		} catch (error) {
			this.logger.error('OAuth callback error:', error);
			return res.redirect(`${process.env.WEBAPP_URL}/?error=auth_failed`);
		}
	}

	@Get('profile')
	@UseGuards(AuthGuard)
	async getProfile(@Req() req: Request) {
		const rawUser: any = req['user'] ?? {};
		const userId = rawUser.sub as string | undefined;
		const isActive = userId ? await this.authService.isUserActive(userId) : false;
		return {
			status: 'success',
			user: {
				userId: rawUser.sub,
				login: rawUser.login,
				email: rawUser.email,
				image: rawUser.image,
				first_name: rawUser.first_name,
				last_name: rawUser.last_name,
				displayname: rawUser.displayname,
				pool_month: rawUser.pool_month,
				pool_year: rawUser.pool_year,
				location: rawUser.location,
				wallet: rawUser.wallet,
				correction_point: rawUser.correction_point,
				grade: rawUser.grade,
				kind: rawUser.kind,
				status: rawUser.status,
				isActive,
			},
		};
	}

	@Get('logout')
	async logout(@Req() req: Request, @Res() res: Response) {
		const accessToken = req.cookies?.access_token;
		const refreshToken = req.cookies?.refresh_token;
		await this.authService.revokeSession({ accessToken, refreshToken });

		res.clearCookie('access_token', clearAccessCookieOptions());
		res.clearCookie('refresh_token', clearRefreshCookieOptions());
		// Defensive: clear refresh cookie at root path too in case an older client
		// still has one stored at "/" from a previous deploy.
		res.clearCookie('refresh_token', { ...clearRefreshCookieOptions(), path: '/' });

		this.logger.log('User logged out');
		const webapp = this.configService.get<string>('WEBAPP_URL') ?? 'http://localhost:3000';
		return res.redirect(webapp);
	}

	@Post('refresh')
	async refresh(@Req() req: Request, @Res() res: Response) {
		const refreshToken = req.cookies?.refresh_token;
		if (!refreshToken) {
			return res.status(401).json({ status: 'error', message: 'No refresh token' });
		}
		try {
			const result = await lastValueFrom(
				this.authService['authClient'].send({ cmd: 'refresh_token' }, { refreshToken }),
			);
			if (result.status !== 'success') {
				// Reuse / revoked / expired refresh: scrub cookies so the client stops looping.
				res.clearCookie('access_token', clearAccessCookieOptions());
				res.clearCookie('refresh_token', clearRefreshCookieOptions());
				return res.status(401).json(result);
			}
			if (result.accessToken) {
				res.cookie('access_token', result.accessToken, accessCookieOptions());
			}
			if (result.refreshToken) {
				res.cookie('refresh_token', result.refreshToken, refreshCookieOptions());
			}
			return res.json({ status: 'success' });
		} catch (error) {
			this.logger.error('Refresh error:', error);
			return res.status(500).json({ status: 'error', message: 'Failed to refresh tokens' });
		}
	}
}
