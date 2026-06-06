'use client';

import { useCallback, useRef, useState } from 'react';
import { useRouter } from 'next/navigation';
import { FolderKanban, Loader2 } from 'lucide-react';
import type { GradedProjectCard } from '@/types/feed';
import { PROFILE_PROJECTS_PAGE_SIZE, SortMethod } from '@/types/search';
import { searchProjects } from '@/server/project';
import { enrichProjectsWithGrades } from '@/server/feed';
import { ProjectSearchCard } from '@/components/ProjectSearchCard';
import { EmptyState } from '@/components/ui/layout';
import { useInfiniteScroll } from '@/hooks/useInfiniteScroll';

type ProfileProjectsProps = {
  userLogin: string;
  initialProjects: GradedProjectCard[];
  initialPageCount: number;
  isOwnProfile?: boolean;
};

export function ProfileProjects({
  userLogin,
  initialProjects,
  initialPageCount,
  isOwnProfile = false,
}: ProfileProjectsProps) {
  const router = useRouter();
  const [projects, setProjects] = useState(initialProjects);
  const [pageNumber, setPageNumber] = useState(0);
  const [pageCount] = useState(initialPageCount);
  const [loading, setLoading] = useState(false);
  const loadingRef = useRef(false);

  const hasMore = pageNumber + 1 < pageCount;

  const loadMore = useCallback(async () => {
    if (loadingRef.current || !hasMore) return;

    loadingRef.current = true;
    setLoading(true);

    const nextPage = pageNumber + 1;
    const [nextProjects] = await searchProjects({
      userId: userLogin,
      title: '',
      subject: null,
      tags: [],
      sortBy: SortMethod.Date,
      descending: true,
      pageNumber: nextPage,
      pageSize: PROFILE_PROJECTS_PAGE_SIZE,
    });

    const enriched = await enrichProjectsWithGrades(nextProjects);
    setProjects((current) => [...current, ...enriched]);
    setPageNumber(nextPage);
    loadingRef.current = false;
    setLoading(false);
  }, [hasMore, pageNumber, userLogin]);

  const sentinelRef = useInfiniteScroll(() => {
    void loadMore();
  }, hasMore && !loading);

  if (projects.length === 0) {
    return (
      <EmptyState
        icon={<FolderKanban className="h-10 w-10 text-gray-400" />}
        message={
          isOwnProfile
            ? 'No projects yet. Create one to showcase your work.'
            : 'This user has not published any projects yet.'
        }
      />
    );
  }

  return (
    <div className="space-y-4">
      <div className="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3">
        {projects.map((card) => (
          <ProjectSearchCard
            key={card.id}
            card={card}
            onClick={() => router.push(`/project/${card.id}`)}
          />
        ))}
      </div>

      {hasMore ? (
        <div ref={sentinelRef} className="flex justify-center py-4">
          {loading ? (
            <Loader2 className="h-6 w-6 animate-spin text-teal-600" aria-label="Loading projects" />
          ) : (
            <span className="text-sm text-gray-400">Loading…</span>
          )}
        </div>
      ) : null}
    </div>
  );
}
