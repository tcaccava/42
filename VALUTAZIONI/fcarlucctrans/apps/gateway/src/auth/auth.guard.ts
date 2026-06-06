import {
	Injectable,
	CanActivate,
	ExecutionContext,
	UnauthorizedException,
	ServiceUnavailableException,
	Logger,
} from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { ConfigService } from '@nestjs/config';
import { ClientProxy } from '@nestjs/microservices';
import { Inject } from '@nestjs/common';
import { readFileSync } from 'fs';
import path from 'path';
import { isConnectionError, sendWithRetry } from '../bootstrap/microservice-retry';

@Injectable()
export class AuthGuard implements CanActivate {
	private readonly logger = new Logger(AuthGuard.name);

	constructor(
		private readonly jwtService: JwtService,
		private readonly configService: ConfigService,
		@Inject('AUTH_SERVICE') private readonly authClient: ClientProxy,
	) {}

	async canActivate(context: ExecutionContext): Promise<boolean> {
		const request = context.switchToHttp().getRequest();
		const token = this.extractTokenFromRequest(request);

		if (!token) {
			this.logger.warn('No JWT token provided');
			throw new UnauthorizedException('No token provided');
		}

		try {
			const key = this.getPublicKey();
			if (!key) {
				this.logger.error('Missing JWT public key for verification');
				throw new UnauthorizedException('Server misconfiguration');
			}

			const issuer = this.configService.get<string>('JWT_ISSUER');
			const audience = this.configService.get<string>('JWT_AUDIENCE');

			const decoded = this.jwtService.verify(token, {
				secret: key,
				algorithms: ['RS256'],
				...(issuer ? { issuer } : {}),
				...(audience ? { audience } : {}),
				clockTolerance: 5,
			}) as Record<string, unknown>;

			const accessJti = decoded['jti'];
			if (typeof accessJti !== 'string' || !accessJti) {
				this.logger.warn('Access token missing session id (jti)');
				throw new UnauthorizedException('Invalid session');
			}

			const sessionCheck = await sendWithRetry<{ status: string }>(
				this.authClient.send({ cmd: 'session_validate' }, { accessJti }),
			);

			if (sessionCheck.status !== 'success') {
				this.logger.warn(`Session not active in database for jti ${accessJti}`);
				throw new UnauthorizedException('Session expired or revoked');
			}

			request.user = decoded;
			return true;
		} catch (error) {
			if (error instanceof UnauthorizedException) throw error;
			if (isConnectionError(error)) {
				this.logger.error('Auth microservice unreachable during session check', error);
				throw new ServiceUnavailableException(
					'Authentication service is starting, please retry',
				);
			}
			this.logger.warn('JWT validation failed:', error);
			throw new UnauthorizedException('Invalid or expired token');
		}
	}

	private getPublicKey(): string | undefined {
		const publicKeyInline = this.configService.get('JWT_PUBLIC_KEY');
		const publicKeyPath = this.configService.get('JWT_PUBLIC_KEY_PATH');
		if (publicKeyInline && publicKeyInline.includes('-----BEGIN')) {
			return publicKeyInline;
		}
		if (publicKeyPath) {
			const rootDir = path.join(__dirname, '../../../../');
			const resolvedPath = path.isAbsolute(publicKeyPath)
				? publicKeyPath
				: path.join(rootDir, publicKeyPath);
			return readFileSync(resolvedPath, 'utf8');
		}
		return undefined;
	}

	private extractTokenFromRequest(request: any): string | null {
		if (request.cookies?.access_token) {
			return request.cookies.access_token;
		}

		const authHeader = request.headers?.authorization;
		if (authHeader && typeof authHeader === 'string') {
			const parts = authHeader.split(' ');
			if (parts.length === 2 && parts[0] === 'Bearer') {
				return parts[1];
			}
		}

		return null;
	}
}
