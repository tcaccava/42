'use client';

import { useState } from 'react';
import { useRouter } from 'next/navigation';
import type { User } from '@/types';
import { EventForm } from '@/components/EventForm';
import { createEvent } from '@/server/events';

export function CreateEventClient({ user }: { user: User }) {
  const router = useRouter();
  const [isSubmitting, setIsSubmitting] = useState(false);

  return (
    <EventForm
      currentUser={user}
      mode="create"
      isSubmitting={isSubmitting}
      onCancel={() => router.push('/events')}
      onSubmit={async (values) => {
        setIsSubmitting(true);
        try {
          const result = await createEvent({
            campus: user.campus ?? 'campus',
            organizer: user.login,
            organizerImage: user.image ?? '',
            ...values,
          });
          if (result.status === 'success') {
            router.push('/events/manage');
            router.refresh();
          }
        } finally {
          setIsSubmitting(false);
        }
      }}
    />
  );
}
