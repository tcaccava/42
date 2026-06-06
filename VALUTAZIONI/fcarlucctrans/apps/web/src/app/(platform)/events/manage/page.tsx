import { redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { listEvents } from '@/server/events';
import { EventsBackoffice } from '@/components/EventsBackoffice';

export default async function EventsManagePage() {
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const events = await listEvents();
  const managedEvents = events.filter(
    (event) => event.organizer === profile.user!.login,
  );

  return (
    <EventsBackoffice
      events={managedEvents}
      currentLogin={profile.user.login}
    />
  );
}
