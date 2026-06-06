import { NextRequest, NextResponse } from 'next/server';
import { fetchWithAuth } from '@/server/http';
import { getApiBaseUrl, joinApiUrl } from '@/server/api-url';

export async function POST(req: NextRequest) {
  try {
    const body = (await req.json()) as { projectId?: number; text?: string };
    if (typeof body.projectId !== 'number' || typeof body.text !== 'string') {
      return NextResponse.json({ message: 'Invalid request body' }, { status: 400 });
    }

    const response = await fetchWithAuth('/comments/secure/comment', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        projectId: body.projectId,
        text: body.text,
      }),
    });

    const payload = await response.text();
    return new NextResponse(payload, {
      status: response.status,
      headers: {
        'Content-Type': response.headers.get('content-type') ?? 'application/json',
      },
    });
  } catch (error) {
    console.error('Comment POST proxy failed:', error);
    return NextResponse.json({ message: 'Failed to post comment' }, { status: 500 });
  }
}

export async function GET(req: NextRequest) {
  try {
    const projectId = req.nextUrl.searchParams.get('projectId');
    if (!projectId) {
      return NextResponse.json({ message: 'projectId is required' }, { status: 400 });
    }

    const base = await getApiBaseUrl();
    const response = await fetch(
      joinApiUrl(base, `/comments/project?projectId=${encodeURIComponent(projectId)}`),
      { cache: 'no-store' },
    );

    const payload = await response.text();
    return new NextResponse(payload, {
      status: response.status,
      headers: {
        'Content-Type': response.headers.get('content-type') ?? 'application/json',
      },
    });
  } catch (error) {
    console.error('Comment GET proxy failed:', error);
    return NextResponse.json({ message: 'Failed to load comments' }, { status: 500 });
  }
}
