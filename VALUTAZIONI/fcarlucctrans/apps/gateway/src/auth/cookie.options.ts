import type { CookieOptions } from 'express';
import { getAccessCookieMaxAgeMs } from './token-ttl';

export const REFRESH_COOKIE_MAX_AGE_MS = 14 * 24 * 60 * 60 * 1000;

export const REFRESH_COOKIE_PATH = '/api/auth';

function useSecureCookies(): boolean {
	if (process.env.COOKIE_SECURE != null) {
		return process.env.COOKIE_SECURE === 'true' || process.env.COOKIE_SECURE === '1';
	}
	return (
		process.env.NODE_ENV === 'production' ||
		(process.env.WEBAPP_URL ?? '').startsWith('https://')
	);
}

export function accessCookieOptions(): CookieOptions {
	return {
		httpOnly: true,
		sameSite: 'lax',
		secure: useSecureCookies(),
		path: '/',
		maxAge: getAccessCookieMaxAgeMs(),
	};
}

export function refreshCookieOptions(): CookieOptions {
	return {
		httpOnly: true,
		sameSite: 'lax',
		secure: useSecureCookies(),
		path: REFRESH_COOKIE_PATH,
		maxAge: REFRESH_COOKIE_MAX_AGE_MS,
	};
}

export function clearAccessCookieOptions(): CookieOptions {
	return {
		httpOnly: true,
		sameSite: 'lax',
		secure: useSecureCookies(),
		path: '/',
	};
}

export function clearRefreshCookieOptions(): CookieOptions {
	return {
		httpOnly: true,
		sameSite: 'lax',
		secure: useSecureCookies(),
		path: REFRESH_COOKIE_PATH,
	};
}
