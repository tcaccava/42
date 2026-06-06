import type { ReactNode } from 'react';

type PageShellProps = {
  children: ReactNode;
  maxWidth?: '2xl' | '3xl' | '4xl';
  className?: string;
};

const maxWidthClass = {
  '2xl': 'max-w-2xl',
  '3xl': 'max-w-3xl',
  '4xl': 'max-w-4xl',
} as const;

export function PageShell({
  children,
  maxWidth = '3xl',
  className = '',
}: PageShellProps) {
  return (
    <div className={`mx-auto ${maxWidthClass[maxWidth]} px-4 py-8 ${className}`}>
      {children}
    </div>
  );
}

type PageHeaderProps = {
  icon?: ReactNode;
  title: string;
  description?: string;
  className?: string;
};

export function PageHeader({
  icon,
  title,
  description,
  className = '',
}: PageHeaderProps) {
  return (
    <div className={`mb-8 flex items-start gap-3 ${className}`}>
      {icon && (
        <div className="rounded-lg border border-border bg-darker-background p-2 shrink-0">
          {icon}
        </div>
      )}
      <div>
        <h1 className="text-2xl font-bold text-foreground">{title}</h1>
        {description && (
          <p className="mt-1 text-sm text-foreground/70">{description}</p>
        )}
      </div>
    </div>
  );
}

type SectionCardProps = {
  children: ReactNode;
  title?: string;
  description?: string;
  icon?: ReactNode;
  tone?: 'default' | 'danger';
  className?: string;
  contentClassName?: string;
};

export function SectionCard({
  children,
  title,
  description,
  icon,
  tone = 'default',
  className = '',
  contentClassName = '',
}: SectionCardProps) {
  const toneClass =
    tone === 'danger'
      ? 'border-red-300/60 bg-red-50/50 dark:border-red-500/40 dark:bg-red-950/20'
      : 'border-border bg-background';

  return (
    <section
      className={`rounded-2xl border shadow-sm ${toneClass} ${className}`}
    >
      {(title || description) && (
        <div className="border-b border-border/70 px-6 py-4">
          {title && (
            <div className="flex items-center gap-2">
              {icon}
              <h2 className="text-lg font-bold text-foreground">{title}</h2>
            </div>
          )}
          {description && (
            <p className={`text-sm text-foreground/70 ${title ? 'mt-1' : ''}`}>
              {description}
            </p>
          )}
        </div>
      )}
      <div className={`p-6 ${contentClassName}`}>{children}</div>
    </section>
  );
}

type AlertBannerProps = {
  kind: 'success' | 'error' | 'info';
  children: ReactNode;
  className?: string;
};

const alertStyles = {
  success:
    'border-emerald-200 bg-emerald-50 text-emerald-800 dark:border-emerald-800 dark:bg-emerald-950/40 dark:text-emerald-200',
  error:
    'border-red-200 bg-red-50 text-red-800 dark:border-red-800 dark:bg-red-950/40 dark:text-red-200',
  info:
    'border-teal-200 bg-teal-50 text-teal-800 dark:border-teal-800 dark:bg-teal-950/40 dark:text-teal-200',
} as const;

export function AlertBanner({
  kind,
  children,
  className = '',
}: AlertBannerProps) {
  return (
    <div
      role={kind === 'error' ? 'alert' : 'status'}
      className={`rounded-lg border p-4 text-sm ${alertStyles[kind]} ${className}`}
    >
      {children}
    </div>
  );
}

type EmptyStateProps = {
  icon: ReactNode;
  message: ReactNode;
  className?: string;
};

export function EmptyState({ icon, message, className = '' }: EmptyStateProps) {
  return (
    <div
      className={`rounded-2xl border border-border bg-background p-12 text-center ${className}`}
    >
      <div className="mx-auto mb-3 flex justify-center text-foreground/40">{icon}</div>
      <p className="text-foreground/70">{message}</p>
    </div>
  );
}

type TabButtonProps = {
  active: boolean;
  children: ReactNode;
  onClick: () => void;
  className?: string;
};

export function TabButton({
  active,
  children,
  onClick,
  className = '',
}: TabButtonProps) {
  return (
    <button
      type="button"
      onClick={onClick}
      className={`rounded-lg px-4 py-2 font-medium transition-colors ${
        active
          ? 'bg-[var(--btn-background)]/12 text-[var(--hvr-background)]'
          : 'text-foreground/70 hover:bg-darker-background hover:text-foreground'
      } ${className}`}
    >
      {children}
    </button>
  );
}

export function PaginationBar({
  pageNumber,
  pageCount,
  totalCount,
  pageSize,
  onPageChange,
  disabled = false,
}: {
  pageNumber: number;
  pageCount: number;
  totalCount: number;
  pageSize: number;
  onPageChange: (page: number) => void;
  disabled?: boolean;
}) {
  if (pageCount <= 1) {
    return null;
  }

  const safeTotal =
    totalCount > 0 ? totalCount : Math.max(pageCount * pageSize, (pageNumber + 1) * pageSize);
  const from = pageNumber * pageSize + 1;
  const to = Math.min((pageNumber + 1) * pageSize, safeTotal);

  return (
    <nav
      aria-label="Pagination"
      className="flex flex-wrap items-center justify-between gap-3 border-t border-border/70 pt-4"
    >
      <p className="text-sm text-foreground/70">
        {from}–{to} of {safeTotal}
      </p>
      <div className="flex items-center gap-2">
        <button
          type="button"
          disabled={disabled || pageNumber === 0}
          onClick={() => onPageChange(pageNumber - 1)}
          className="rounded-lg border border-border px-3 py-1.5 text-sm font-medium text-foreground transition-colors hover:bg-darker-background disabled:cursor-not-allowed disabled:opacity-40"
        >
          Previous
        </button>
        <span className="min-w-[6rem] text-center text-sm text-foreground/70">
          Page {pageNumber + 1} / {pageCount}
        </span>
        <button
          type="button"
          disabled={disabled || pageNumber >= pageCount - 1}
          onClick={() => onPageChange(pageNumber + 1)}
          className="rounded-lg border border-border px-3 py-1.5 text-sm font-medium text-foreground transition-colors hover:bg-darker-background disabled:cursor-not-allowed disabled:opacity-40"
        >
          Next
        </button>
      </div>
    </nav>
  );
}

export const navLinkClass = (active: boolean) =>
  active
    ? 'bg-[var(--btn-background)]/12 text-[var(--hvr-background)]'
    : 'text-foreground/70 hover:bg-darker-background hover:text-foreground';
