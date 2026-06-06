'use server';

import { cookies } from 'next/headers';
import { fetchWithAuth } from './http';
import { getApiBaseUrl, joinApiUrl } from './api-url';

export type GdprActionResult = {
  status: 'success' | 'error' | 'unauthorized';
  message?: string;
  delivered?: boolean;
};

/**
 * Fetch the user's full data export from the gateway (already authenticated)
 * and return it as a serializable string the client can hand to a Blob and
 * download. Returns null on failure.
 */
export async function downloadMyDataExport(): Promise<{
  status: 'success' | 'error' | 'unauthorized';
  filename?: string;
  contents?: string;
  message?: string;
}> {
  try {
    const res = await fetchWithAuth('/api/user/me/export', { method: 'GET' });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Export failed (${res.status})` };

    const disposition = res.headers.get('content-disposition') ?? '';
    const match = /filename="([^"]+)"/.exec(disposition);
    const filename = match?.[1] ?? `ft-transcendence-export-${Date.now()}.json`;

    const contents = await res.text();
    return { status: 'success', filename, contents };
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function requestAccountDeletion(): Promise<GdprActionResult> {
  return postGdpr('/api/user/me/delete-request');
}

export async function requestDataAnonymization(): Promise<GdprActionResult> {
  return postGdpr('/api/user/me/anonymize-request');
}

async function postGdpr(path: string): Promise<GdprActionResult> {
  try {
    const res = await fetchWithAuth(path, { method: 'POST' });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Request failed (${res.status})` };

    return (await res.json()) as GdprActionResult;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

/**
 * Confirmation endpoints: token-based, do NOT require auth (the email link
 * works in any browser session). After success the gateway scrubs cookies,
 * so we also clear the local Next.js cookie store to keep the SSR view in
 * sync.
 */
export async function confirmDeletion(token: string): Promise<GdprActionResult> {
  return confirm('/api/user/me/delete-confirm', token);
}

export async function confirmAnonymization(token: string): Promise<GdprActionResult> {
  return confirm('/api/user/me/anonymize-confirm', token);
}

async function confirm(path: string, token: string): Promise<GdprActionResult> {
  try {
    const base = await getApiBaseUrl();
    const res = await fetch(joinApiUrl(base, path), {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        accept: 'application/json',
      },
      body: JSON.stringify({ token }),
      cache: 'no-store',
    });

    const body = (await res.json().catch(() => ({}))) as GdprActionResult;
    if (!res.ok) {
      return {
        status: 'error',
        message: body.message ?? `Confirmation failed (${res.status})`,
      };
    }

    const store = await cookies();
    store.delete({ name: 'access_token', path: '/' });
    store.delete({ name: 'refresh_token', path: '/' });
    store.delete({ name: 'refresh_token', path: '/api/auth' });

    return { ...body, status: 'success' };
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}
