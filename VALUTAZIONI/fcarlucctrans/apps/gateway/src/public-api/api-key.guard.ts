import {
	CanActivate,
	ExecutionContext,
	Injectable,
	UnauthorizedException,
	HttpException,
	Logger,
	Inject,
} from '@nestjs/common';
import { ClientProxy } from '@nestjs/microservices';
import { lastValueFrom } from 'rxjs';

export type ValidatedApiKey = {
	id: string;
	name: string;
	prefix: string;
	requestsPerMinute: number;
};

type Bucket = { count: number; resetAt: number };

/**
 * Validates `X-API-Key` against user-service and enforces per-key rate limits
 * using the `requestsPerMinute` value stored in the database.
 */
@Injectable()
export class ApiKeyGuard implements CanActivate {
	private readonly logger = new Logger(ApiKeyGuard.name);
	private readonly buckets = new Map<string, Bucket>();

	constructor(@Inject('USER_SERVICE') private readonly userClient: ClientProxy) {}

	async canActivate(context: ExecutionContext): Promise<boolean> {
		const request = context.switchToHttp().getRequest();
		const rawKey = this.extractKey(request);

		if (!rawKey) {
			throw new UnauthorizedException(
				'Missing API key. Send header X-API-Key or Authorization: Bearer <key>',
			);
		}

		const result = await lastValueFrom<{
			status: string;
			message?: string;
			key?: ValidatedApiKey;
		}>(this.userClient.send({ cmd: 'public_api_validate_key' }, { apiKey: rawKey }));

		if (result.status !== 'success' || !result.key) {
			throw new UnauthorizedException(result.message ?? 'Invalid API key');
		}

		this.enforceRateLimit(result.key);
		request.publicApiKey = result.key;
		return true;
	}

	private extractKey(request: {
		headers?: Record<string, string | string[] | undefined>;
	}): string | null {
		const header = request.headers?.['x-api-key'];
		if (typeof header === 'string' && header.trim()) return header.trim();

		const auth = request.headers?.authorization;
		if (typeof auth === 'string' && auth.toLowerCase().startsWith('bearer ')) {
			return auth.slice(7).trim();
		}

		return null;
	}

	private enforceRateLimit(key: ValidatedApiKey) {
		const now = Date.now();
		let bucket = this.buckets.get(key.id);

		if (!bucket || now >= bucket.resetAt) {
			bucket = { count: 0, resetAt: now + 60_000 };
		}

		bucket.count += 1;
		this.buckets.set(key.id, bucket);

		if (bucket.count > key.requestsPerMinute) {
			const retryAfter = Math.ceil((bucket.resetAt - now) / 1000);
			this.logger.warn(`Rate limit exceeded for API key ${key.prefix}…`);
			throw new HttpException(
				{
					status: 'error',
					message: 'Rate limit exceeded',
					limit: key.requestsPerMinute,
					windowSeconds: 60,
					retryAfter,
				},
				429,
			);
		}
	}
}
