export default function SearchLoading() {
  return (
    <div className="mx-auto max-w-4xl space-y-5 px-4 py-8">
      <div className="space-y-2">
        <div className="h-8 w-40 animate-pulse rounded-lg bg-darker-background" />
        <div className="h-4 w-64 animate-pulse rounded bg-darker-background" />
      </div>
      <div className="h-28 animate-pulse rounded-2xl bg-darker-background" />
      <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
        {Array.from({ length: 6 }).map((_, i) => (
          <div
            key={i}
            className="h-56 animate-pulse rounded-xl bg-darker-background"
          />
        ))}
      </div>
    </div>
  );
}
