import Link from 'next/link';
import type { ReactNode } from 'react';

type LinkButtonProps = {
  href: string;
  children: ReactNode;
  variant?: 'primary' | 'secondary';
  className?: string;
};

const variantClass = {
  primary:
    'bg-[var(--btn-background)] text-white hover:bg-[var(--hvr-background)] focus:ring-[var(--color-hvr-background)]',
  secondary:
    'border-2 border-[var(--color-border)] bg-[var(--color-background)] text-[var(--color-foreground)] hover:bg-[var(--darker-background)] focus:ring-[var(--color-hvr-background)]',
} as const;

export function LinkButton({
  href,
  children,
  variant = 'primary',
  className = '',
}: LinkButtonProps) {
  return (
    <Link
      href={href}
      className={`inline-flex items-center justify-center rounded-lg px-4 py-2 text-sm font-medium transition-colors focus:outline-none focus:ring-2 focus:ring-offset-2 ${variantClass[variant]} ${className}`}
    >
      {children}
    </Link>
  );
}
