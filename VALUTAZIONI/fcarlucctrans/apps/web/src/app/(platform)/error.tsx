'use client';

import { useEffect } from 'react';
import { local } from '../../utilities'

interface ErrorBoundaryProps {
  error: Error & { digest?: string };
  reset: () => void;
}

/**
 * Error boundary for (platform) layout
 * Handles errors from server components and data fetching
 */
export default function PlatformError({ error, reset }: ErrorBoundaryProps) {
  useEffect(() => {
    // Log error for monitoring
    console.error('Platform layout error:', error);
  }, [error]);

  const t = local.useLocalization().t;
  return (
    <div className="min-h-screen bg-gradient-to-br from-background to-darker-background flex items-center justify-center px-4">
      <div className="max-w-md w-full bg-background rounded-2xl shadow-lg p-8 text-center space-y-6">
        <div className="flex justify-center">
          <div className="w-16 h-16 bg-red-100 rounded-full flex items-center justify-center">
            <svg
              className="w-8 h-8 text-red-600"
              fill="none"
              stroke="currentColor"
              viewBox="0 0 24 24"
            >
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                strokeWidth={2}
                d="M12 8v4m0 4v.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"
              />
            </svg>
          </div>
        </div>

        <div className="space-y-2">
          <h2 className="text-2xl font-bold text-gray-900">{t.somethingWrong}</h2>
          <p className="text-gray-600">
            {error.message || 'An unexpected error occurred while loading this page.'}
          </p>
        </div>

        <button
          onClick={reset}
          className="w-full px-4 py-3 bg-btn-background text-foreground rounded-lg font-semibold hover:bg-hvr-background transition-colors"
        >
          {t.tryAgain}
        </button>

        <a
          href="/"
          className="block w-full px-4 py-3 text-btn-background border border-btn-background rounded-lg font-semibold hover:bg-hvr-background transition-colors"
        >
          {t.goHome}
        </a>
      </div>
    </div>
  );
}
