import type { CommentData } from '@/types/comment';

export async function fetchProjectComments(
  projectId: number,
): Promise<CommentData[]> {
  const res = await fetch(`/api/comments?projectId=${projectId}`, {
    cache: 'no-store',
  });
  if (!res.ok) {
    throw new Error(`Failed to load comments (${res.status})`);
  }
  return (await res.json()) as CommentData[];
}

export async function upsertCommentUser(
  login: string,
  name: string,
): Promise<void> {
  const res = await fetch('/api/comments/user', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ id: login, name }),
  });
  if (!res.ok) {
    throw new Error(`Failed to register comment user (${res.status})`);
  }
}

export async function postProjectComment(
  projectId: number,
  text: string,
): Promise<CommentData> {
  const res = await fetch('/api/comments', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ projectId, text }),
  });
  if (!res.ok) {
    throw new Error(`Failed to post comment (${res.status})`);
  }
  return (await res.json()) as CommentData;
}
