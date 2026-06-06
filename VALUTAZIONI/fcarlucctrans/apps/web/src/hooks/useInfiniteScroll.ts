'use client';

import { useEffect, useRef } from 'react';

export function useInfiniteScroll(
  onLoadMore: () => void,
  enabled: boolean,
) {
  const sentinelRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (!enabled) return;

    const element = sentinelRef.current;
    if (!element) return;

    const observer = new IntersectionObserver(
      (entries) => {
        if (entries[0]?.isIntersecting) {
          onLoadMore();
        }
      },
      { rootMargin: '240px' },
    );

    observer.observe(element);
    return () => observer.disconnect();
  }, [enabled, onLoadMore]);

  return sentinelRef;
}
