'use server';

import { redirect } from 'next/navigation';
import { getApiBaseUrl, joinApiUrl } from './api-url';

function isNextRedirect(error: unknown): boolean {
  return (
    typeof error === 'object' &&
    error !== null &&
    'digest' in error &&
    typeof (error as { digest?: string }).digest === 'string' &&
    (error as { digest: string }).digest.startsWith('NEXT_REDIRECT')
  );
}

/**
 * Server Action to initiate OAuth login
 */
export async function loginAction() {
  try {
    const base = await getApiBaseUrl();
    const target = joinApiUrl(base, '/api/auth/login');
    const response = await fetch(target, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      },
      cache: 'no-store',
    });

    if (!response.ok) {
      const bodyText = await response.text().catch(() => '');
      throw new Error(
        `Failed to fetch login URL (${response.status}) from ${target}${bodyText ? ` - ${bodyText}` : ''}`,
      );
    }

    const { loginUrl } = (await response.json()) as { loginUrl: string };

    if (!loginUrl) {
      throw new Error('Gateway did not return a login URL');
    }

    redirect(loginUrl);
  } catch (error) {
    if (isNextRedirect(error)) {
      throw error;
    }

    const message = error instanceof Error ? error.message : 'Login failed';
    console.error('Login error:', message);
    throw error;
  }
}
