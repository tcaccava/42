/**
 * Loading skeleton for events page
 */
export default function EventsLoading() {
  return (
    <div className="space-y-4">
      <div className="h-12 bg-darker-background rounded-lg animate-pulse" />

      {Array.from({ length: 3 }).map((_, i) => (
        <div key={i} className="bg-background rounded-xl p-6 space-y-4 shadow-sm animate-pulse">
          <div className="h-6 bg-darker-background rounded w-1/2" />
          <div className="h-4 bg-darker-background rounded w-full" />
          <div className="grid grid-cols-2 gap-4">
            <div className="h-24 bg-darker-background rounded" />
            <div className="h-24 bg-darker-background rounded" />
          </div>
        </div>
      ))}
    </div>
  );
}
