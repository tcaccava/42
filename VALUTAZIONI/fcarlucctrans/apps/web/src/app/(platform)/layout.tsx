import { Suspense } from 'react';
import { Header } from '@/components/Header';
import { Sidebar } from '@/components/Sidebar';
import { fetchServerProfile } from '@/server/auth';

export const dynamic = 'force-dynamic';

export default async function PlatformLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  const profile = await fetchServerProfile();
  const user = profile.status === 'success' ? profile.user ?? null : null;

  return (
    <div className="min-h-screen bg-background text-foreground">
      <Suspense fallback={<div className="h-16 bg-background border-b border-border sticky top-0 z-50" />}>
        <Header user={user} />
      </Suspense>

      <div className="max-w-screen-xl mx-auto">
        <div className="lg:flex">
          <aside className="hidden lg:block w-56 xl:w-64 shrink-0">
            <div className="sticky top-16 h-[calc(100vh-4rem)] overflow-y-auto px-2">
              <Sidebar />
            </div>
          </aside>

          <main id="main-content" className="flex-1 px-4 py-8 pb-24 lg:pb-8 min-w-0">
            {children}
          </main>
        </div>
      </div>
    </div>
  );
}
