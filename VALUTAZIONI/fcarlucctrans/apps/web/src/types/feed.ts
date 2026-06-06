import type { ProjectCard } from '@/types/search';

export type GradedProjectCard = ProjectCard & {
  averageRating: number | null;
  ratingCount: number;
  likeCount: number;
  userRating: number | null;
  userLiked: boolean;
};

export type FeedProject = GradedProjectCard & {
  authorLogin: string;
  authorName: string;
  authorImage?: string;
  authorCampus: string;
  createdAt?: string;
};

export type ProjectGradeStats = {
  projectId: number;
  averageScore: number | null;
  gradeCount: number;
  favouriteCount: number;
};

export type UserProjectGrade = {
  score: number;
  favourite: boolean;
};
