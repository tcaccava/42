import 'server-only';
import { cookies, headers } from 'next/headers';
import { getApiBaseUrl, joinApiUrl } from './api-url';

function useSecureCookies(): boolean {
  if (process.env.COOKIE_SECURE != null) {
    return process.env.COOKIE_SECURE === 'true' || process.env.COOKIE_SECURE === '1';
  }
  return (
    process.env.NODE_ENV === 'production' ||
    (process.env.WEBAPP_URL ?? '').startsWith('https://')
  );
}

type AuthContext = {
  cookieHeader: string;
  accessToken?: string;
  refreshToken?: string;
};

type RefreshedTokens = {
  accessToken?: string;
  refreshToken?: string;
};

/**
 * Server-side fetch helper that:
 *
 *  1. Forwards the incoming HttpOnly cookies to the gateway.
 *  2. Refreshes proactively when only a refresh cookie is present.
 *  3. On a 401, transparently calls `/api/auth/refresh` once, applies rotated
 *     cookies when possible, and retries with the new access token.
 *  4. Returns the final `Response`. Callers still own response shape.
 */
export async function fetchWithAuth(
  path: string,
  init: RequestInit = {},
): Promise<Response> {
  const base = await getApiBaseUrl();
  const url = joinApiUrl(base, path);

  let auth = await getAuthContext();
  if (!auth.accessToken && auth.refreshToken) {
    const refreshed = await refreshAccessToken(base);
    if (refreshed) {
      auth = mergeAuthContext(auth, refreshed);
    }
  }

  const first = await fetchWithCookies(url, init, auth);
  if (first.status !== 401) return first;

  const refreshed = await refreshAccessToken(base);
  if (!refreshed?.accessToken) return first;

  return fetchWithCookies(url, init, mergeAuthContext(auth, refreshed));
}

async function fetchWithCookies(
  url: string,
  init: RequestInit,
  auth: AuthContext,
): Promise<Response> {
  const reqHeaders = new Headers(init.headers);
  const cookieHeader = buildCookieHeader(auth);
  if (cookieHeader) reqHeaders.set('cookie', cookieHeader);
  if (auth.accessToken && !reqHeaders.has('authorization')) {
    reqHeaders.set('authorization', `Bearer ${auth.accessToken}`);
  }
  if (!reqHeaders.has('accept')) reqHeaders.set('accept', 'application/json');

  return fetch(url, {
    cache: 'no-store',
    ...init,
    headers: reqHeaders,
  });
}

async function getAuthContext(): Promise<AuthContext> {
  const store = await cookies();
  const fromStore = store
    .getAll()
    .map((c) => `${c.name}=${c.value}`)
    .join('; ');

  const incoming = (await headers()).get('cookie') ?? '';
  const cookieHeader = fromStore || incoming;

  const accessToken =
    store.get('access_token')?.value ??
    parseCookieValue(incoming, 'access_token');
  const refreshToken =
    store.get('refresh_token')?.value ??
    parseCookieValue(incoming, 'refresh_token');

  return { cookieHeader, accessToken, refreshToken };
}

function mergeAuthContext(
  auth: AuthContext,
  refreshed: RefreshedTokens,
): AuthContext {
  const accessToken = refreshed.accessToken ?? auth.accessToken;
  const refreshToken = refreshed.refreshToken ?? auth.refreshToken;
  return {
    cookieHeader: buildCookieHeader({ ...auth, accessToken, refreshToken }),
    accessToken,
    refreshToken,
  };
}

function buildCookieHeader(auth: AuthContext): string {
  const cookieMap = new Map<string, string>();
  for (const part of auth.cookieHeader.split(';')) {
    const trimmed = part.trim();
    if (!trimmed) continue;
    const eq = trimmed.indexOf('=');
    if (eq === -1) continue;
    cookieMap.set(trimmed.slice(0, eq), trimmed.slice(eq + 1));
  }
  if (auth.accessToken) cookieMap.set('access_token', auth.accessToken);
  if (auth.refreshToken) cookieMap.set('refresh_token', auth.refreshToken);
  return Array.from(cookieMap.entries())
    .map(([k, v]) => `${k}=${v}`)
    .join('; ');
}

function parseCookieValue(cookieHeader: string, name: string): string | undefined {
  if (!cookieHeader) return undefined;
  for (const part of cookieHeader.split(';')) {
    const trimmed = part.trim();
    const eq = trimmed.indexOf('=');
    if (eq === -1) continue;
    if (trimmed.slice(0, eq) === name) {
      return trimmed.slice(eq + 1);
    }
  }
  return undefined;
}

async function refreshAccessToken(base: string): Promise<RefreshedTokens | null> {
  const store = await cookies();
  const { refreshToken: refreshFromContext } = await getAuthContext();
  const refresh = refreshFromContext ?? store.get('refresh_token')?.value;
  if (!refresh) return null;

  let res: Response;
  try {
    res = await fetch(joinApiUrl(base, '/api/auth/refresh'), {
      method: 'POST',
      headers: { cookie: `refresh_token=${refresh}` },
      cache: 'no-store',
    });
  } catch {
    return null;
  }

  if (!res.ok) return null;

  const setCookieHeaders = readSetCookieHeaders(res);
  const refreshed: RefreshedTokens = {};
  for (const sc of setCookieHeaders) {
    const parsed = parseSetCookie(sc);
    if (!parsed) continue;
    if (parsed.name === 'access_token') refreshed.accessToken = parsed.value;
    if (parsed.name === 'refresh_token') refreshed.refreshToken = parsed.value;
    try {
      store.set({
        name: parsed.name,
        value: parsed.value,
        httpOnly: true,
        sameSite: 'lax',
        secure: useSecureCookies(),
        path: parsed.path ?? '/',
        ...(parsed.maxAge ? { maxAge: parsed.maxAge } : {}),
      });
    } catch {
      // Outside a route handler / server action, cookies().set is read-only.
    }
  }

  return refreshed.accessToken ? refreshed : null;
}

function readSetCookieHeaders(res: Response): string[] {
  const anyHeaders = res.headers as Headers & { getSetCookie?: () => string[] };
  if (typeof anyHeaders.getSetCookie === 'function') {
    return anyHeaders.getSetCookie();
  }
  const single = res.headers.get('set-cookie');
  return single ? [single] : [];
}

function parseSetCookie(
  header: string,
): { name: string; value: string; path?: string; maxAge?: number } | null {
  const parts = header.split(';').map((p) => p.trim());
  const first = parts[0];
  if (!first) return null;
  const eq = first.indexOf('=');
  if (eq === -1) return null;
  const out: { name: string; value: string; path?: string; maxAge?: number } = {
    name: first.slice(0, eq),
    value: first.slice(eq + 1),
  };
  for (const attr of parts.slice(1)) {
    const lower = attr.toLowerCase();
    if (lower.startsWith('path=')) out.path = attr.slice(5);
    else if (lower.startsWith('max-age=')) {
      const n = Number(attr.slice(8));
      if (!Number.isNaN(n)) out.maxAge = n;
    }
  }
  return out;
}
