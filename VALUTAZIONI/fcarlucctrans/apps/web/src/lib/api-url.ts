/**
 * Shared API base URL helpers (safe for middleware and server code).
 */

export function getApiBaseUrlFromRequest(requestUrl: URL): string {
  const explicit =
    process.env.API_URL?.replace(/\/$/, '') ??
    process.env.NEXT_PUBLIC_API_URL?.replace(/\/$/, '');
  if (explicit) return explicit;
  return requestUrl.origin;
}

export function joinApiUrl(base: string, path: string): string {
  if (path.startsWith('http')) return path;
  const normalized = path.startsWith('/') ? path : `/${path}`;
  return `${base.replace(/\/$/, '')}${normalized}`;
}
