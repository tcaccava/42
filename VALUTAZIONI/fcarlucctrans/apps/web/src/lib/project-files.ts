import type { ScreenshotFileData } from '@/types/project';

export async function urlToScreenshotFile(
  url: string,
  index: number,
): Promise<ScreenshotFileData> {
  const response = await fetch(url);
  const blob = await response.blob();
  const extension = blob.type.split('/')[1] || 'png';
  const name = `screenshot-${index + 1}.${extension}`;
  const data = new File([blob], name, { type: blob.type || 'image/png' });
  return { data, name };
}

export function archiveLabelFromUrl(url: string | null | undefined): string | null {
  if (!url) return null;
  try {
    const pathname = new URL(url).pathname;
    const segment = pathname.split('/').pop();
    return segment ? decodeURIComponent(segment) : 'Source archive';
  } catch {
    return 'Source archive';
  }
}
