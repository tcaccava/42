/**
 * Stable image URLs (JPEG via Unsplash CDN — allowed in apps/web/next.config.ts).
 */

function unsplash(photoId, { w, h }) {
  const params = new URLSearchParams({
    w: String(w),
    h: String(h),
    fit: 'crop',
    auto: 'format',
    q: '80',
  });
  return `https://images.unsplash.com/${photoId}?${params}`;
}

function avatarUrl(photoId) {
  return unsplash(photoId, { w: 128, h: 128 });
}

function coverUrl(photoId) {
  return unsplash(photoId, { w: 1200, h: 400 });
}

function projectShotUrl(photoId) {
  return unsplash(photoId, { w: 800, h: 500 });
}

/** Fetch image bytes for S3 upload during project seeding. */
async function fetchImageBuffer(url) {
  const res = await fetch(url);
  if (!res.ok) {
    throw new Error(`Failed to fetch ${url} (${res.status})`);
  }
  const contentType = res.headers.get('content-type') ?? 'image/jpeg';
  return { buffer: Buffer.from(await res.arrayBuffer()), contentType };
}

module.exports = {
  avatarUrl,
  coverUrl,
  projectShotUrl,
  fetchImageBuffer,
};
