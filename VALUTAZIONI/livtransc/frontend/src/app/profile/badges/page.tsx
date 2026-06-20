'use client';

import { useState, useEffect } from 'react';
import Link from 'next/link';
import { useTranslation } from "@/hooks/useTranslation";

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

interface Badge {
  id: number;
  image: string;
  name: string;
  label: string;
  duration: string;
}

export default function BadgesPage() {
  const { t } = useTranslation();
  const [to_complete, setToComplete] = useState<Badge[]>([]);
  const [completed, setCompleted] = useState<Badge[]>([]);
  const [loading, setLoading] = useState(true);

  const takeBadges = async () => {
    try {
      const response = await fetch(`${API_BASE}/api/get_all_badges`,
        { credentials: 'include' }
      );

      if (!response.ok) throw new Error('Error loading badges');

      const data = await response.json();
      console.log(data);
      const to_complete = (data.to_complete || []).map((m: any, index: number) => ({
        id: index,
        image: m.image,
        name: m.name,
        label: m.description
      }));
      const completed = (data.completed || []).map((m: any, index: number) => ({
        id: index,
        image: m.image,
        name: m.name,
        label: m.description
      }));
      setToComplete(to_complete);
      setCompleted(completed);
    } catch (err) {
      console.error('Error:', err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    takeBadges();
  }, []);

  return (
    <div className="min-h-screen bg-[#0F172A] text-[#F8FAFC] font-sans p-6">
      <div className="max-w-4xl mx-auto flex flex-col gap-8">
        {/* Header */}
        <div className="flex items-center justify-between">
          <div className="flex flex-col">
            <h1 className="text-3xl font-bold text-[#FACC15]">🏅 {t('badges.all')}</h1>
            <p className="text-[#94A3B8] text-sm mt-1">{completed.length} / {completed.length + to_complete.length} {t('badges.completed')}</p>
          </div>
          <Link href="/profile">
            <button className="px-5 py-2 rounded-lg bg-[#334155] hover:bg-[#475569] text-white font-bold transition cursor-pointer flex items-center gap-2">
              <span>←</span> {t('profile.back_to_profile')}
            </button>
          </Link>
        </div>

        {/* Badges Grid */}
        {completed.length > 0 && <div className="py-2">
          <div className="grid grid-cols-[repeat(auto-fill,minmax(6rem,1fr))] gap-4">
            {completed.map((b, i) => (
              <div
                key={i}
                className="group relative flex flex-col items-center justify-center w-24 rounded-2xl bg-[#1E293B] border border-[#334155] px-3 py-4 cursor-pointer transition hover:border-[#FACC15]"
              >
                {/* Emoji */}
                <span className="text-4xl mb-2">{b.image}</span>

                {/* Nome */}
                <span className="text-xs font-bold text-[#F8FAFC] text-center leading-tight">
                  {t(b.name)}
                </span>

                {/* Tooltip on hover */}
                <div className="absolute bottom-full mb-2 left-1/2 -translate-x-1/2 opacity-0 group-hover:opacity-100 transition pointer-events-none z-10">
                  <div className="bg-[#FACC15] text-[#0F172A] text-xs font-bold px-3 py-1 rounded-lg whitespace-nowrap shadow-lg">
                    {t(b.label)}
                  </div>
                  <div className="w-2 h-2 bg-[#FACC15] rotate-45 mx-auto -mt-1" />
                </div>
              </div>
            ))}
          </div>
        </div>}

        {to_complete.length > 0 && <div className="py-2">
          <div className="grid grid-cols-[repeat(auto-fill,minmax(6rem,1fr))] gap-4">
            {to_complete.map((b, i) => (
              <div
                key={i}
                className="group relative flex flex-col items-center justify-center w-24 rounded-2xl bg-[#1E293B] border border-[#334155] px-3 py-4 cursor-pointer transition hover:border-[#FACC15]"
              >
                {/* Emoji */}
                <span className="text-4xl mb-2" style={{ filter: "grayscale(1)" }}>{b.image}</span>

                {/* Nome */}
                <span className="text-xs font-bold text-[#F8FAFC] text-center leading-tight">
                  {t(b.name)}
                </span>

                {/* Tooltip on hover */}
                <div className="absolute bottom-full mb-2 left-1/2 -translate-x-1/2 opacity-0 group-hover:opacity-100 transition pointer-events-none z-10">
                  <div className="bg-[#FACC15] text-[#0F172A] text-xs font-bold px-3 py-1 rounded-lg whitespace-nowrap shadow-lg">
                    {t(b.label)}
                  </div>
                  <div className="w-2 h-2 bg-[#FACC15] rotate-45 mx-auto -mt-1" />
                </div>
              </div>
            ))}
          </div>
        </div>}

      </div>
    </div>
  );
}
