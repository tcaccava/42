import { fetchWithAuth } from './http';
import type { AuthResponse } from '@/types';

/**
 * Fetch user profile from gateway.
 *
 * Uses fetchWithAuth so an expired access token is transparently refreshed
 * (in concert with the edge middleware) instead of bubbling a 401 to the UI.
 */
export async function fetchServerProfile(): Promise<AuthResponse> {
  try {
    const res = await fetchWithAuth('/api/auth/profile', { method: 'GET' });

    if (!res.ok) {
      if (res.status === 401) {
        return { status: 'unauthorized', user: undefined };
      }
      return { status: 'error', message: `Profile fetch failed (${res.status})` };
    }

    return (await res.json()) as AuthResponse;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}
