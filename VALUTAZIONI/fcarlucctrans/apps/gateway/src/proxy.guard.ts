import { Injectable, CanActivate, ExecutionContext, UnauthorizedException, Logger } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { ConfigService } from '@nestjs/config';
import { Observable } from 'rxjs';
import { readFileSync } from 'fs';
import path from 'path';

// Yoinked it all from the auth.guard.ts
// This guard is used in Project- and Comment-backend controllers.
// It is placed here for code de-duplication purposes.
@Injectable()
export class ProxyGuard implements CanActivate {
	private readonly logger = new Logger(ProxyGuard.name);

	constructor(
		private readonly jwtService: JwtService,
		private readonly configService: ConfigService,
	) {}

	canActivate(
		context: ExecutionContext,
	): boolean | Promise<boolean> | Observable<boolean> {
		const request = context.switchToHttp().getRequest();

		// If the request doesn't contain the /secure/ path, then its all good :)
		if (!this.isRequestSecure(request)) {
			return true;
		}

		const token = this.extractTokenFromRequest(request);

		if (!token) {
			this.logger.warn('No JWT token provided');
			throw new UnauthorizedException('No token provided');
		}

		try {
			const publicKeyInline = this.configService.get('JWT_PUBLIC_KEY');
			const publicKeyPath = this.configService.get('JWT_PUBLIC_KEY_PATH');
			let key: string | undefined;
			if (publicKeyInline && publicKeyInline.includes('-----BEGIN')) {
				key = publicKeyInline;
			} else if (publicKeyPath) {
				const rootDir = path.join(__dirname, '../../../');
				const resolvedPath = path.isAbsolute(publicKeyPath) ? publicKeyPath : path.join(rootDir, publicKeyPath);
				key = readFileSync(resolvedPath, 'utf8');
			}
			if (!key) {
				this.logger.error('Missing JWT public key for verification');
				throw new UnauthorizedException('Server misconfiguration');
			}
			const decoded = this.jwtService.verify(token, { secret: key, algorithms: ['RS256'] });

			request.user = decoded;
			return true;
		} catch (error) {
			this.logger.warn('JWT validation failed:', error);
			throw new UnauthorizedException('Invalid or expired token');
		}
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

	private isRequestSecure(request: any): boolean {
		const segments: string[] = request.url.split('/');
		return !!(segments.find((s) => s == 'secure'));
	}
}
