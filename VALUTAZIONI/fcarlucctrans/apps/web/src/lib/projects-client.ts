import type { UserProjectGrade } from '@/types/feed';

export async function submitProjectGrade(
  projectId: number,
  score: number,
  favourite: boolean,
): Promise<UserProjectGrade | null> {
  const res = await fetch('/api/projects/grade', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ projectId, score, favourite }),
  });
  if (!res.ok) return null;
  return (await res.json()) as UserProjectGrade;
}

export async function deleteProjectClient(projectId: number): Promise<boolean> {
  const res = await fetch(`/api/projects/${projectId}`, { method: 'DELETE' });
  if (!res.ok) return false;
  const result = (await res.json()) as boolean;
  return Boolean(result);
}
