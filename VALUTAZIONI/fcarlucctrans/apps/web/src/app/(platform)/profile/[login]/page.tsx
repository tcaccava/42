import { redirect, notFound } from 'next/navigation';
import { fetchServerProfile } from '@/server/auth';
import { fetchPublicProfile } from '@/server/users';
import { enrichProjectsWithGrades } from '@/server/feed';
import { searchProjects } from '@/server/project';
import { Profile } from '@/components/Profile';
import type { User } from '@/types';
import { PROFILE_PROJECTS_PAGE_SIZE, SortMethod } from '@/types/search';

type PageProps = {
  params: Promise<{ login: string }>;
};

export default async function PublicProfilePage({ params }: PageProps) {
  const { login } = await params;

  const [authProfile, publicProfile] = await Promise.all([
    fetchServerProfile(),
    fetchPublicProfile(login),
  ]);

  if (authProfile.status !== 'success' || !authProfile.user) {
    redirect('/');
  }

  if (publicProfile.status !== 'success' || !publicProfile.user) {
    notFound();
  }

  if (publicProfile.isOwnProfile) {
    redirect('/profile');
  }

  const user = {
    ...publicProfile.user,
    campus: publicProfile.user.campus ?? publicProfile.user.location,
  } satisfies User;

  const [projects, projectsPageCount] = await searchProjects({
    userId: user.login,
    title: '',
    subject: null,
    tags: [],
    sortBy: SortMethod.Date,
    descending: true,
    pageNumber: 0,
    pageSize: PROFILE_PROJECTS_PAGE_SIZE,
  });

  const gradedProjects = await enrichProjectsWithGrades(projects);

  return (
    <div className="py-8 px-4">
      <Profile
        user={user}
        projects={gradedProjects}
        projectsPageCount={projectsPageCount}
        isOwnProfile={false}
        connectionRelation={publicProfile.connection?.relation ?? 'none'}
        connectionId={publicProfile.connection?.connectionId}
        connectionCount={publicProfile.connectionCount}
        isActive={publicProfile.isActive}
      />
    </div>
  );
}
