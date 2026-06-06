'use client';

import Link from 'next/link';
import { usePathname } from 'next/navigation';
import { Settings, Shield } from 'lucide-react';
import { local } from '@/utilities';

function isSettingsPath(pathname: string | null, href: string): boolean {
  if (!pathname) return false;
  if (href === '/settings') {
    return pathname === '/settings';
  }
  return pathname.startsWith(href);
}

export function SettingsNav() {
  const pathname = usePathname();
  const { t } = local.useLocalization();

  const items = [
    {
      href: '/settings',
      icon: <Settings className="h-4 w-4" />,
      label: t.settingsTitle,
    },
    {
      href: '/settings/privacy',
      icon: <Shield className="h-4 w-4" />,
      label: t.privacyAndData,
    },
  ] as const;

  return (
    <nav
      aria-label={t.settingsSections}
      className="mb-8 flex flex-wrap gap-2 rounded-xl border border-border bg-background p-1 shadow-sm"
    >
      {items.map(({ href, icon, label }) => {
        const active = isSettingsPath(pathname, href);
        return (
          <Link
            key={href}
            href={href}
            aria-current={active ? 'page' : undefined}
            className={`inline-flex flex-1 items-center justify-center gap-2 rounded-lg px-4 py-2.5 text-sm font-medium transition-colors sm:flex-none ${
              active
                ? 'bg-[var(--btn-background)]/12 text-[var(--hvr-background)]'
                : 'text-foreground/70 hover:bg-darker-background hover:text-foreground'
            }`}
          >
            {icon}
            {label}
          </Link>
        );
      })}
    </nav>
  );
}

export function isSettingsNavActive(pathname: string | null, href: string): boolean {
  return isSettingsPath(pathname, href);
}
