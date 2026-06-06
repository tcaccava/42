'use server';

import { revalidatePath } from 'next/cache';
import { fetchWithAuth } from './http';

export type EditableProfilePatch = {
  bio?: string;
  username?: string;
  website?: string;
  github?: string;
  linkedin?: string;
  campus?: string;
  coverImage?: string;
};

export type UserProfileResponse = {
  status: 'success' | 'error' | 'unauthorized';
  profile?: {
    userId?: string;
    login: string;
    bio?: string | null;
    username?: string | null;
    website?: string | null;
    github?: string | null;
    linkedin?: string | null;
    campus?: string | null;
    coverImage?: string | null;
    createdAt?: string;
    updatedAt?: string;
  };
  message?: string;
};

export async function fetchMyEditableProfile(): Promise<UserProfileResponse> {
  try {
    const res = await fetchWithAuth('/api/user/profile', { method: 'GET' });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Profile fetch failed (${res.status})` };

    return (await res.json()) as UserProfileResponse;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function updateMyEditableProfile(
  patch: EditableProfilePatch,
): Promise<UserProfileResponse> {
  try {
    const res = await fetchWithAuth('/api/user/profile', {
      method: 'PATCH',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(patch ?? {}),
    });

    if (res.status === 401) return { status: 'unauthorized' };
    if (res.status === 413) {
      return { status: 'error', message: 'Image is too large. Try a smaller cover photo.' };
    }
    if (!res.ok) return { status: 'error', message: `Profile update failed (${res.status})` };

    const data = (await res.json()) as UserProfileResponse;
    revalidatePath('/profile');
    return data;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}
