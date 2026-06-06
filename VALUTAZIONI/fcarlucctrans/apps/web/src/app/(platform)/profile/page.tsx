import { fetchServerProfile } from '@/server/auth';
import { fetchMyEditableProfile } from '@/server/user-profile';
import { enrichProjectsWithGrades } from '@/server/feed';
import { searchProjects } from '@/server/project';
import { Profile } from '@/components/Profile';
import { redirect } from 'next/navigation';
import { User } from '@/types';
import { PROFILE_PROJECTS_PAGE_SIZE, SortMethod } from '@/types/search';

export default async function ProfilePage() {
  const [profile, editableProfile] = await Promise.all([
    fetchServerProfile(),
    fetchMyEditableProfile(),
  ]);

  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const editable =
    editableProfile.status === 'success' ? editableProfile.profile : undefined;

  const currentUser: User = {
    ...profile.user,
    campus: editable?.campus ?? profile.user.location ?? undefined,
    bio: editable?.bio ?? undefined,
    username: editable?.username ?? undefined,
    website: editable?.website ?? undefined,
    github: editable?.github ?? undefined,
    linkedin: editable?.linkedin ?? undefined,
    coverImage: editable?.coverImage ?? undefined,
  };

  const [projects, projectsPageCount] = await searchProjects({
    userId: currentUser.login,
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
        user={currentUser}
        projects={gradedProjects}
        projectsPageCount={projectsPageCount}
        isOwnProfile={true}
      />
    </div>
  );
}
