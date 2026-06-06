/**
 * Loading skeleton for profile page
 */
export default function ProfileLoading() {
  return (
    <div className="space-y-6">
      {/* Header skeleton */}
      <div className="bg-background rounded-xl p-8 space-y-6 shadow-lg animate-pulse">
        <div className="flex items-center gap-6">
          <div className="w-24 h-24 rounded-full bg-darker-background" />
          <div className="flex-1 space-y-3">
            <div className="h-8 bg-darker-background rounded w-1/2" />
            <div className="h-4 bg-darker-background rounded w-1/3" />
            <div className="h-4 bg-darker-background rounded w-1/4" />
          </div>
        </div>
      </div>

      {/* Posts skeleton */}
      <div className="space-y-4">
        {Array.from({ length: 2 }).map((_, i) => (
          <div key={i} className="bg-background rounded-xl p-6 space-y-4 shadow-sm animate-pulse">
            <div className="h-4 bg-darker-background rounded w-full" />
            <div className="h-48 bg-darker-background rounded" />
          </div>
        ))}
      </div>
    </div>
  );
}
