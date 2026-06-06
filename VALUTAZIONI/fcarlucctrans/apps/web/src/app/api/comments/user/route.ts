import { NextRequest, NextResponse } from 'next/server';
import { fetchWithAuth } from '@/server/http';

export async function POST(req: NextRequest) {
  try {
    const body = (await req.json()) as { id?: string; name?: string };
    if (typeof body.id !== 'string' || typeof body.name !== 'string') {
      return NextResponse.json({ message: 'Invalid request body' }, { status: 400 });
    }

    const response = await fetchWithAuth('/comments/secure/user', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        id: body.id,
        name: body.name,
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
    console.error('Comment user POST proxy failed:', error);
    return NextResponse.json({ message: 'Failed to upsert comment user' }, { status: 500 });
  }
}
