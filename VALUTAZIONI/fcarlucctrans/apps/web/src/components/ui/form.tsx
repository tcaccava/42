import type { ReactNode, SelectHTMLAttributes } from 'react';

export const selectClassName =
  'w-full rounded-lg border border-gray-300 bg-white px-3 py-2 text-gray-900 outline-none transition-colors focus:border-teal-600 focus:ring-2 focus:ring-teal-500 disabled:cursor-not-allowed disabled:opacity-50';

type FormLabelProps = {
  htmlFor?: string;
  children: ReactNode;
  className?: string;
};

export function FormLabel({ htmlFor, children, className = '' }: FormLabelProps) {
  return (
    <label
      htmlFor={htmlFor}
      className={`mb-2 block text-sm font-medium text-gray-700 ${className}`}
    >
      {children}
    </label>
  );
}

type SelectFieldProps = SelectHTMLAttributes<HTMLSelectElement> & {
  label?: string;
};

export function SelectField({
  label,
  id,
  className = '',
  children,
  ...props
}: SelectFieldProps) {
  return (
    <div>
      {label && <FormLabel htmlFor={id}>{label}</FormLabel>}
      <select
        id={id}
        className={`${selectClassName} ${className}`}
        {...props}
      >
        {children}
      </select>
    </div>
  );
}

export const dropZoneClassName =
  'cursor-pointer rounded-xl border-2 border-dashed border-gray-300 bg-gray-50 p-6 text-center text-sm text-gray-500 transition-colors hover:border-teal-300 hover:bg-teal-50/30';
