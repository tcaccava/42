import { notFound, redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { getEventById } from '@/server/events';
import { EditEventClient } from '@/components/EditEventClient';

type PageProps = {
  params: Promise<{ id: string }>;
};

export default async function EditEventPage({ params }: PageProps) {
  const { id } = await params;
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const event = await getEventById(id);
  if (!event || event.organizer !== profile.user.login) {
    notFound();
  }

  return <EditEventClient user={profile.user} event={event} />;
}
