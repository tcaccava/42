import { Logger } from '@nestjs/common';
import { lastValueFrom, type Observable } from 'rxjs';

const logger = new Logger('MicroserviceRetry');

function isConnectionError(err: unknown): boolean {
	if (!err || typeof err !== 'object') return false;
	const code = (err as { code?: string }).code;
	if (code === 'ECONNREFUSED' || code === 'ECONNRESET' || code === 'ETIMEDOUT') {
		return true;
	}
	const errors = (err as { errors?: unknown[] }).errors;
	if (Array.isArray(errors)) {
		return errors.some(isConnectionError);
	}
	return false;
}

/**
 * Call a microservice with short retries when the peer is still starting.
 */
export async function sendWithRetry<T>(
	observable: Observable<T>,
	opts: { attempts?: number; delayMs?: number } = {},
): Promise<T> {
	const { attempts = 5, delayMs = 400 } = opts;
	let lastError: unknown;

	for (let attempt = 1; attempt <= attempts; attempt++) {
		try {
			return await lastValueFrom(observable);
		} catch (err) {
			lastError = err;
			if (!isConnectionError(err) || attempt === attempts) {
				throw err;
			}
			logger.warn(
				`Microservice unreachable (attempt ${attempt}/${attempts}), retrying in ${delayMs}ms…`,
			);
			await new Promise((r) => setTimeout(r, delayMs));
		}
	}

	throw lastError;
}

export { isConnectionError };
