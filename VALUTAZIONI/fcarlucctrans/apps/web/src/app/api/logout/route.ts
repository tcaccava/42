import { NextResponse, NextRequest } from 'next/server';
import { getGatewayLogoutUrl } from '@/lib/logout-url';

export async function POST(req: NextRequest) {
  const target = getGatewayLogoutUrl();

  try {
    const cookieHeader = req.headers.get('cookie') || '';

    await fetch(target, {
      method: 'GET',
      headers: {
        ...(cookieHeader ? { cookie: cookieHeader } : {}),
      },
    });
  } catch (err) {
    // ignore errors from gateway — we'll still clear cookies locally
    // eslint-disable-next-line no-console
    console.warn('Gateway logout proxy failed:', err);
  }

  const res = NextResponse.json({ status: 'ok' });
  res.cookies.set('access_token', '', { maxAge: 0, path: '/' });
  res.cookies.set('refresh_token', '', { maxAge: 0, path: '/' });

  return res;
}

export async function GET(req: NextRequest) {
  const target = getGatewayLogoutUrl();

  try {
    const cookieHeader = req.headers.get('cookie') || '';
    await fetch(target, {
      method: 'GET',
      headers: {
        ...(cookieHeader ? { cookie: cookieHeader } : {}),
      },
    });
  } catch (err) {
    // ignore
    // eslint-disable-next-line no-console
    console.warn('Gateway logout proxy failed (GET):', err);
  }

  const res = NextResponse.redirect(new URL('/', process.env.WEBAPP_URL || 'http://localhost:3000'));
  res.cookies.set('access_token', '', { maxAge: 0, path: '/' });
  res.cookies.set('refresh_token', '', { maxAge: 0, path: '/' });
  return res;
}
