/**
 * Loading skeleton for feed page
 * Displayed while feed data is being fetched
 */
export default function FeedLoading() {
  return (
    <div className="space-y-4">
      {/* Header skeleton */}
      <div className="h-12 bg-darker-background rounded-lg animate-pulse" />

      {/* Posts skeleton */}
      {Array.from({ length: 3 }).map((_, i) => (
        <div key={i} className="bg-background rounded-xl p-6 space-y-4 shadow-sm animate-pulse">
          <div className="flex items-center gap-3">
            <div className="w-10 h-10 rounded-full bg-darker-background" />
            <div className="space-y-2 flex-1">
              <div className="h-4 bg-darker-background rounded w-32" />
              <div className="h-3 bg-darker-background rounded w-24" />
            </div>
          </div>
          <div className="space-y-3">
            <div className="h-4 bg-darker-background rounded w-full" />
            <div className="h-4 bg-darker-background rounded w-5/6" />
          </div>
          <div className="h-48 bg-darker-background rounded-lg" />
        </div>
      ))}
    </div>
  );
}
