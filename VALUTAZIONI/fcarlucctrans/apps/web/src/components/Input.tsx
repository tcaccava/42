import { InputHTMLAttributes, TextareaHTMLAttributes } from 'react';

type InputProps = InputHTMLAttributes<HTMLInputElement> & {
  label?: string;
  error?: string;
  helperText?: string;
};

type TextareaProps = TextareaHTMLAttributes<HTMLTextAreaElement> & {
  label?: string;
  error?: string;
  helperText?: string;
};

export function Input({ label, error, helperText, className = '', ...props }: InputProps) {
  const errorId = props.id ? `${props.id}-error` : undefined;

  return (
    <div>
      {label && (
        <label
          htmlFor={props.id}
          className="block text-[length:calc(0.875rem*var(--font-size-multiplier,1))] font-medium text-[var(--color-foreground)] mb-[calc(0.5rem+0.1em)]"
        >
          {label}
        </label>
      )}
      <input
        className={`w-full px-4 py-2 border-2 border-[var(--color-border)] bg-[var(--color-background)] text-[var(--color-foreground)] rounded-lg focus:outline-none focus:ring-2 focus:ring-[var(--color-hvr-background)] focus:ring-offset-2 focus:border-[var(--color-hvr-background)] transition-colors placeholder:text-[var(--color-foreground)] placeholder:opacity-60 ${className}`}
        aria-invalid={error ? 'true' : 'false'}
        aria-describedby={errorId}
        {...props}
      />
      {(error || helperText) && (
        <div id={errorId} className={`text-[length:calc(0.75rem*var(--font-size-multiplier,1))] mt-1 ${error ? 'text-red-600 dark:text-red-400' : 'text-[var(--color-foreground)] opacity-70'}`}>
          {error || helperText}
        </div>
      )}
    </div>
  );
}

export function Textarea({ label, error, helperText, className = '', ...props }: TextareaProps) {
  const errorId = props.id ? `${props.id}-error` : undefined;

  return (
    <div>
      {label && (
        <label
          htmlFor={props.id}
          className="block text-[length:calc(0.875rem*var(--font-size-multiplier,1))] font-medium text-[var(--color-foreground)] mb-[calc(0.5rem+0.1em)]"
        >
          {label}
        </label>
      )}
      <textarea
        className={`w-full px-4 py-2 border-2 border-[var(--color-border)] bg-[var(--color-background)] text-[var(--color-foreground)] rounded-lg focus:outline-none focus:ring-2 focus:ring-[var(--color-hvr-background)] focus:ring-offset-2 focus:border-[var(--color-hvr-background)] transition-colors placeholder:text-[var(--color-foreground)] placeholder:opacity-60 resize-none ${className}`}
        aria-invalid={error ? 'true' : 'false'}
        aria-describedby={errorId}
        {...props}
      />
      {(error || helperText) && (
        <div id={errorId} className={`text-[length:calc(0.75rem*var(--font-size-multiplier,1))] mt-1 ${error ? 'text-red-600 dark:text-red-400' : 'text-[var(--color-foreground)] opacity-70'}`}>
          {error || helperText}
        </div>
      )}
    </div>
  );
}
