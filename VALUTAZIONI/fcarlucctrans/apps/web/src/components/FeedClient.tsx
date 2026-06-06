'use client';

import { useCallback, useRef, useState } from 'react';
import { Loader2, Newspaper } from 'lucide-react';
import type { FeedProject } from '@/types/feed';
import { FEED_PAGE_SIZE, SortMethod } from '@/types/search';
import { searchProjectsSlice } from '@/server/project';
import { enrichFeedProjects } from '@/server/feed';
import { FeedProjectCard } from '@/components/FeedProjectCard';
import { EmptyState } from '@/components/ui/layout';
import { useInfiniteScroll } from '@/hooks/useInfiniteScroll';

const feedQuery = {
  title: '',
  subject: null,
  tags: [] as string[],
  sortBy: SortMethod.Date,
  descending: true,
};

type FeedClientProps = {
  initialProjects: FeedProject[];
  initialHasMore: boolean;
};

export function FeedClient({ initialProjects, initialHasMore }: FeedClientProps) {
  const [projects, setProjects] = useState(initialProjects);
  const [hasMore, setHasMore] = useState(initialHasMore);
  const [loading, setLoading] = useState(false);
  const offsetRef = useRef(initialProjects.length);
  const loadingRef = useRef(false);

  const loadMore = useCallback(async () => {
    if (loadingRef.current || !hasMore) return;

    loadingRef.current = true;
    setLoading(true);

    const offset = offsetRef.current;
    const [nextProjects, nextHasMore] = await searchProjectsSlice(
      feedQuery,
      offset,
      FEED_PAGE_SIZE,
    );

    const enriched = await enrichFeedProjects(nextProjects);
    offsetRef.current = offset + nextProjects.length;
    setProjects((current) => [...current, ...enriched]);
    setHasMore(nextHasMore);
    loadingRef.current = false;
    setLoading(false);
  }, [hasMore]);

  const sentinelRef = useInfiniteScroll(() => {
    void loadMore();
  }, hasMore && !loading);

  if (projects.length === 0) {
    return (
      <EmptyState
        icon={<Newspaper className="h-10 w-10 text-gray-400" />}
        message="No projects yet. Publish a project to share it with the community."
      />
    );
  }

  return (
    <div className="space-y-4">
      <div className="flex flex-col gap-5">
        {projects.map((project) => (
          <FeedProjectCard key={project.id} project={project} />
        ))}
      </div>

      {hasMore ? (
        <div ref={sentinelRef} className="flex justify-center py-6">
          {loading ? (
            <Loader2
              className="h-6 w-6 animate-spin text-teal-600"
              aria-label="Loading more projects"
            />
          ) : null}
        </div>
      ) : projects.length > 0 ? (
        <p className="pb-4 text-center text-sm text-gray-400">You&apos;re all caught up.</p>
      ) : null}
    </div>
  );
}
