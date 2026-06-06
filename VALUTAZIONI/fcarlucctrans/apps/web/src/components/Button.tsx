import { ReactNode } from 'react';

type ButtonVariant = 'primary' | 'secondary' | 'ghost' | 'danger';
type ButtonSize = 'sm' | 'md' | 'lg';

type ButtonProps = {
  children: ReactNode;
  variant?: ButtonVariant;
  size?: ButtonSize;
  onClick?: () => void;
  type?: 'button' | 'submit';
  className?: string;
  disabled?: boolean;
  setWaitCursor?: boolean;
};

export function Button({
  children,
  variant = 'primary',
  size = 'md',
  onClick,
  type = 'button',
  className = '',
  disabled = false
  , setWaitCursor = false
}: ButtonProps) {
  const baseStyles = 'inline-flex items-center justify-center font-medium rounded-lg focus:outline-none focus:ring-2 focus:ring-offset-2 disabled:opacity-50 disabled:cursor-not-allowed';

  const variantStyles = {
    primary: 'bg-[var(--btn-background)] text-white hover:bg-[var(--hvr-background)] focus:ring-[var(--color-hvr-background)] transition-colors',
    secondary: 'border-2 border-[var(--color-border)] bg-[var(--color-background)] text-[var(--color-foreground)] hover:bg-[var(--darker-background)] focus:ring-[var(--color-hvr-background)] transition-colors',
    ghost: 'text-[var(--color-foreground)] hover:bg-[var(--darker-background)] focus:ring-[var(--color-hvr-background)] transition-colors',
    danger: 'bg-red-600 text-white hover:bg-red-700 focus:ring-red-500 transition-colors',
  };

  const sizeStyles = {
    sm: 'px-3 py-1.5 text-[length:calc(0.875rem*var(--font-size-multiplier,1))]',
    md: 'px-4 py-2 text-[length:calc(1rem*var(--font-size-multiplier,1))]',
    lg: 'px-6 py-3 text-[length:calc(1.125rem*var(--font-size-multiplier,1))]'
  };

  const handleClick = (e: any) => {
    if (setWaitCursor && typeof document !== 'undefined') {
      document.body.style.cursor = 'wait';
    }
    if (onClick) onClick();
  };

  return (
    <button
      type={type}
      onClick={handleClick}
      disabled={disabled}
      className={`${baseStyles} ${variantStyles[variant]} ${sizeStyles[size]} ${className}`}
    >
      {children}
    </button>
  );
}
