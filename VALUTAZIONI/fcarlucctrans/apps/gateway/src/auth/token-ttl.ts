/** Keep cookie maxAge aligned with auth-service JWT TTL (ACCESS_TOKEN_TTL / ACCESS_TOKEN_TTL_MS). */
export function getAccessCookieMaxAgeMs(): number {
	const ms = process.env.ACCESS_TOKEN_TTL_MS;
	if (ms !== undefined && ms !== '') {
		const n = Number(ms);
		if (!Number.isNaN(n) && n > 0) return n;
	}
	const ttl = process.env.ACCESS_TOKEN_TTL ?? '15m';
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
