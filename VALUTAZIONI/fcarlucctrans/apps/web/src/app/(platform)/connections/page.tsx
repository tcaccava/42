import { redirect } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { fetchConnections } from '@/server/connections';
import { ConnectionsManager } from '@/components/ConnectionsManager';

export default async function ConnectionsPage() {
  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const [pendingRes, acceptedRes] = await Promise.all([
    fetchConnections('PENDING'),
    fetchConnections('ACCEPTED'),
  ]);

  const pending = pendingRes.status === 'success' ? pendingRes.connections ?? [] : [];
  const accepted = acceptedRes.status === 'success' ? acceptedRes.connections ?? [] : [];

  return (
    <ConnectionsManager pending={pending} accepted={accepted} />
  );
}
