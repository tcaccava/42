import { fetchServerProfile } from '@/server/auth';
import LandingContent from '@/components/LandingContent';
import { redirect } from 'next/navigation';

export default async function Home() {
  const profile = await fetchServerProfile();

  if (profile.status === 'success' && profile.user) {
    redirect('/feed');
  }

  return <LandingContent />;
}
