'use client';

import Link from 'next/link';
import { useRouter } from 'next/navigation';
import { MessageCircle, Star, ThumbsUp } from 'lucide-react';
import { useState } from 'react';
import type { FeedProject } from '@/types/feed';
import { Badge } from '@/components/Badge';
import { submitProjectGrade } from '@/lib/projects-client';
import { FeedImageCarousel } from '@/components/FeedImageCarousel';
import { formatAppDate } from '@/lib/format-date';
import { local } from '@/utilities';

type FeedProjectCardProps = {
  project: FeedProject;
};

export function FeedProjectCard({ project }: FeedProjectCardProps) {
  const router = useRouter();
  const { locale, t } = local.useLocalization();
  const [liked, setLiked] = useState(project.userLiked);
  const [likeCount, setLikeCount] = useState(project.likeCount);
  const [rating, setRating] = useState<number | null>(project.userRating);
  const [averageRating, setAverageRating] = useState(project.averageRating);
  const [saving, setSaving] = useState(false);

  async function persistGrade(nextScore: number, nextFavourite: boolean) {
    setSaving(true);
    const saved = await submitProjectGrade(project.id, nextScore, nextFavourite);
    setSaving(false);
    if (!saved) return false;

    if (rating === null && nextScore > 0) {
      setAverageRating((prev) => {
        const count = project.ratingCount + 1;
        const total = (prev ?? 0) * project.ratingCount + nextScore;
        return count > 0 ? total / count : nextScore;
      });
    }
    return true;
  }

  async function handleLike() {
    if (saving) return;
    const nextLiked = !liked;
    const ok = await persistGrade(rating ?? 0, nextLiked);
    if (!ok) return;
    setLiked(nextLiked);
    setLikeCount((count) => Math.max(0, count + (nextLiked ? 1 : -1)));
  }

  async function handleRate(value: number) {
    if (saving) return;
    const ok = await persistGrade(value, liked);
    if (!ok) return;
    setRating(value);
  }

  const images =
    project.screenshotImages && project.screenshotImages.length > 0
      ? project.screenshotImages
      : project.coverImage
        ? [project.coverImage]
        : [
            'data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" width="800" height="450"><rect fill="%230d9488" width="100%" height="100%"/></svg>',
          ];

  return (
    <article className="overflow-hidden rounded-2xl border border-gray-200 bg-white shadow-sm">
      {/* Header */}
      <div className="flex items-center gap-3 p-4">
        <Link href={`/profile/${project.authorLogin}`} className="shrink-0">
          {/* eslint-disable-next-line @next/next/no-img-element */}
          <img
            src={project.authorImage || '/globe.svg'}
            alt={`${project.authorLogin}'s profile picture`}
            className="h-11 w-11 rounded-full border border-gray-200 object-cover"
          />
        </Link>
        <div className="min-w-0 flex-1">
          <div className="flex flex-wrap items-center gap-2">
            <Link
              href={`/profile/${project.authorLogin}`}
              className="font-semibold text-gray-900 hover:text-teal-700"
            >
              {project.authorName}
            </Link>
            <span className="text-sm text-gray-500">@{project.authorLogin}</span>
            <Badge variant="campus" campus={project.authorCampus}>
              {project.authorCampus}
            </Badge>
          </div>
          {project.createdAt ? (
            <p className="mt-0.5 text-xs text-gray-500">
              {formatAppDate(project.createdAt, locale)}
            </p>
          ) : null}
        </div>
      </div>

      {/* Cover / screenshots */}
      <FeedImageCarousel
        images={images}
        alt={project.title}
        onClick={() => router.push(`/project/${project.id}`)}
      />

      {/* Actions */}
      <div className="space-y-3 p-4">
        <div className="flex items-center gap-3">
          <button
            type="button"
            onClick={() => void handleLike()}
            disabled={saving}
            className="flex items-center gap-2 rounded-lg p-1 transition-colors hover:bg-gray-50"
            aria-pressed={liked}
            aria-label={liked ? t.unLike : t.like}
          >
            <ThumbsUp
              className={`h-6 w-6 ${liked ? 'fill-green-600 text-green-600' : 'text-gray-700'}`}
            />
          </button>
          <button
            type="button"
            onClick={() => router.push(`/project/${project.id}`)}
            className="rounded-lg p-1 transition-colors hover:bg-gray-50"
            aria-label="View project and comments"
          >
            <MessageCircle className="h-6 w-6 text-teal-700" />
          </button>
        </div>

        {/* Stars */}
        <div
          className="flex flex-wrap items-center gap-1"
          role="group"
          aria-label="Project rating"
        >
          {[1, 2, 3, 4, 5].map((value) => (
            <button
              key={value}
              type="button"
              disabled={saving}
              onClick={() => void handleRate(value)}
              className="rounded p-0.5 transition-transform hover:scale-110"
              aria-label={`Rate ${value} star${value > 1 ? 's' : ''}`}
              aria-pressed={rating === value}
            >
              <Star
                className={`h-5 w-5 ${
                  rating !== null && value <= rating
                    ? 'fill-yellow-400 text-yellow-400'
                    : 'text-gray-300'
                }`}
              />
            </button>
          ))}
          <span className="ml-2 text-sm text-gray-600">
            {rating !== null ? `${rating}/5` : t.noRating}
            {averageRating !== null && rating === null
              ? ` · avg ${averageRating.toFixed(1)}`
              : ''}
          </span>
        </div>

        <p className="text-sm font-semibold text-gray-900">
          {likeCount} {t.likes}
        </p>

        <div className="text-sm leading-relaxed text-gray-700">
          <span className="font-semibold text-gray-900">{project.authorLogin}</span>{' '}
          {project.shortDescription || 'No description yet.'}
        </div>

        <button
          type="button"
          onClick={() => router.push(`/project/${project.id}`)}
          className="text-sm font-medium text-teal-700 hover:text-teal-800"
        >
          #{project.title}
        </button>

        {project.tags.length > 0 ? (
          <div className="flex flex-wrap gap-1.5">
            {project.tags.map((tag) => (
              <Badge key={tag} variant="tag" className="text-[11px]">
                #{tag}
              </Badge>
            ))}
          </div>
        ) : null}
      </div>
    </article>
  );
}
