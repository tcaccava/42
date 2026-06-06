'use client';

import { GemIcon, Star, ThumbsUp } from 'lucide-react';
import type { GradedProjectCard } from '@/types/feed';
import type { ProjectCard } from '@/types/search';
import { Badge } from '@/components/Badge';

type ProjectSearchCardProps = {
  card: ProjectCard | GradedProjectCard;
  onClick: () => void;
  layout?: 'grid' | 'feed';
};

function hasGradeStats(card: ProjectCard | GradedProjectCard): card is GradedProjectCard {
  return 'likeCount' in card;
}

export function ProjectSearchCard({
  card,
  onClick,
  layout = 'grid',
}: ProjectSearchCardProps) {
  const visibleTags = card.tags.slice(0, 3);
  const extraTags = card.tags.length - visibleTags.length;
  const coverHeight = layout === 'feed' ? 'h-44 sm:h-52' : 'h-36';

  return (
    <button
      type="button"
      onClick={onClick}
      className="group flex h-full w-full flex-col overflow-hidden rounded-xl border border-gray-200 bg-white text-left shadow-sm transition-all hover:border-teal-300 hover:shadow-md"
    >
      <div
        className={`relative ${coverHeight} shrink-0 overflow-hidden bg-gradient-to-br from-teal-600 to-teal-500`}
      >
        {card.coverImage ? (
          // eslint-disable-next-line @next/next/no-img-element
          <img
            src={card.coverImage}
            alt=""
            className="h-full w-full object-cover transition-transform duration-300 group-hover:scale-[1.02]"
          />
        ) : null}
        <div className="absolute inset-0 bg-gradient-to-t from-black/45 via-black/10 to-transparent" />
        <div className="absolute right-3 top-3 rounded-full bg-black/30 p-2 text-white backdrop-blur-sm">
          <GemIcon className="h-4 w-4" aria-hidden="true" />
        </div>
        <span className="absolute bottom-3 left-3 rounded-full bg-white/90 px-2.5 py-0.5 text-xs font-semibold uppercase tracking-wide text-teal-800">
          {card.subject}
        </span>
      </div>

      <div className="flex flex-1 flex-col gap-2 p-4">
        <h3 className="line-clamp-1 text-base font-semibold text-gray-900 group-hover:text-teal-700">
          {card.repositoryUrl ? (
            <span className="font-mono text-sm">{card.title}</span>
          ) : (
            card.title
          )}
        </h3>
        {card.repositoryUrl ? (
          <p className="line-clamp-1 text-xs text-teal-700">{card.repositoryUrl}</p>
        ) : null}
        <p className="line-clamp-2 min-h-[2.5rem] text-sm leading-relaxed text-gray-600">
          {card.shortDescription || 'No description yet.'}
        </p>
        <div className="mt-auto flex flex-wrap gap-1.5 pt-1">
          {visibleTags.length > 0 ? (
            visibleTags.map((tag) => (
              <Badge key={tag} variant="tag" className="text-[11px]">
                {tag}
              </Badge>
            ))
          ) : (
            <span className="text-xs text-gray-400">No tags</span>
          )}
          {extraTags > 0 ? (
            <span className="text-xs font-medium text-gray-500">+{extraTags}</span>
          ) : null}
        </div>
        {hasGradeStats(card) ? (
          <div className="flex items-center gap-3 border-t border-gray-100 pt-2 text-xs text-gray-500">
            <span className="inline-flex items-center gap-1">
              <ThumbsUp className="h-3.5 w-3.5" aria-hidden="true" />
              {card.likeCount}
            </span>
            <span className="inline-flex items-center gap-1">
              <Star
                className={`h-3.5 w-3.5 ${
                  card.averageRating !== null
                    ? 'fill-yellow-400 text-yellow-400'
                    : 'text-gray-300'
                }`}
                aria-hidden="true"
              />
              {card.averageRating !== null ? card.averageRating.toFixed(1) : '—'}
            </span>
          </div>
        ) : null}
      </div>
    </button>
  );
}
