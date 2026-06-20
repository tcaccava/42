import { NextResponse } from 'next/server';
import type { NextRequest } from 'next/server';

const API_BASE = process.env.BACKEND_URL || 'http://localhost:8000';

export async function proxy(request: NextRequest) {
  const cookieHeader = request.headers.get('cookie') ?? '';
  let response;
  try {
    response = await fetch(
      `${API_BASE}/api/get_session`,
      { headers: { 'Cookie': cookieHeader } }
    );
  } catch (error) {
    response = null;
    console.error('Error fetching session:', error);
  }

  const { pathname } = request.nextUrl;

  // Utente loggato che tenta di accedere a route guest-only (login/signup)
  if (response && response.ok) {
    if (pathname === '/login' || pathname === '/signup' || pathname === '/') {
      return NextResponse.redirect(new URL('/profile', request.url));
    }
  } else {
    // Utente non loggato che tenta di accedere a route protette
    const isGuestPath = pathname === '/' || pathname === '/login' || pathname === '/signup' || pathname.startsWith('/play/local/') || pathname === '/privacy-policy' || pathname === '/terms-of-service';
    if (!isGuestPath) {
      return NextResponse.redirect(new URL('/login', request.url));
    }
  }

  return NextResponse.next();
}

export const config = {
  matcher: [
    /*
     * Match all request paths except for the ones starting with:
     * - api (API routes)
     * - _next/static (static files)
     * - _next/image (image optimization files)
     * - favicon.ico (favicon file)
     */
    '/((?!api|_next/static|_next/image|favicon.ico).*)',
  ],
};
