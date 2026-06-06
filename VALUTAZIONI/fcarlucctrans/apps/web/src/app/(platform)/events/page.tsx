import { listEvents } from '@/server/events';
import { fetchServerProfile } from '@/server/auth';
import { Events } from '@/components/Events';
import { redirect } from 'next/navigation';

export default async function EventsPage() {
  const [profile, events] = await Promise.all([
    fetchServerProfile(),
    listEvents(),
  ]);

  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  return (
    <Events 
      events={events} 
      currentUser={profile.user}
    />
  );
}
