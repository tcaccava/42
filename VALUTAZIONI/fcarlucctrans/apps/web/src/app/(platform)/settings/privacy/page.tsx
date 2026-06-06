import { redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { PrivacySettingsClient } from './privacy-settings-client';

export const dynamic = 'force-dynamic';

export default async function PrivacySettingsPage() {
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  return <PrivacySettingsClient userEmail={profile.user.email} />;
}
