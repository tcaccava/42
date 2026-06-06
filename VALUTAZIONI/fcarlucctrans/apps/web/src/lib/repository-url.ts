export type ParsedRepositoryUrl = {
  normalized: string;
  slug: string;
};

/** Client-side mirror of project-service repository URL validation. */
export function parsePublicRepositoryUrl(input: string): ParsedRepositoryUrl | null {
  const trimmed = input.trim();
  if (!trimmed) return null;

  let url: URL;
  try {
    url = new URL(trimmed);
  } catch {
    return null;
  }

  if (url.protocol !== 'https:') return null;

  const host = url.hostname.toLowerCase();
  const parts = url.pathname
    .replace(/\/+$/, '')
    .replace(/\.git$/i, '')
    .split('/')
    .filter(Boolean);

  if (parts.length < 2) return null;

  if (host === 'github.com') {
    if (parts.length !== 2) return null;
    const [owner, repo] = parts;
    if (!/^[A-Za-z0-9._-]+$/.test(owner) || !/^[A-Za-z0-9._-]+$/.test(repo)) {
      return null;
    }
    return {
      normalized: `https://github.com/${owner}/${repo}`,
      slug: `${owner}/${repo}`,
    };
  }

  if (host === 'gitlab.com') {
    const repo = parts[parts.length - 1];
    const owner = parts.slice(0, -1).join('/');
    if (!owner || !/^[A-Za-z0-9._-]+$/.test(repo)) return null;
    return {
      normalized: `https://gitlab.com/${owner}/${repo}`,
      slug: `${owner}/${repo}`,
    };
  }

  return null;
}
