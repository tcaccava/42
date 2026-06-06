'use client';

import { useCallback, useEffect, useState } from 'react';
import { ChevronLeft, ChevronRight, ImageIcon } from 'lucide-react';

type ScreenshotCarouselProps = {
  images: string[];
  altPrefix: string;
  className?: string;
};

export function ScreenshotCarousel({
  images,
  altPrefix,
  className = '',
}: ScreenshotCarouselProps) {
  const [index, setIndex] = useState(0);
  const count = images.length;

  const goTo = useCallback(
    (next: number) => {
      if (count === 0) return;
      setIndex((next + count) % count);
    },
    [count],
  );

  const goPrev = useCallback(() => goTo(index - 1), [goTo, index]);
  const goNext = useCallback(() => goTo(index + 1), [goTo, index]);

  useEffect(() => {
    if (index >= count && count > 0) {
      setIndex(0);
    }
  }, [count, index]);

  useEffect(() => {
    if (count <= 1) return;

    const onKeyDown = (event: KeyboardEvent) => {
      if (event.key === 'ArrowLeft') goPrev();
      if (event.key === 'ArrowRight') goNext();
    };

    window.addEventListener('keydown', onKeyDown);
    return () => window.removeEventListener('keydown', onKeyDown);
  }, [count, goNext, goPrev]);

  if (count === 0) {
    return (
      <div
        className={`flex aspect-video flex-col items-center justify-center rounded-xl border border-dashed border-gray-200 bg-gray-50 text-gray-400 ${className}`}
      >
        <ImageIcon className="mb-2 h-8 w-8" aria-hidden="true" />
        <p className="text-sm">No screenshots uploaded.</p>
      </div>
    );
  }

  return (
    <div className={`space-y-3 ${className}`}>
      <div className="group relative overflow-hidden rounded-xl border border-gray-200 bg-gray-50">
        <div className="relative aspect-video w-full">
          {/* eslint-disable-next-line @next/next/no-img-element */}
          <img
            key={images[index]}
            src={images[index]}
            alt={`${altPrefix} screenshot ${index + 1}`}
            className="h-full w-full object-contain"
          />
        </div>

        {count > 1 && (
          <>
            <button
              type="button"
              onClick={goPrev}
              aria-label="Previous screenshot"
              className="absolute left-2 top-1/2 flex h-9 w-9 -translate-y-1/2 items-center justify-center rounded-full border border-gray-200 bg-white/95 text-gray-700 shadow-sm opacity-0 transition-opacity hover:bg-white group-hover:opacity-100 focus:opacity-100"
            >
              <ChevronLeft className="h-5 w-5" />
            </button>
            <button
              type="button"
              onClick={goNext}
              aria-label="Next screenshot"
              className="absolute right-2 top-1/2 flex h-9 w-9 -translate-y-1/2 items-center justify-center rounded-full border border-gray-200 bg-white/95 text-gray-700 shadow-sm opacity-0 transition-opacity hover:bg-white group-hover:opacity-100 focus:opacity-100"
            >
              <ChevronRight className="h-5 w-5" />
            </button>
            <div className="absolute bottom-2 right-2 rounded-full bg-black/60 px-2.5 py-0.5 text-xs font-medium text-white">
              {index + 1} / {count}
            </div>
          </>
        )}
      </div>

      {count > 1 && (
        <div className="flex gap-2 overflow-x-auto pb-1">
          {images.map((src, thumbIndex) => {
            const isActive = thumbIndex === index;
            return (
              <button
                key={`${src}-${thumbIndex}`}
                type="button"
                onClick={() => setIndex(thumbIndex)}
                aria-label={`View screenshot ${thumbIndex + 1}`}
                aria-current={isActive ? 'true' : undefined}
                className={`relative h-14 w-20 shrink-0 overflow-hidden rounded-lg border-2 transition-colors ${
                  isActive
                    ? 'border-teal-500 ring-2 ring-teal-100'
                    : 'border-gray-200 hover:border-teal-300'
                }`}
              >
                {/* eslint-disable-next-line @next/next/no-img-element */}
                <img
                  src={src}
                  alt=""
                  className="h-full w-full object-cover"
                />
              </button>
            );
          })}
        </div>
      )}
    </div>
  );
}
