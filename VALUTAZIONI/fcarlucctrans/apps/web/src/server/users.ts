'use server';

import { revalidatePath } from 'next/cache';
import { fetchWithAuth } from './http';
import type { PublicProfileResponse, ConnectionRelation } from '@/types/connection';
import type { User } from '@/types';
import { SEARCH_PAGE_SIZE } from '@/types/search';

export type UserSearchResult = User & {
	connection?: {
		relation: ConnectionRelation;
		connectionId?: string;
	};
};

export type PaginatedUsersResult = {
	status: 'success' | 'error' | 'unauthorized';
	users?: UserSearchResult[];
	pageCount?: number;
	totalCount?: number;
	message?: string;
};

function mapApiUser(raw: Record<string, unknown>): UserSearchResult {
  const connection = raw.connection as
    | { relation?: ConnectionRelation; connectionId?: string }
    | undefined;

  return {
    userId: raw.userId as string | undefined,
    login: raw.login as string,
    email: (raw.email as string) ?? '',
    image: (raw.image as string) ?? undefined,
    displayname: raw.displayname as string | undefined,
    first_name: raw.first_name as string | undefined,
    last_name: raw.last_name as string | undefined,
    location: raw.location as string | undefined,
    campus: (raw.campus as string) ?? (raw.location as string) ?? undefined,
    isActive: Boolean(raw.isActive),
    ...(connection
      ? {
          connection: {
            relation: connection.relation ?? 'none',
            connectionId: connection.connectionId,
          },
        }
      : {}),
  };
}

function parseUsersResponse(
  res: Response,
  data: Record<string, unknown>,
): PaginatedUsersResult {
  if (res.status === 401) return { status: 'unauthorized' };
  if (!res.ok) {
    return { status: 'error', message: `Request failed (${res.status})` };
  }
  if (data.status !== 'success' || !Array.isArray(data.users)) {
    return { status: 'error', message: 'Invalid user list response' };
  }

  return {
    status: 'success',
    users: data.users.map((u) => mapApiUser(u as Record<string, unknown>)),
    pageCount: Number(data.pageCount ?? 0),
    totalCount: Number(data.totalCount ?? 0),
  };
}

export async function fetchUserDirectory(
  pageNumber = 0,
  pageSize = SEARCH_PAGE_SIZE,
): Promise<PaginatedUsersResult> {
  try {
    const params = new URLSearchParams({
      pageNumber: String(pageNumber),
      pageSize: String(pageSize),
    });
    const res = await fetchWithAuth(`/api/user/directory?${params}`, { method: 'GET' });
    const data = (await res.json()) as Record<string, unknown>;
    return parseUsersResponse(res, data);
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function searchUsers(
  query: string,
  pageNumber = 0,
  pageSize = SEARCH_PAGE_SIZE,
): Promise<PaginatedUsersResult> {
  try {
    const trimmed = query.trim();
    if (trimmed.length < 2) {
      return { status: 'success', users: [], pageCount: 0, totalCount: 0 };
    }

    const params = new URLSearchParams({
      q: trimmed,
      pageNumber: String(pageNumber),
      pageSize: String(pageSize),
    });
    const res = await fetchWithAuth(`/api/user/search?${params}`, { method: 'GET' });
    const data = (await res.json()) as Record<string, unknown>;
    return parseUsersResponse(res, data);
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function fetchPublicProfile(login: string): Promise<PublicProfileResponse> {
  try {
    const res = await fetchWithAuth(
      `/api/user/by-login/${encodeURIComponent(login)}`,
      { method: 'GET' },
    );

    if (res.status === 401) return { status: 'unauthorized' };
    if (res.status === 404) return { status: 'error', message: 'User not found' };
    if (!res.ok) return { status: 'error', message: `Profile fetch failed (${res.status})` };

    return (await res.json()) as PublicProfileResponse;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function revalidateProfilePaths(login: string) {
  revalidatePath(`/profile/${login}`);
  revalidatePath('/connections');
  revalidatePath('/search');
}
