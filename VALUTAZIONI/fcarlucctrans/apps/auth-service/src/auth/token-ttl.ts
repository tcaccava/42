import { ConfigService } from '@nestjs/config';

/** Parse ACCESS_TOKEN_TTL (e.g. `1m`, `15m`) or ACCESS_TOKEN_TTL_MS into milliseconds. */
export function getAccessTokenTtlMs(config: ConfigService): number {
	const ms = config.get<string>('ACCESS_TOKEN_TTL_MS');
	if (ms !== undefined && ms !== '') {
		const n = Number(ms);
		if (!Number.isNaN(n) && n > 0) return n;
	}
	const ttl = config.get<string>('ACCESS_TOKEN_TTL') ?? '15m';
	return parseDurationMs(ttl);
}

/** JWT `expiresIn` as seconds (avoids `StringValue` typing issues in @nestjs/jwt). */
export function getAccessTokenExpiresInSec(config: ConfigService): number {
	return Math.max(1, Math.floor(getAccessTokenTtlMs(config) / 1000));
}

function parseDurationMs(ttl: string): number {
	const match = /^(\d+)(ms|s|m|h|d)$/i.exec(ttl.trim());
	if (!match) return 15 * 60 * 1000;
	const n = Number(match[1]);
	const unit = match[2].toLowerCase();
	switch (unit) {
		case 'ms':
			return n;
		case 's':
			return n * 1000;
		case 'm':
			return n * 60 * 1000;
		case 'h':
			return n * 60 * 60 * 1000;
		case 'd':
			return n * 24 * 60 * 60 * 1000;
		default:
			return 15 * 60 * 1000;
	}
}
