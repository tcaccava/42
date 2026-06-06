import { fetchServerProfile } from '@/server/auth';
import { enrichFeedProjects } from '@/server/feed';
import { searchProjectsSlice } from '@/server/project';
import { FeedClient } from '@/components/FeedClient';
import { PageShell } from '@/components/ui/layout';
import { redirect } from 'next/navigation';
import { FEED_INITIAL_SIZE, SortMethod } from '@/types/search';

const feedQuery = {
  title: '',
  subject: null,
  tags: [] as string[],
  sortBy: SortMethod.Date,
  descending: true,
};

export default async function FeedPage() {
  const profile = await fetchServerProfile();

  if (profile.status !== 'success' || !profile.user) {
    redirect('/');
  }

  const [initialProjects, initialHasMore] = await searchProjectsSlice(
    feedQuery,
    0,
    FEED_INITIAL_SIZE,
  );

  const enrichedProjects = await enrichFeedProjects(initialProjects);

  return (
    <PageShell maxWidth="2xl">
      <FeedClient
        initialProjects={enrichedProjects}
        initialHasMore={initialHasMore}
      />
    </PageShell>
  );
}
