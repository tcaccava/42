'use client';

import { CommentData } from '@/types/comment';
import { useState } from 'react';
import { Button } from './Button';
import {
  fetchProjectComments,
  postProjectComment,
  upsertCommentUser,
} from '@/lib/comments-client';
import { User } from '@/types';
import { formatAppDateTime } from '@/lib/format-date';
import { local } from '../utilities';
import { AlertBanner } from '@/components/ui/layout';

export default function Comments(params: {
  comments: CommentData[];
  projectId: number;
  userData: User;
}) {
  const { locale, t } = local.useLocalization();
  const projectId = params.projectId;
  const userData = params.userData;
  const [commentList, setCommentList] = useState(params.comments);
  const [commentText, setCommentText] = useState('');
  const [submitting, setSubmitting] = useState(false);
  const [error, setError] = useState<string | null>(null);

  async function refreshComments() {
    try {
      const comments = await fetchProjectComments(projectId);
      setCommentList(comments);
    } catch {
      setError('Could not refresh comments.');
    }
  }

  async function handleSubmit() {
    const text = commentText.trim();
    if (text.length === 0 || submitting) return;

    setSubmitting(true);
    setError(null);

    const displayName =
      `${userData.first_name ?? ''} ${userData.last_name ?? ''}`.trim() ||
      userData.login ||
      'User';

    try {
      await upsertCommentUser(userData.login!, displayName);
      await postProjectComment(projectId, text);
      await refreshComments();
      setCommentText('');
    } catch {
      setError('Could not save your comment. Please try again.');
    } finally {
      setSubmitting(false);
    }
  }

  return (
    <div className="space-y-3">
      {error ? <AlertBanner kind="error">{error}</AlertBanner> : null}

      {commentList.length > 0 ? (
        <ul className="divide-y divide-gray-100">
          {commentList.map((comment) => (
            <li key={comment.id} className="py-2.5 first:pt-0">
              <div className="flex flex-wrap items-baseline gap-x-2 gap-y-0.5">
                <span className="text-sm font-semibold text-gray-900">
                  {comment.user.name}
                </span>
                <time
                  dateTime={comment.createdAt}
                  className="text-xs text-gray-500"
                >
                  {formatAppDateTime(comment.createdAt, locale)}
                </time>
              </div>
              <p className="mt-0.5 text-sm leading-snug text-gray-700">
                {comment.commentText}
              </p>
            </li>
          ))}
        </ul>
      ) : (
        <p className="text-sm text-gray-500">{t.noCommentsYet}</p>
      )}

      <div className="rounded-lg border border-gray-200 bg-gray-50 p-2">
        <textarea
          rows={2}
          className="w-full resize-none bg-transparent px-2 py-1.5 text-sm text-gray-900 outline-none placeholder:text-gray-400"
          placeholder={t.writeComment}
          value={commentText}
          disabled={submitting}
          onChange={(e) => setCommentText(e.target.value)}
          onKeyDown={(e) => {
            if (e.key === 'Enter' && (e.metaKey || e.ctrlKey)) {
              e.preventDefault();
              void handleSubmit();
            }
          }}
        />
        <div className="flex justify-end gap-1.5 pt-1">
          {commentText.length > 0 && (
            <Button
              size="sm"
              variant="ghost"
              disabled={submitting}
              onClick={() => setCommentText('')}
            >
              {t.cancel}
            </Button>
          )}
          <Button
            size="sm"
            onClick={() => void handleSubmit()}
            disabled={!commentText.trim() || submitting}
          >
            {submitting ? '…' : t.submit}
          </Button>
        </div>
      </div>
    </div>
  );
}
