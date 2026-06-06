import { NextRequest, NextResponse } from 'next/server';
import { fetchWithAuth } from '@/server/http';

type RouteContext = { params: Promise<{ id: string }> };

export async function DELETE(_req: NextRequest, context: RouteContext) {
  try {
    const { id } = await context.params;
    const projectId = Number.parseInt(id, 10);
    if (Number.isNaN(projectId)) {
      return NextResponse.json({ message: 'Invalid project id' }, { status: 400 });
    }

    const response = await fetchWithAuth(
      `/projects/secure/byId?id=${projectId}`,
      { method: 'DELETE' },
    );

    const payload = await response.text();
    return new NextResponse(payload, {
      status: response.status,
      headers: {
        'Content-Type': response.headers.get('content-type') ?? 'application/json',
      },
    });
  } catch (error) {
    console.error('Project DELETE proxy failed:', error);
    return NextResponse.json({ message: 'Failed to delete project' }, { status: 500 });
  }
}
