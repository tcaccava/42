'use client';

import { Home, Search as SearchIcon, Calendar, User, PlusSquare, LogOut, Settings, Users } from 'lucide-react';
import { usePathname } from 'next/navigation';
import Link from 'next/link';
import { useState } from 'react';
import { local } from '@/utilities';
import { navLinkClass } from '@/components/ui/layout';

export function Sidebar() {
  const pathname = usePathname();
  const [isLoggingOut, setIsLoggingOut] = useState(false);
  const t = local.useLocalization().t;

  const isActive = (path: string) => {
    if (path === '/feed' && pathname === '/') return true;
    if (path === '/settings') {
      return pathname?.startsWith('/settings') ?? false;
    }
    return pathname?.startsWith(path);
  };

  const handleLogout = () => {
    setIsLoggingOut(true);
    window.location.href = '/api/logout';
  };

  const navItems = [
    { href: '/feed', icon: <Home className="w-5 h-5" />, label: t.feed },
    { href: '/search', icon: <SearchIcon className="w-5 h-5" />, label: t.search },
    { href: '/connections', icon: <Users className="w-5 h-5" />, label: t.connections },
    { href: '/create', icon: <PlusSquare className="w-5 h-5" />, label: t.create },
    { href: '/events', icon: <Calendar className="w-5 h-5" />, label: t.events },
    { href: '/profile', icon: <User className="w-5 h-5" />, label: t.profile },
    { href: '/settings', icon: <Settings className="w-5 h-5" />, label: t.settings },
  ];

  return (
    <nav className="flex flex-col gap-1 py-4">
      {navItems.map(({ href, icon, label }) => (
        <Link
          key={href}
          href={href}
          aria-current={isActive(href) ? 'page' : undefined}
          className={`flex items-center gap-3 px-4 py-3 rounded-xl font-medium text-sm transition-colors ${
            navLinkClass(isActive(href))
          }`}
        >
          {icon}
          <span>{label}</span>
        </Link>
      ))}

      <div className="mt-6 pt-4 border-t border-border">
        <button
          onClick={handleLogout}
          disabled={isLoggingOut}
          aria-label="Logout"
          className="flex w-full items-center gap-3 rounded-xl px-4 py-3 text-sm font-medium text-foreground/80 transition-colors hover:bg-red-500/10 hover:text-red-600 dark:hover:text-red-400 disabled:cursor-not-allowed disabled:opacity-50"
        >
          <LogOut className="w-5 h-5" />
          <span>{isLoggingOut ? t.loggingOut : t.logout}</span>
        </button>
      </div>
    </nav>
  );
}
