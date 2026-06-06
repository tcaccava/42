import axios from 'axios';
import { cookies } from 'next/headers';
import { NextRequest } from 'next/server';
import { getGatewayUrl, joinApiUrl } from '@/server/api-url';

export async function getCookieHeader(): Promise<string> {
  const cookieStore = await cookies();
  return cookieStore
    .getAll()
    .map((c) => `${c.name}=${c.value}`)
    .join('; ');
}

export async function PUT(req: NextRequest) {
  const cookieHeader = await getCookieHeader();
  const query = req.nextUrl.searchParams;
  const projectId = query.get('id');
  if (!projectId) {
    return Response.json({ error: 'Missing project id' }, { status: 400 });
  }

  try {
    const contentType = req.headers.get('content-type');
    const body = await req.arrayBuffer();
    const response = await axios.put(
      joinApiUrl(getGatewayUrl(), `/projects/secure/update/screenshots?id=${projectId}`),
      body,
      {
        headers: {
          cookie: cookieHeader,
          ...(contentType ? { 'content-type': contentType } : {}),
        },
        maxBodyLength: Infinity,
        maxContentLength: Infinity,
      },
    );
    return Response.json(response.data);
  } catch (error) {
    if (axios.isAxiosError(error)) {
      const status = error.response?.status ?? 502;
      const data = error.response?.data ?? { error: error.message };
      console.error('Screenshot upload proxy failed:', status, data);
      return Response.json(data, { status });
    }
    console.error('Screenshot upload proxy failed:', error);
    return Response.json({ error: 'Upload failed' }, { status: 500 });
  }
}
