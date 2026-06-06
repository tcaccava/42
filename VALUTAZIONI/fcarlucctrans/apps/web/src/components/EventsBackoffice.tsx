'use client';

import { useState, useTransition } from 'react';
import Link from 'next/link';
import { useRouter } from 'next/navigation';
import {
  Calendar,
  Clock,
  MapPin,
  Trash2,
  Users,
  AlertTriangle,
  LayoutDashboard,
  Pencil,
} from 'lucide-react';
import type { Event } from '@/types';
import { Button } from '@/components/Button';
import { deleteEvent } from '@/server/events';
import {
  AlertBanner,
  EmptyState,
  PageHeader,
  PageShell,
  SectionCard,
} from '@/components/ui/layout';
import { LinkButton } from '@/components/ui/link-button';

type EventsBackofficeProps = {
  events: Event[];
  currentLogin: string;
};

export function EventsBackoffice({ events: initialEvents, currentLogin }: EventsBackofficeProps) {
  const router = useRouter();
  const [events, setEvents] = useState(initialEvents);
  const [error, setError] = useState<string | null>(null);
  const [confirmingId, setConfirmingId] = useState<string | null>(null);
  const [pendingId, setPendingId] = useState<string | null>(null);
  const [isPending, startTransition] = useTransition();

  const handleDelete = (eventId: string) => {
    setError(null);
    setPendingId(eventId);

    startTransition(async () => {
      const result = await deleteEvent(eventId, currentLogin);
      if (result.status === 'success') {
        setEvents((list) => list.filter((event) => event.id !== eventId));
        setConfirmingId(null);
        router.refresh();
      } else {
        setError(result.message ?? 'Could not delete event.');
      }
      setPendingId(null);
    });
  };

  return (
    <PageShell className="space-y-6">
      <div className="flex flex-col gap-4 sm:flex-row sm:items-start sm:justify-between">
        <PageHeader
          icon={<LayoutDashboard className="h-5 w-5 text-teal-600" />}
          title="Event backoffice"
          description="Monitor registrations and manage events you organize."
          className="mb-0"
        />
        <LinkButton href="/events" variant="secondary" className="shrink-0">
          Back to events
        </LinkButton>
      </div>

      {error && <AlertBanner kind="error">{error}</AlertBanner>}

      {events.length === 0 ? (
        <EmptyState
          icon={<Calendar className="h-12 w-12" />}
          message={
            <>
              You have not created any events yet.{' '}
              <Link href="/events/create" className="font-medium text-teal-700 hover:underline">
                Create one
              </Link>
            </>
          }
        />
      ) : (
        events.map((event) => {
          const isConfirming = confirmingId === event.id;
          const isDeleting = isPending && pendingId === event.id;

          return (
            <SectionCard
              key={event.id}
              title={event.title}
              description={`${event.participants.length} / ${event.maxParticipants} registered`}
              icon={<Calendar className="h-5 w-5 text-teal-600" />}
              contentClassName="space-y-4"
            >
              <div className="grid gap-2 text-sm text-gray-600 sm:grid-cols-3">
                <div className="flex items-center gap-2">
                  <Calendar className="h-4 w-4 text-teal-600" />
                  {event.date}
                </div>
                <div className="flex items-center gap-2">
                  <Clock className="h-4 w-4 text-teal-600" />
                  {event.time}
                </div>
                <div className="flex items-center gap-2">
                  <MapPin className="h-4 w-4 text-teal-600" />
                  {event.location}
                </div>
              </div>

              <div>
                <div className="mb-3 flex items-center gap-2">
                  <Users className="h-4 w-4 text-teal-600" />
                  <h3 className="text-sm font-semibold text-gray-900">
                    Registered participants ({event.participants.length})
                  </h3>
                </div>
                {event.participants.length === 0 ? (
                  <p className="text-sm text-gray-500">No one has joined yet.</p>
                ) : (
                  <ul className="space-y-2">
                    {event.participants.map((login) => (
                      <li
                        key={login}
                        className="flex items-center justify-between rounded-xl border border-gray-100 bg-gray-50 px-4 py-2 text-sm"
                      >
                        <Link
                          href={`/profile/${login}`}
                          className="font-medium text-gray-900 hover:text-teal-700"
                        >
                          @{login}
                        </Link>
                        {login === event.organizer && (
                          <span className="rounded-full bg-teal-50 px-2 py-0.5 text-xs font-medium text-teal-700">
                            Organizer
                          </span>
                        )}
                      </li>
                    ))}
                  </ul>
                )}
              </div>

              {!isConfirming ? (
                <div className="flex flex-wrap gap-2">
                  <LinkButton
                    href={`/events/${event.id}/edit`}
                    variant="secondary"
                    className="inline-flex gap-2"
                  >
                    <Pencil className="h-4 w-4" />
                    Edit event
                  </LinkButton>
                  <Button
                    variant="danger"
                    onClick={() => {
                      setConfirmingId(event.id);
                      setError(null);
                    }}
                    className="inline-flex gap-2"
                  >
                    <Trash2 className="h-4 w-4" />
                    Delete event
                  </Button>
                </div>
              ) : (
                <div className="space-y-3 rounded-xl border border-red-200 bg-red-50/40 p-4">
                  <div className="flex items-start gap-2 text-sm text-red-900">
                    <AlertTriangle className="mt-0.5 h-4 w-4 shrink-0" />
                    <p>
                      Delete <strong>{event.title}</strong>? All registrations will be
                      removed. This cannot be undone.
                    </p>
                  </div>
                  <div className="flex flex-wrap gap-2">
                    <Button
                      variant="danger"
                      onClick={() => handleDelete(event.id)}
                      disabled={isDeleting}
                      className="inline-flex gap-2"
                    >
                      <Trash2 className="h-4 w-4" />
                      {isDeleting ? 'Deleting…' : 'Confirm delete'}
                    </Button>
                    <Button
                      variant="secondary"
                      onClick={() => setConfirmingId(null)}
                      disabled={isDeleting}
                    >
                      Cancel
                    </Button>
                  </div>
                </div>
              )}
            </SectionCard>
          );
        })
      )}
    </PageShell>
  );
}
