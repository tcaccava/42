import { ReactNode } from 'react';

type CardProps = {
  children: ReactNode;
  className?: string;
  tone?: 'default' | 'danger';
};

export function Card({ children, className = '', tone = 'default' }: CardProps) {
  const toneClass =
    tone === 'danger'
      ? 'border-red-200 bg-red-50/40'
      : 'border-gray-200 bg-white';

  return (
    <div
      className={`rounded-2xl border shadow-sm ${toneClass} ${className}`}
    >
      {children}
    </div>
  );
}

export function CardHeader({ children, className = '' }: CardProps) {
  return (
    <div className={`border-b border-gray-100 px-6 py-4 ${className}`}>
      {children}
    </div>
  );
}

export function CardContent({ children, className = '' }: CardProps) {
  return <div className={`p-6 ${className}`}>{children}</div>;
}
