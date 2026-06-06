import {
	BadRequestException,
	Body,
	Controller,
	Get,
	Post,
	Req,
	Res,
	UnauthorizedException,
	UseGuards,
} from '@nestjs/common';
import type { Request, Response } from 'express';
import { AuthGuard } from '../auth/auth.guard';
import { AuthService } from '../auth/auth.service';
import {
	clearAccessCookieOptions,
	clearRefreshCookieOptions,
} from '../auth/cookie.options';
import { GdprService } from './gdpr.service';

/**
 * GDPR endpoints.
 *
 * - `/me/export`             — auth required. Streams a JSON snapshot.
 * - `/me/delete-request`     — auth required. Sends confirmation email.
 * - `/me/anonymize-request`  — auth required. Sends confirmation email.
 * - `/me/delete-confirm`     — token-based, NO auth (email link works in
 *                              any browser session).
 * - `/me/anonymize-confirm`  — token-based, NO auth.
 */
@Controller('api/user/me')
export class GdprController {
	constructor(
		private readonly gdprService: GdprService,
		private readonly authService: AuthService,
	) {}

	@UseGuards(AuthGuard)
	@Get('export')
	async export(@Req() req: Request, @Res() res: Response) {
		const uuid = (req as any).user?.sub;
		const login = (req as any).user?.login;
		if (!uuid || !login) throw new UnauthorizedException('Missing user identity');

		const result = await this.gdprService.exportAll({ uuid, login });
		if (result.status !== 'success') {
			return res.status(500).json(result);
		}

		const filename = `ft-transcendence-${login}-${new Date()
			.toISOString()
			.slice(0, 10)}.json`;

		res.setHeader('Content-Type', 'application/json; charset=utf-8');
		res.setHeader(
			'Content-Disposition',
			`attachment; filename="${filename}"`,
		);
		return res.send(JSON.stringify(result.data, null, 2));
	}

	@UseGuards(AuthGuard)
	@Post('delete-request')
	async requestDeletion(@Req() req: Request) {
		const uuid = (req as any).user?.sub;
		if (!uuid) throw new UnauthorizedException('Missing user id');
		return this.gdprService.requestDeletion(uuid);
	}

	@UseGuards(AuthGuard)
	@Post('anonymize-request')
	async requestAnonymization(@Req() req: Request) {
		const uuid = (req as any).user?.sub;
		if (!uuid) throw new UnauthorizedException('Missing user id');
		return this.gdprService.requestAnonymization(uuid);
	}

	@Post('delete-confirm')
	async confirmDeletion(
		@Body() body: { token?: string },
		@Res() res: Response,
	) {
		const token = body?.token?.trim();
		if (!token) throw new BadRequestException('token is required');

		const result = await this.gdprService.confirmDeletion(token);
		if (result.status !== 'success') {
			return res.status(400).json(result);
		}

		// User row is gone; their cookies on this device should be too.
		res.clearCookie('access_token', clearAccessCookieOptions());
		res.clearCookie('refresh_token', clearRefreshCookieOptions());
		res.clearCookie('refresh_token', { ...clearRefreshCookieOptions(), path: '/' });
		return res.json(result);
	}

	@Post('anonymize-confirm')
	async confirmAnonymization(
		@Body() body: { token?: string },
		@Res() res: Response,
	) {
		const token = body?.token?.trim();
		if (!token) throw new BadRequestException('token is required');

		const result = await this.gdprService.confirmAnonymization(token);
		if (result.status !== 'success') {
			return res.status(400).json(result);
		}

		// Anonymization revokes existing sessions in auth-service; clear cookies too.
		res.clearCookie('access_token', clearAccessCookieOptions());
		res.clearCookie('refresh_token', clearRefreshCookieOptions());
		res.clearCookie('refresh_token', { ...clearRefreshCookieOptions(), path: '/' });
		return res.json(result);
	}
}
