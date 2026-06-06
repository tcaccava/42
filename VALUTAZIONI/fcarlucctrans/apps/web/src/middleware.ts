import { NextResponse } from 'next/server';
import type { NextRequest } from 'next/server';
import { getApiBaseUrlFromRequest, joinApiUrl } from '@/lib/api-url';

/**
 * Proactive token refresh middleware.
 *
 * Runs on the Edge before every page/server-action request. If the access
 * cookie is missing or about to expire (and a refresh cookie is present), it
 * calls the gateway `/api/auth/refresh` once and forwards the new Set-Cookie
 * headers back to the browser AND to the downstream Next.js request — so the
 * incoming SSR/server-action sees the fresh access token immediately.
 *
 * This is the change that fixes the "logged out after 15 minutes" UX: the
 * client never sees a 401 in steady state.
 */

// Refresh when the access token has less than this much life left.
const REFRESH_LEEWAY_MS = 60 * 1000;

export async function middleware(request: NextRequest) {
  const apiBase = getApiBaseUrlFromRequest(request.nextUrl);
  const accessToken = request.cookies.get('access_token')?.value;
  const refreshToken = request.cookies.get('refresh_token')?.value;

  if (!refreshToken) {
    return NextResponse.next();
  }

  const needsRefresh = !accessToken || isExpiringSoon(accessToken);
  if (!needsRefresh) {
    return NextResponse.next();
  }

  let refreshRes: Response;
  try {
    refreshRes = await fetch(joinApiUrl(apiBase, '/api/auth/refresh'), {
      method: 'POST',
      headers: { cookie: `refresh_token=${refreshToken}` },
      cache: 'no-store',
    });
  } catch {
    return NextResponse.next();
  }

  if (!refreshRes.ok) {
    if (refreshRes.status === 401) {
      const res = NextResponse.next();
      res.cookies.delete('access_token');
      res.cookies.delete('refresh_token');
      return res;
    }
    return NextResponse.next();
  }

  const setCookieHeaders = readSetCookieHeaders(refreshRes);
  if (setCookieHeaders.length === 0) {
    return NextResponse.next();
  }

  const requestHeaders = new Headers(request.headers);
  const updatedCookieHeader = mergeCookies(
    request.headers.get('cookie') ?? '',
    setCookieHeaders,
  );
  requestHeaders.set('cookie', updatedCookieHeader);

  const response = NextResponse.next({ request: { headers: requestHeaders } });
  for (const sc of setCookieHeaders) {
    response.headers.append('set-cookie', sc);
  }
  return response;
}

function isExpiringSoon(jwt: string): boolean {
  try {
    const [, payload] = jwt.split('.');
    if (!payload) return true;
    const json = JSON.parse(
      Buffer.from(payload.replace(/-/g, '+').replace(/_/g, '/'), 'base64').toString('utf8'),
    ) as { exp?: number };
    if (typeof json.exp !== 'number') return true;
    return json.exp * 1000 - Date.now() < REFRESH_LEEWAY_MS;
  } catch {
    return true;
  }
}

function readSetCookieHeaders(res: Response): string[] {
  const anyHeaders = res.headers as Headers & { getSetCookie?: () => string[] };
  if (typeof anyHeaders.getSetCookie === 'function') {
    return anyHeaders.getSetCookie();
  }
  const single = res.headers.get('set-cookie');
  return single ? [single] : [];
}

function mergeCookies(existingCookieHeader: string, setCookies: string[]): string {
  const cookieMap = new Map<string, string>();
  if (existingCookieHeader) {
    for (const part of existingCookieHeader.split(';')) {
      const trimmed = part.trim();
      if (!trimmed) continue;
      const eq = trimmed.indexOf('=');
      if (eq === -1) continue;
      cookieMap.set(trimmed.slice(0, eq), trimmed.slice(eq + 1));
    }
  }
  for (const sc of setCookies) {
    const first = sc.split(';')[0]?.trim();
    if (!first) continue;
    const eq = first.indexOf('=');
    if (eq === -1) continue;
    cookieMap.set(first.slice(0, eq), first.slice(eq + 1));
  }
  return Array.from(cookieMap.entries())
    .map(([k, v]) => `${k}=${v}`)
    .join('; ');
}

export const config = {
  matcher: [
    // Run on every request EXCEPT Next.js internals and static assets.
    // Server actions POST back to the same Next.js URL so they're covered too.
    '/((?!_next/static|_next/image|favicon.ico|.*\\..*).*)',
  ],
};
