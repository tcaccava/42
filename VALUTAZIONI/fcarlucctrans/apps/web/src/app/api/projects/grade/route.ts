import { NextRequest, NextResponse } from 'next/server';
import { fetchWithAuth } from '@/server/http';

export async function POST(req: NextRequest) {
  try {
    const body = (await req.json()) as {
      projectId?: number;
      score?: number;
      favourite?: boolean;
    };

    if (
      typeof body.projectId !== 'number' ||
      typeof body.score !== 'number' ||
      typeof body.favourite !== 'boolean'
    ) {
      return NextResponse.json({ message: 'Invalid request body' }, { status: 400 });
    }

    const response = await fetchWithAuth('/projects/secure/grade', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        projectId: body.projectId,
        score: body.score,
        favourite: body.favourite,
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
    console.error('Project grade POST proxy failed:', error);
    return NextResponse.json({ message: 'Failed to save grade' }, { status: 500 });
  }
}
