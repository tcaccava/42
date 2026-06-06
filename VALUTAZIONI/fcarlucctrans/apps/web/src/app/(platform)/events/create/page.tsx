import { redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { CreateEventClient } from '@/components/CreateEventClient';

export default async function CreateEventPage() {
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  return <CreateEventClient user={profile.user} />;
}
