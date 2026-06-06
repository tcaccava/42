type BadgeVariant = 'campus' | 'tag' | 'status';

type BadgeProps = {
  children: React.ReactNode;
  variant?: BadgeVariant;
  campus?: string;
  className?: string;
};

const campusColors: Record<string, string> = {
  'Rome': 'bg-red-100 text-red-700 border-red-300',
  'Milan': 'bg-blue-100 text-blue-700 border-blue-300',
  'Florence': 'bg-purple-100 text-purple-700 border-purple-300',
  'Turin': 'bg-green-100 text-green-700 border-green-300',
  'default': 'bg-gray-100 text-gray-700 border-gray-300'
};

export function Badge({ children, variant = 'tag', campus, className = '' }: BadgeProps) {
  let styles = 'inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium border';
  
  if (variant === 'campus' && campus) {
    styles += ` ${campusColors[campus] || campusColors.default}`;
  } else if (variant === 'tag') {
    styles += ' bg-blue-50 text-blue-600 border-blue-200';
  } else if (variant === 'status') {
    styles += ' bg-green-50 text-green-600 border-green-200';
  }
  
  return (
    <span className={`${styles} ${className}`}>
      {children}
    </span>
  );
}
