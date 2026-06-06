'use server';

import type { FeedProject, GradedProjectCard } from '@/types/feed';
import type { ProjectCard } from '@/types/search';
import { getProjectGradeStats, getMyProjectGrade } from '@/server/project';
import { fetchPublicProfile } from '@/server/users';
import type { User } from '@/types';

function authorNameFromUser(user: User | undefined, login: string): string {
  if (!user) return login;
  const full = `${user.first_name ?? ''} ${user.last_name ?? ''}`.trim();
  return full || user.displayname || login;
}

export async function enrichProjectsWithGrades(
  cards: ProjectCard[],
): Promise<GradedProjectCard[]> {
  if (cards.length === 0) return [];

  return Promise.all(
    cards.map(async (card) => {
      const [gradeStats, ownGrade] = await Promise.all([
        getProjectGradeStats(card.id),
        getMyProjectGrade(card.id),
      ]);

      return {
        ...card,
        averageRating: gradeStats?.averageScore ?? null,
        ratingCount: gradeStats?.gradeCount ?? 0,
        likeCount: gradeStats?.favouriteCount ?? 0,
        userRating: ownGrade?.score ?? null,
        userLiked: ownGrade?.favourite ?? false,
      };
    }),
  );
}

export async function enrichFeedProjects(cards: ProjectCard[]): Promise<FeedProject[]> {
  if (cards.length === 0) return [];

  const uniqueLogins = [...new Set(cards.map((card) => card.userId))];
  const profileMap = new Map<string, User>();

  await Promise.all(
    uniqueLogins.map(async (login) => {
      const profileRes = await fetchPublicProfile(login);
      if (profileRes.status === 'success' && profileRes.user) {
        profileMap.set(login, profileRes.user);
      }
    }),
  );

  const graded = await enrichProjectsWithGrades(cards);

  return graded.map((card) => {
    const user = profileMap.get(card.userId);

    return {
      ...card,
      authorLogin: card.userId,
      authorName: authorNameFromUser(user, card.userId),
      authorImage: user?.image,
      authorCampus: user?.campus ?? user?.location ?? 'Unknown',
      createdAt: card.createdAt ?? undefined,
    };
  });
}
