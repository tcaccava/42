'use client';

import { useState, useTransition } from 'react';
import Link from 'next/link';
import { Event } from '@/types';
import EventCard from '@/components/EventCard';
import { Calendar, Plus, LayoutDashboard } from 'lucide-react';
import { local } from '@/utilities';
import { AlertBanner, EmptyState, PageHeader, PageShell } from '@/components/ui/layout';
import { LinkButton } from '@/components/ui/link-button';
import { joinEvent, leaveEvent } from '@/server/events';

type EventsProps = {
  events: Event[];
  currentUser: { login: string };
};

export function Events({ events: initialEvents, currentUser }: EventsProps) {
  const [events, setEvents] = useState<Event[]>(initialEvents);
  const [error, setError] = useState<string | null>(null);
  const [activeEventId, setActiveEventId] = useState<string | null>(null);
  const [isPending, startTransition] = useTransition();
  const t = local.useLocalization().t;

  const runParticipantAction = (
    eventId: string,
    action: 'join' | 'leave',
  ) => {
    setError(null);
    setActiveEventId(eventId);

    startTransition(async () => {
      const result =
        action === 'join'
          ? await joinEvent(eventId, currentUser.login)
          : await leaveEvent(eventId, currentUser.login);

      if (result.status === 'success' && result.event) {
        setEvents((list) =>
          list.map((event) => (event.id === eventId ? result.event! : event)),
        );
      } else if (result.status === 'error') {
        setError(result.message ?? 'Something went wrong.');
      }

      setActiveEventId(null);
    });
  };

  return (
    <PageShell className="space-y-6">
      <div className="flex flex-col gap-4 sm:flex-row sm:items-start sm:justify-between">
        <PageHeader
          icon={<Calendar className="h-5 w-5 text-teal-600" />}
          title={t.campusEvents}
          description={t.campusDescription}
          className="mb-0"
        />
        <div className="flex shrink-0 flex-wrap gap-2">
          <LinkButton href="/events/manage" variant="secondary" className="inline-flex gap-2">
            <LayoutDashboard className="h-4 w-4" />
            Backoffice
          </LinkButton>
          <LinkButton href="/events/create" className="inline-flex gap-2">
            <Plus className="h-4 w-4" />
            {t.createEvent}
          </LinkButton>
        </div>
      </div>

      {error && <AlertBanner kind="error">{error}</AlertBanner>}

      {events.length > 0 ? (
        events.map((event) => (
          <EventCard
            key={event.id}
            event={event}
            currentUser={currentUser.login}
            onJoin={(eventId) => runParticipantAction(eventId, 'join')}
            onLeave={(eventId) => runParticipantAction(eventId, 'leave')}
            disabled={isPending && activeEventId === event.id}
          />
        ))
      ) : (
        <EmptyState
          icon={<Calendar className="h-12 w-12" />}
          message={
            <>
              {t.noEvents}{' '}
              <Link href="/events/create" className="font-medium text-teal-700 hover:underline">
                {t.createEvent}
              </Link>
            </>
          }
        />
      )}
    </PageShell>
  );
}
