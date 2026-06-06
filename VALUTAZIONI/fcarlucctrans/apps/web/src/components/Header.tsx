'use client';

import { Home, Search as SearchIcon, Calendar, PlusSquare, Users, Shield } from 'lucide-react';
import Image from 'next/image';
import { usePathname } from 'next/navigation';
import Link from 'next/link';
import { local } from '@/utilities';
import { navLinkClass } from '@/components/ui/layout';
import { isSettingsNavActive } from '@/components/SettingsNav';
import type { User } from '@/types';

type HeaderProps = {
  user?: User | null;
};

function ProfileAvatar({ user, className = '' }: { user: User; className?: string }) {
  const isActive = usePathname()?.startsWith('/profile');

  return (
    <Link
      href="/profile"
      className={`rounded-full ring-2 ring-transparent transition-colors hover:ring-teal-200 focus-visible:outline-none focus-visible:ring-teal-500 ${
        isActive ? 'ring-teal-400' : ''
      } ${className}`}
      aria-label="Your profile"
      aria-current={isActive ? 'page' : undefined}
    >
      <Image
        src={user.image || '/globe.svg'}
        alt={user.login}
        width={36}
        height={36}
        className="h-9 w-9 rounded-full object-cover bg-darker-background"
      />
    </Link>
  );
}

export function Header({ user }: HeaderProps) {
  const pathname = usePathname();
  const t = local.useLocalization().t;

  const isActive = (path: string) => {
    if (path === '/feed' && pathname === '/') return true;
    if (path === '/settings/privacy') {
      return isSettingsNavActive(pathname, path);
    }
    return pathname?.startsWith(path);
  };

  const linkClass = (path: string) =>
    `p-2 rounded-lg transition-colors ${navLinkClass(isActive(path))}`;

  return (
    <header className="bg-background border-b border-border sticky top-0 z-50">
      <a href="#main-content" className="skip-link">
        Skip to main content
      </a>
      <div className="max-w-screen-xl mx-auto px-4 py-3 flex items-center justify-between">
        <div className="flex flex-col sm:flex-row items-center gap-1 sm:gap-3">
          <Image
            src="/42logo.png"
            alt="42 Logo"
            width={40}
            height={40}
            className="w-6 h-6 sm:w-10 sm:h-10 object-contain"
          />
          <h1 className="text-sm sm:text-2xl md:text-3xl font-black tracking-tight">
            <span className="text-[#05BBBB]">{'<'}</span>
            <span className="text-foreground">Share</span>
            <span className="text-green-500">{'/'}</span>
            <span className="text-[#05BBBB]">{'>'}</span>
          </h1>
        </div>

        <nav className="flex lg:hidden items-center gap-2 sm:gap-3">
          <Link href="/feed" className={linkClass('/feed')} aria-label={t.feed} aria-current={isActive('/feed') ? 'page' : undefined}>
            <Home className="w-6 h-6" />
          </Link>
          <Link href="/search" className={linkClass('/search')} aria-label={t.search} aria-current={isActive('/search') ? 'page' : undefined}>
            <SearchIcon className="w-6 h-6" />
          </Link>
          <Link href="/connections" className={linkClass('/connections')} aria-label={t.connections}>
            <Users className="w-6 h-6" />
          </Link>
          <Link href="/create" className={linkClass('/create')} aria-label={t.createPost} aria-current={isActive('/create') ? 'page' : undefined}>
            <PlusSquare className="w-6 h-6" />
          </Link>
          <Link href="/events" className={linkClass('/events')} aria-label={t.events} aria-current={isActive('/events') ? 'page' : undefined}>
            <Calendar className="w-6 h-6" />
          </Link>
          <Link href="/settings/privacy" className={linkClass('/settings/privacy')} aria-label={t.privacyAndData}>
            <Shield className="w-6 h-6" />
          </Link>
          {user && (
            <div className="ml-1 border-l border-border pl-2">
              <ProfileAvatar user={user} />
            </div>
          )}
        </nav>

        <div className="hidden lg:flex items-center gap-2">
          {user && <ProfileAvatar user={user} />}
        </div>
      </div>
    </header>
  );
}
