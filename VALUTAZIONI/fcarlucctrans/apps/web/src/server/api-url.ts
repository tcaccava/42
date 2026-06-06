import 'server-only';
import { headers } from 'next/headers';
import { joinApiUrl } from '@/lib/api-url';

export { joinApiUrl } from '@/lib/api-url';

/** Internal gateway URL for server-side proxies (upload routes). Never use NEXT_PUBLIC_* here. */
export function getGatewayUrl(): string {
  return (
    process.env.API_URL?.replace(/\/$/, '') ??
    process.env.API_BASE?.replace(/\/$/, '') ??
    'http://localhost:4000'
  );
}

/**
 * Base URL for server-side calls to the API gateway.
 *
 * - Local dev (make dev): set NEXT_PUBLIC_API_URL=https://localhost:8443
 * - Docker (nginx): uses Host / X-Forwarded-* from the incoming request
 * - Docker (direct): set API_URL=http://gateway:4000
 */
export async function getApiBaseUrl(): Promise<string> {
  const explicit =
    process.env.API_URL?.replace(/\/$/, '') ??
    process.env.NEXT_PUBLIC_API_URL?.replace(/\/$/, '');
  if (explicit) return explicit;

  const h = await headers();
  const host = h.get('x-forwarded-host') ?? h.get('host');
  const proto = h.get('x-forwarded-proto') ?? 'http';
  if (host) return `${proto}://${host.split(',')[0]!.trim()}`;

  return 'http://localhost:4000';
}
