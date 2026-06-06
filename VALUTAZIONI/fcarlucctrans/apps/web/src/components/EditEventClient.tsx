'use client';

import { useState } from 'react';
import { useRouter } from 'next/navigation';
import type { Event } from '@/types';
import type { User } from '@/types';
import { EventForm } from '@/components/EventForm';
import { AlertBanner } from '@/components/ui/layout';
import { updateEvent } from '@/server/events';

export function EditEventClient({ user, event }: { user: User; event: Event }) {
  const router = useRouter();
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [error, setError] = useState<string | null>(null);

  return (
    <>
      {error && (
        <div className="mx-auto max-w-2xl px-4 pt-8">
          <AlertBanner kind="error">{error}</AlertBanner>
        </div>
      )}
      <EventForm
        currentUser={user}
        mode="edit"
        isSubmitting={isSubmitting}
        minParticipants={event.participants.length}
          initialValues={{
          title: event.title,
          description: event.description,
          date: event.date,
          time: event.time,
          location: event.location,
          maxParticipants: event.maxParticipants,
          tags: event.tags,
          coverImage: event.coverImage ?? '',
        }}
        onCancel={() => router.push('/events/manage')}
        onSubmit={async (values) => {
          setError(null);
          setIsSubmitting(true);
          try {
            const result = await updateEvent(event.id, user.login, {
              ...values,
              campus: user.campus ?? event.campus,
            });
            if (result.status === 'success') {
              router.push('/events/manage');
              router.refresh();
              return;
            }
            setError(result.message ?? 'Could not update event.');
          } finally {
            setIsSubmitting(false);
          }
        }}
      />
    </>
  );
}
