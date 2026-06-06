'use server';

import { revalidatePath } from 'next/cache';
import { fetchWithAuth } from './http';
import type {
  ConnectionActionResponse,
  ConnectionsResponse,
} from '@/types/connection';

export async function fetchConnections(
  status?: 'PENDING' | 'ACCEPTED',
): Promise<ConnectionsResponse> {
  try {
    const params = status ? `?status=${status}` : '';
    const res = await fetchWithAuth(`/api/user/connections${params}`, {
      method: 'GET',
    });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Failed (${res.status})` };

    return (await res.json()) as ConnectionsResponse;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function sendConnectionRequest(
  targetUserId: string,
): Promise<ConnectionActionResponse> {
  try {
    const res = await fetchWithAuth('/api/user/connections', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ targetUserId }),
    });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Failed (${res.status})` };

    const data = (await res.json()) as ConnectionActionResponse;
    revalidatePath('/connections');
    revalidatePath('/search');
    return data;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function acceptConnectionRequest(
  connectionId: string,
): Promise<ConnectionActionResponse> {
  try {
    const res = await fetchWithAuth(
      `/api/user/connections/${connectionId}/accept`,
      { method: 'POST' },
    );

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Failed (${res.status})` };

    const data = (await res.json()) as ConnectionActionResponse;
    revalidatePath('/connections');
    return data;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function rejectConnectionRequest(
  connectionId: string,
): Promise<ConnectionActionResponse> {
  try {
    const res = await fetchWithAuth(
      `/api/user/connections/${connectionId}/reject`,
      { method: 'POST' },
    );

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Failed (${res.status})` };

    const data = (await res.json()) as ConnectionActionResponse;
    revalidatePath('/connections');
    return data;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}

export async function removeOrCancelConnection(
  connectionId: string,
): Promise<ConnectionActionResponse> {
  try {
    const res = await fetchWithAuth(`/api/user/connections/${connectionId}`, {
      method: 'DELETE',
    });

    if (res.status === 401) return { status: 'unauthorized' };
    if (!res.ok) return { status: 'error', message: `Failed (${res.status})` };

    const data = (await res.json()) as ConnectionActionResponse;
    revalidatePath('/connections');
    revalidatePath('/search');
    return data;
  } catch (err) {
    const message = err instanceof Error ? err.message : 'Unknown server error';
    return { status: 'error', message };
  }
}
