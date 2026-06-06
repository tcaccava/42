'use server';

import { revalidatePath } from 'next/cache';
import type { Event } from '@/types';
import { fetchWithAuth } from '@/server/http';
import { SEARCH_PAGE_SIZE } from '@/types/search';

function revalidateEventPaths() {
  revalidatePath('/events');
  revalidatePath('/events/manage');
}

type ApiEventResult = {
  status: 'success' | 'error';
  event?: Event;
  events?: Event[];
  pageCount?: number;
  totalCount?: number;
  message?: string;
};

export type PaginatedEventsResult = {
  status: 'success' | 'error' | 'unauthorized';
  events: Event[];
  pageCount: number;
  totalCount: number;
  message?: string;
};

async function parseEventResponse(res: Response): Promise<ApiEventResult> {
  if (res.status === 401) {
    return { status: 'error', message: 'Unauthorized' };
  }
  const body = (await res.json().catch(() => ({}))) as ApiEventResult;
  if (!res.ok) {
    return { status: 'error', message: body.message ?? `Request failed (${res.status})` };
  }
  if (body.status === 'error') {
    return body;
  }
  return body;
}

export type EventActionResult =
  | { status: 'success'; event?: Event }
  | { status: 'error'; message: string };

export type CreateEventInput = {
  campus: string;
  organizer: string;
  organizerImage: string;
  title: string;
  description: string;
  date: string;
  time: string;
  location: string;
  maxParticipants: number;
  tags: string[];
  coverImage?: string;
};

export type UpdateEventInput = {
  campus: string;
  title: string;
  description: string;
  date: string;
  time: string;
  location: string;
  maxParticipants: number;
  tags: string[];
  coverImage?: string;
};

export async function listEvents(): Promise<Event[]> {
  try {
    const res = await fetchWithAuth('/api/events', { method: 'GET' });
    const data = await parseEventResponse(res);
    return data.status === 'success' ? (data.events ?? []) : [];
  } catch {
    return [];
  }
}

export async function searchEvents(
  query = '',
  pageNumber = 0,
  pageSize = SEARCH_PAGE_SIZE,
): Promise<PaginatedEventsResult> {
  try {
    const params = new URLSearchParams({
      pageNumber: String(pageNumber),
      pageSize: String(pageSize),
    });
    if (query.trim()) {
      params.set('q', query.trim());
    }

    const res = await fetchWithAuth(`/api/events/search?${params}`, { method: 'GET' });
    if (res.status === 401) {
      return {
        status: 'unauthorized',
        events: [],
        pageCount: 0,
        totalCount: 0,
      };
    }

    const data = (await res.json()) as ApiEventResult;
    if (!res.ok || data.status !== 'success') {
      return {
        status: 'error',
        events: [],
        pageCount: 0,
        totalCount: 0,
        message: data.message ?? `Search failed (${res.status})`,
      };
    }

    return {
      status: 'success',
      events: data.events ?? [],
      pageCount: data.pageCount ?? 0,
      totalCount: data.totalCount ?? 0,
    };
  } catch (err) {
    return {
      status: 'error',
      events: [],
      pageCount: 0,
      totalCount: 0,
      message: err instanceof Error ? err.message : 'Event search failed',
    };
  }
}

export async function getEventById(id: string): Promise<Event | null> {
  try {
    const res = await fetchWithAuth(`/api/events/${encodeURIComponent(id)}`, {
      method: 'GET',
    });
    const data = await parseEventResponse(res);
    return data.status === 'success' ? (data.event ?? null) : null;
  } catch {
    return null;
  }
}

export async function createEvent(input: CreateEventInput): Promise<EventActionResult> {
  try {
    const res = await fetchWithAuth('/api/events', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        title: input.title,
        description: input.description,
        organizerImage: input.organizerImage,
        campus: input.campus,
        date: input.date,
        time: input.time,
        location: input.location,
        maxParticipants: input.maxParticipants,
        tags: input.tags,
        coverImage: input.coverImage,
      }),
    });
    const data = await parseEventResponse(res);
    if (data.status !== 'success') {
      return { status: 'error', message: data.message ?? 'Could not create event.' };
    }
    revalidateEventPaths();
    return { status: 'success', event: data.event };
  } catch {
    return { status: 'error', message: 'Could not create event.' };
  }
}

export async function deleteEvent(
  eventId: string,
  _login: string,
): Promise<EventActionResult> {
  try {
    const res = await fetchWithAuth(`/api/events/${encodeURIComponent(eventId)}`, {
      method: 'DELETE',
    });
    const data = await parseEventResponse(res);
    if (data.status !== 'success') {
      return { status: 'error', message: data.message ?? 'Could not delete event.' };
    }
    revalidateEventPaths();
    return { status: 'success' };
  } catch {
    return { status: 'error', message: 'Could not delete event.' };
  }
}

export async function updateEvent(
  eventId: string,
  _login: string,
  input: UpdateEventInput,
): Promise<EventActionResult> {
  try {
    const res = await fetchWithAuth(`/api/events/${encodeURIComponent(eventId)}`, {
      method: 'PATCH',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(input),
    });
    const data = await parseEventResponse(res);
    if (data.status !== 'success') {
      return { status: 'error', message: data.message ?? 'Could not update event.' };
    }
    revalidateEventPaths();
    return { status: 'success', event: data.event };
  } catch {
    return { status: 'error', message: 'Could not update event.' };
  }
}

export async function joinEvent(eventId: string, _login: string): Promise<EventActionResult> {
  try {
    const res = await fetchWithAuth(`/api/events/${encodeURIComponent(eventId)}/join`, {
      method: 'POST',
    });
    const data = await parseEventResponse(res);
    if (data.status !== 'success') {
      return { status: 'error', message: data.message ?? 'Could not join event.' };
    }
    revalidateEventPaths();
    return { status: 'success', event: data.event };
  } catch {
    return { status: 'error', message: 'Could not join event.' };
  }
}

export async function leaveEvent(eventId: string, _login: string): Promise<EventActionResult> {
  try {
    const res = await fetchWithAuth(`/api/events/${encodeURIComponent(eventId)}/leave`, {
      method: 'POST',
    });
    const data = await parseEventResponse(res);
    if (data.status !== 'success') {
      return { status: 'error', message: data.message ?? 'Could not leave event.' };
    }
    revalidateEventPaths();
    return { status: 'success', event: data.event };
  } catch {
    return { status: 'error', message: 'Could not leave event.' };
  }
}
