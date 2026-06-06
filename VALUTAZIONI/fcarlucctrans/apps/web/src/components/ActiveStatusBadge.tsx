type ActiveStatusBadgeProps = {
  isActive?: boolean;
  className?: string;
};

export function ActiveStatusBadge({ isActive, className = '' }: ActiveStatusBadgeProps) {
  if (!isActive) return null;

  return (
    <div
      className={`absolute bottom-2 right-2 flex h-5 w-5 ${className}`}
      title="Active now"
      aria-label="User is active"
    >
      <span className="animate-ping absolute inline-flex h-full w-full rounded-full bg-green-400 opacity-75" />
      <span className="relative inline-flex rounded-full h-5 w-5 bg-green-500 border-2 border-white" />
    </div>
  );
}
