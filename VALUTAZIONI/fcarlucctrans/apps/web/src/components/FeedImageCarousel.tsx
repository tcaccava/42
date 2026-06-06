'use client';

import { useCallback, useRef, useState } from 'react';
import { ChevronLeft, ChevronRight } from 'lucide-react';

type FeedImageCarouselProps = {
  images: string[];
  alt: string;
  onClick?: () => void;
};

export function FeedImageCarousel({ images, alt, onClick }: FeedImageCarouselProps) {
  const scrollRef = useRef<HTMLDivElement>(null);
  const [hovered, setHovered] = useState(false);
  const [activeIndex, setActiveIndex] = useState(0);
  const count = images.length;
  const canScroll = count > 1;

  const updateActiveIndex = useCallback(() => {
    const el = scrollRef.current;
    if (!el || count <= 1) return;
    const width = el.clientWidth || 1;
    const next = Math.round(el.scrollLeft / width);
    setActiveIndex(Math.min(Math.max(next, 0), count - 1));
  }, [count]);

  const scrollTo = useCallback(
    (direction: -1 | 1) => {
      const el = scrollRef.current;
      if (!el) return;
      el.scrollBy({ left: direction * el.clientWidth, behavior: 'smooth' });
    },
    [],
  );

  const handleWheel = useCallback(
    (event: React.WheelEvent<HTMLDivElement>) => {
      if (!canScroll || !hovered) return;
      event.preventDefault();
      scrollRef.current?.scrollBy({
        left: event.deltaY + event.deltaX,
        behavior: 'auto',
      });
    },
    [canScroll, hovered],
  );

  return (
    <div
      className="px-4 pb-1"
      onMouseEnter={() => setHovered(true)}
      onMouseLeave={() => setHovered(false)}
    >
      <div
        className={`group relative rounded-xl bg-gray-100 p-3 ${
          canScroll && hovered ? 'ring-1 ring-teal-200/80' : ''
        }`}
      >
        <div
          ref={scrollRef}
          onScroll={updateActiveIndex}
          onWheel={handleWheel}
          className={`flex aspect-video snap-x snap-mandatory overflow-x-auto rounded-lg bg-gray-50 ${
            canScroll
              ? hovered
                ? 'cursor-grab active:cursor-grabbing feed-carousel-scroll feed-carousel-scroll-active'
                : 'feed-carousel-scroll feed-carousel-scroll-idle'
              : ''
          }`}
          role={canScroll ? 'group' : undefined}
          aria-label={canScroll ? `${alt} screenshots` : undefined}
        >
          {images.map((src, index) => (
            <button
              key={`${src}-${index}`}
              type="button"
              onClick={onClick}
              className="relative h-full w-full shrink-0 snap-center snap-always overflow-hidden rounded-md"
              aria-label={
                canScroll ? `${alt} image ${index + 1} of ${count}` : alt
              }
            >
              {/* eslint-disable-next-line @next/next/no-img-element */}
              <img
                src={src}
                alt=""
                className="h-full w-full object-cover"
                draggable={false}
              />
            </button>
          ))}
        </div>

        {canScroll && hovered ? (
          <>
            <button
              type="button"
              onClick={(event) => {
                event.stopPropagation();
                scrollTo(-1);
              }}
              aria-label="Previous screenshot"
              className="absolute left-5 top-1/2 z-10 flex h-8 w-8 -translate-y-1/2 items-center justify-center rounded-full border border-gray-200 bg-white/95 text-gray-700 shadow-sm transition hover:bg-white"
            >
              <ChevronLeft className="h-4 w-4" />
            </button>
            <button
              type="button"
              onClick={(event) => {
                event.stopPropagation();
                scrollTo(1);
              }}
              aria-label="Next screenshot"
              className="absolute right-5 top-1/2 z-10 flex h-8 w-8 -translate-y-1/2 items-center justify-center rounded-full border border-gray-200 bg-white/95 text-gray-700 shadow-sm transition hover:bg-white"
            >
              <ChevronRight className="h-4 w-4" />
            </button>
            <div className="pointer-events-none absolute bottom-5 right-5 rounded-full bg-black/60 px-2 py-0.5 text-[11px] font-medium text-white">
              {activeIndex + 1} / {count}
            </div>
            <div className="pointer-events-none absolute inset-x-5 bottom-3 flex justify-center gap-1.5">
              {images.map((src, index) => (
                <span
                  key={`dot-${src}-${index}`}
                  className={`h-1.5 rounded-full transition-all ${
                    index === activeIndex
                      ? 'w-4 bg-teal-500'
                      : 'w-1.5 bg-gray-300'
                  }`}
                />
              ))}
            </div>
          </>
        ) : null}
      </div>
    </div>
  );
}
