import { fetchServerProfile } from '@/server/auth';
import { EditProjectClient } from '@/components/EditProjectClient';
import { User } from '@/types';
import { getProject } from '@/server/project';
import { notFound, redirect } from 'next/navigation';

export default async function EditProjectPage({
  searchParams,
}: {
  searchParams: Promise<{ projectId?: string }>;
}) {
  const { projectId: projectIdRaw } = await searchParams;

  const profile = await fetchServerProfile();
  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const projectId = Number.parseInt(projectIdRaw ?? '', 10);
  if (!Number.isFinite(projectId)) {
    notFound();
  }

  const projectData = await getProject(projectId);
  if (!projectData) {
    notFound();
  }

  const currentUser: User = {
    ...profile.user,
    campus: profile.user.location || profile.user.campus || '42 Campus',
    bio: profile.user.bio,
  };

  return <EditProjectClient user={currentUser} project={projectData} />;
}
