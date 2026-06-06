'use client';

import { Post, User } from '@/types';
import { ThumbsUp, MessageCircle, ArrowUpRight, Star } from 'lucide-react';
import { useEffect, useRef, useState } from 'react';
import { local } from '@/utilities';


interface PostCardProps {
  post: Post;
  currentUser?: User;
  onLike?: (postId: string, liked: boolean) => void;
  onAddReaction: (postId: string, commentId: string, reaction: 'like' | 'heart' | 'fire' | 'clap') => void;
  onRate?: (postId: string, rating: number) => void;
  onAddComment?: (postId: string, text: string) => void;
  onShare?: (postId: string, comment?: string) => void;
}

export default function PostCard({
  post,
  currentUser,
  onLike,
  onAddReaction,
  onRate,
  onAddComment,
  onShare,
}: PostCardProps) {
  const t = local.useLocalization().t;
  const [showComments, setShowComments] = useState(false);
  const [commentText, setCommentText] = useState('');
  const [liked, setLiked] = useState(false);
  const [rating, setRating] = useState<number | null>(typeof post.rating === 'number' ? post.rating : null);
  const [localComments, setLocalComments] = useState(post.comments || []);
  const [showShareModal, setShowShareModal] = useState(false);
  const [shareText, setShareText] = useState('');
  const shareButtonRef = useRef<HTMLButtonElement>(null);

  const handleLike = () => {
    const newLiked = !liked;
    setLiked(newLiked);
    if (typeof onLike === 'function') onLike(post.id, newLiked);
  };

  const handleRate = (val: number) => {
    setRating(val);
    if (typeof onRate === 'function') onRate(post.id, val);
  };

  const handleComment = () => {
    const txt = commentText.trim();
    if (!txt) return;

    // Optimistic local update
    const newComment = {
      id: Date.now().toString(),
      login: currentUser?.login ?? 'user',
      image: currentUser?.image ?? '/globe.svg',
      campus: currentUser?.campus ?? '',
      text: txt,
      timestamp: 'Just now',
      reactions: [],
    };
    setLocalComments([...localComments, newComment]);
    setCommentText('');

    if (typeof onAddComment === 'function') {
      try {
        onAddComment(post.id, txt);
      } catch (err) {
        // ignore — optimistic update already applied
      }
    }
  };

  //{SARA} Use effect to handle Escape key for closing share modal for better accessibility (focus trap):
  useEffect(() => {
    if (!showShareModal) return;

    const handleEscape = (e: KeyboardEvent) => {
      if (e.key === 'Escape') {
        setShowShareModal(false);
      }
    };

    document.addEventListener('keydown', handleEscape);
    return () => document.removeEventListener('keydown', handleEscape);
  }, [showShareModal]);

  // In modal close handler:
  useEffect(() => {
    if (!showShareModal && shareButtonRef.current) {
      shareButtonRef.current.focus();
    }
  }, [showShareModal]);

  return (
    <div className="bg-[var(--color-background)] border-2 border-[var(--color-border)] rounded-lg mb-4">
      {/* Header */}
      <div className="flex items-center gap-3 p-4">
        <img
          src={post.image}
          alt={`${post.login}'s profile picture`}
          className="w-10 h-10 rounded-full object-cover"
        />
        <div className="flex-1">
          <div className="flex items-center gap-2">
            <span className="font-semibold text-[var(--color-foreground)]">{post.login}</span>
            <span className="px-2 py-0.5 bg-blue-200 dark:bg-blue-900 text-blue-900 dark:text-blue-200 text-[length:calc(0.75rem*var(--font-size-multiplier))] rounded-full font-medium border border-blue-400 dark:border-blue-700">
              {post.campus}
            </span>
          </div>
          <p className="text-[length:calc(0.75rem*var(--font-size-multiplier))] text-[var(--color-foreground)] opacity-60">{post.timestamp}</p>
        </div>
      </div>

      {/* Project Screenshot */}
      <div className="relative aspect-video bg-[var(--darker-background)]">
        <img
          src={post.screenshot}
          alt={`Screenshot of ${post.projectName} project by ${post.login}`}
          className="w-full h-full object-cover"
        />
      </div>

      {/* Actions */}
      <div className="p-4">
        <div className="flex items-center gap-4 mb-3 [&>button]:transition-colors [&>button]:duration-100 html.reduce-motion [&>button]:transition-none">
          <button
            onClick={handleLike}
            className="flex items-center gap-2 hover:opacity-70 focus:outline-2 focus:outline-offset-1 focus:outline-green-600"
            aria-pressed={liked}
            aria-label={liked ? t.unLike : t.like}
          >
            <ThumbsUp
              className={`w-6 h-6 ${liked ? 'text-green-600' : 'text-[var(--color-foreground)]'}`}
              fill={liked ? 'currentColor' : 'none'}
              stroke={liked ? 'none' : 'currentColor'}
            />
          </button>
          <button
            onClick={() => setShowComments(!showComments)}
            className="w-10 h-10 flex items-center justify-center rounded-md hover:opacity-70 focus:outline-2 focus:outline-offset-1 focus:outline-green-600 transition-colors duration-100 html.reduce-motion:transition-none"
            aria-label={showComments ? 'Hide comments' : `Show ${localComments.length} comments`}
            aria-expanded={showComments}
          >
            <MessageCircle className="w-6 h-6 text-green-700 dark:text-green-300" />
          </button>
          <button
            ref={shareButtonRef}
            onClick={() => setShowShareModal(true)}
            className="ml-auto bg-green-600 hover:bg-green-700 text-white px-3 py-1 rounded-md transition-colors duration-100 html.reduce-motion:transition-none flex items-center gap-2 focus:outline-2 focus:outline-offset-1 focus:outline-green-700 text-[length:calc(0.875rem*var(--font-size-multiplier))] font-medium"
            aria-label="Share this post"
          >
            <ArrowUpRight className="w-5 h-5 text-white" strokeWidth={2.2} strokeLinecap="round" strokeLinejoin="round" />
            <span className="text-sm font-medium">{t.share}</span>
          </button>
        </div>

        {/* Rating (0-5 stars) */}
        <div className="flex items-center gap-1 mb-3 [&>button]:transition-colors [&>button]:duration-100 html.reduce-motion [&>button]:transition-none" role="group" aria-label="Post rating">
          {[1,2,3,4,5].map((i) => (
            <button
              key={i}
              onClick={() => handleRate(i)}
              className="p-1 focus:outline-2 focus:outline-offset-1 focus:outline-yellow-500"
              aria-label={`Rate ${i} star${i>1?'s':''}`}
              aria-pressed={rating === i}
            >
              <Star
                className={`w-5 h-5 ${(rating !== null && i <= rating) ? 'fill-yellow-400 text-yellow-400' : 'text-[var(--color-border)]'}`}
              />
            </button>
          ))}
          <span className="text-sm text-gray-600 ml-2">{rating !== null ? `${rating}/5` : t.noRating}</span>
        </div>

        {/* Likes */}
        <div className="mb-2">
          <span className="font-semibold text-gray-900">{post.likes} {t.likes}</span>
        </div>

        {/* Description */}
        <div className="mb-2 text-[length:calc(0.875rem*var(--font-size-multiplier))]">
          <span className="font-semibold text-[var(--color-foreground)] mr-2">{post.login}</span>
          <span className="text-[var(--color-foreground)]">{post.description}</span>
        </div>

        {/* Project Name */}
        <div className="mb-2">
          <span className="text-blue-600 dark:text-blue-300 font-medium text-[length:calc(0.875rem*var(--font-size-multiplier))]">#{post.projectName}</span>
        </div>

        {/* Tags */}
        {post.tags.length > 0 && (
          <div className="flex flex-wrap gap-2 mb-2" role="list" aria-label="Post tags">
            {post.tags.map((tag, index) => (
              <span key={index} className="text-blue-600 dark:text-blue-300 text-[length:calc(0.75rem*var(--font-size-multiplier))]" role="listitem">
                #{tag}
              </span>
            ))}
          </div>
        )}

        {/* Comments Preview */}
        {post.comments.length > 0 && !showComments && (
          <button
            onClick={() => setShowComments(true)}
            className="text-[var(--color-foreground)] opacity-60 text-[length:calc(0.875rem*var(--font-size-multiplier))] mb-2 hover:opacity-100 focus:outline-2 focus:outline-offset-1 focus:outline-[var(--color-hvr-background)]"
            aria-label={`View all ${post.comments.length} comments`}
          >
            View all {post.comments.length} comments
          </button>
        )}

        {/* Comments Section */}
        {showComments && (
          <div className="mt-3 space-y-3" role="region" aria-label="Comments">
            {localComments.map((comment) => (
              <div key={comment.id} className="flex gap-3">
                <img
                  src={comment.image}
                  alt={`${comment.login}'s profile picture`}
                  className="w-8 h-8 rounded-full object-cover flex-shrink-0"
                />
                <div className="flex-1">
                  <div className="bg-[var(--darker-background)] rounded-md px-4 py-2 border-2 border-green-500">
                    <div className="flex items-center gap-2 flex-wrap">
                      <span className="font-semibold text-[var(--color-foreground)] text-[length:calc(0.875rem*var(--font-size-multiplier))]">
                        {comment.login}
                      </span>
                      <span className="px-1.5 py-0.5 bg-blue-200 dark:bg-blue-900 text-blue-900 dark:text-blue-200 text-[length:calc(0.75rem*var(--font-size-multiplier))] rounded-full border border-blue-400 dark:border-blue-700">
                        {comment.campus}
                      </span>
                    </div>
                    <p className="text-[var(--color-foreground)] text-[length:calc(0.875rem*var(--font-size-multiplier))]">{comment.text}</p>
                  </div>
                  <div className="flex items-center gap-4 mt-1 px-4">
                    <span className="text-[length:calc(0.75rem*var(--font-size-multiplier))] text-[var(--color-foreground)] opacity-60">{comment.timestamp}</span>
                    <button
                      onClick={() => onAddReaction(post.id, comment.id, 'like')}
                      className="text-[length:calc(0.75rem*var(--font-size-multiplier))] text-[var(--color-foreground)] opacity-60 font-semibold hover:opacity-100 focus:outline-2 focus:outline-offset-1 focus:outline-[var(--color-hvr-background)] transition-opacity duration-100 html.reduce-motion:transition-none"
                      aria-label="Like this comment"
                    >
                      Like
                    </button>
                  </div>
                </div>
              </div>
            ))}
          </div>
        )}

        {/* Add Comment */}
        <div className="flex items-center gap-2 mt-3 pt-3 border-t-2 border-[var(--color-border)]">
          <input
            type="text"
            value={commentText}
            onChange={(e) => setCommentText(e.target.value)}
            onKeyPress={(e) => e.key === 'Enter' && handleComment()}
            placeholder={t.addComment}
            className="flex-1 bg-transparent text-gray-900 placeholder-gray-500 outline-none text-sm"
          />
          {commentText.trim() && (
            <button
              onClick={handleComment}
              className="text-green-600 dark:text-green-400 font-semibold text-[length:calc(0.875rem*var(--font-size-multiplier))] hover:opacity-70 focus:outline-2 focus:outline-offset-1 focus:outline-green-600 transition-opacity duration-100 html.reduce-motion:transition-none"
              aria-label="Post this comment"
            >
              {t.post}
            </button>
          )}
        </div>
      </div>

      {/* Share Modal */}
      {showShareModal && (
        <div className="fixed inset-0 z-50 flex items-center justify-center">
          <div className="absolute inset-0 bg-black opacity-50" onClick={() => setShowShareModal(false)} />
          <div className="bg-white rounded-lg shadow-lg z-10 w-full max-w-md p-4">
            <h3 className="text-lg font-bold mb-2">{t.sharePost}</h3>
            <div className="border rounded p-3 mb-3">
              <div className="flex items-start gap-3">
                <img
                  src={post.image}
                  alt={`${post.login}'s profile picture`}
                  className="w-10 h-10 rounded-full object-cover flex-shrink-0"
                />
                <div className="flex-1">
                  <div className="font-semibold text-[var(--color-foreground)] text-[length:calc(0.875rem*var(--font-size-multiplier))]">{post.login}</div>
                  <div className="text-[length:calc(0.875rem*var(--font-size-multiplier))] text-[var(--color-foreground)] opacity-60">#{post.projectName}</div>
                  <p className="text-[length:calc(0.875rem*var(--font-size-multiplier))] text-[var(--color-foreground)] mt-2 line-clamp-3">{post.description}</p>
                </div>
              </div>
              {post.screenshot && (
                <div className="mt-3">
                  <img
                    src={post.screenshot}
                    alt={`${post.projectName} screenshot`}
                    className="w-full h-40 object-cover rounded"
                  />
                </div>
              )}
            </div>

            <textarea
              value={shareText}
              onChange={(e) => setShareText(e.target.value)}
              placeholder={t.shareAddComment}
              className="w-full border rounded p-2 mb-3 text-sm h-24 resize-none"
            />

            <div className="flex justify-end gap-2">
              <button onClick={() => { setShowShareModal(false); setShareText(''); }} className="px-3 py-2 rounded bg-gray-100">{t.cancel}</button>
              <button
                onClick={() => {
                  // optimistic close + callback
                  if (typeof onShare === 'function') onShare(post.id, shareText.trim());
                  setShowShareModal(false);
                  setShareText('');
                }}
                className="px-3 py-2 rounded bg-green-600 hover:bg-green-700 text-white focus:outline-2 focus:outline-offset-1 focus:outline-green-700 text-[length:calc(0.875rem*var(--font-size-multiplier))] font-medium"
                aria-label="Share this post"
              >
                {t.share}
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
