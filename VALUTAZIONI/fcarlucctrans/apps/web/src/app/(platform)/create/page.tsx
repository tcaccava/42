import { redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { CreateProjectClient } from '@/components/CreateProjectClient';

export default async function CreateProjectPage() {
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  return <CreateProjectClient user={profile.user} />;
}
