'use client';

import { useState, useEffect } from 'react';
import Link from 'next/link';
import { useTranslation } from "@/hooks/useTranslation";

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

interface Match {
  id: number;
  opponent: string;
  result: string;
  score: string;
  date: string;
  duration: string;
  ranking: number;
}

export default function MatchesPage() {
  const { t } = useTranslation();
  const [matches, setMatches] = useState<Match[]>([]);
  const [loading, setLoading] = useState(true);

  const takeMatches = async () => {
    try {
      const response = await fetch(`${API_BASE}/api/get_old_matches`,
        { credentials: 'include' }
      );

      if (!response.ok) throw new Error('Error loading matches');

      const data = await response.json();
      const formattedMatches = (data.old_matches || []).map((m: any, index: number) => ({
        id: index,
        opponent: m.mode || 'AI',
        result: m.status === 0 ? 'Win' : 'Lost',
        score: `${m.correct_answers}/15`,
        date: new Date(m.finished_at).toLocaleDateString(),
        duration: 'N/A',
        ranking: m.ranking || 0
      }));
      setMatches(formattedMatches);
    } catch (err) {
      console.error('Error:', err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    takeMatches();
  }, []);

  return (
    <div className="min-h-screen bg-[#0F172A] text-[#F8FAFC] font-sans p-6">
      <div className="max-w-2xl mx-auto flex flex-col gap-8">
        {/* Header */}
        <div className="flex items-center justify-between">
          <div className="flex flex-col">
            <h1 className="text-3xl font-bold text-[#FACC15]">📜 {t('matches.title')}</h1>
            <p className="text-[#94A3B8] text-sm mt-1">{t('matches.subtitle')}</p>
          </div>
          <Link href="/profile">
            <button className="px-5 py-2 rounded-lg bg-[#334155] hover:bg-[#475569] text-white font-bold transition cursor-pointer flex items-center gap-2">
              <span>←</span> {t('profile.back_to_profile')}
            </button>
          </Link>
        </div>

        {/* Matches List */}
        <div className="bg-[#1E293B] rounded-2xl shadow-xl overflow-hidden border border-[#334155]">
          <div className="p-6 flex flex-col gap-0">
            {loading ? (
              <div className="text-center py-10 text-[#94A3B8]">{t('matches.loading')}</div>
            ) : matches.length > 0 ? (
              matches.map((m, i) => (
                <div
                  key={m.id}
                  className={`flex justify-between items-center py-5 transition hover:bg-[#334155]/30 px-4 -mx-4 rounded-xl ${i !== matches.length - 1 ? 'border-b border-[#334155]' : ''}`}
                >
                  <div className="flex flex-col">
                    <span className="font-bold text-lg text-[#F8FAFC]">{t(m.opponent)}</span>
                    <div className="flex gap-3 text-xs text-[#94A3B8] font-medium uppercase tracking-wider">
                      <span>{m.date}</span>
                    </div>
                  </div>

                  <div className="flex items-center gap-6">
                    <div className="flex flex-col items-end">
                      <span className="font-mono text-2xl font-bold tracking-tighter">{m.score}</span>
                    </div>
                    <div
                      className={`min-w-16 inline-flex items-center justify-center px-2 py-1 rounded-full font-black tracking-widest ${
                        m.opponent !== "play.multiplayer"
                          ? m.result === 'Win'
                            ? 'bg-green-500/20 text-green-400 border border-green-500/30 text-xs tracking-widest'
                            : 'bg-red-500/20 text-red-400 border border-red-500/30 text-xs tracking-widest'
                          : 'font-mono text-2xl font-bold tracking-tighter'
                      }`}
                    >
                      {m.opponent === "play.multiplayer"
                        ? (
                            m.ranking === 1
                              ? "🥇"
                              : m.ranking === 2
                              ? "🥈"
                              : m.ranking === 3
                              ? "🥉"
                              : `#${m.ranking}`
                          )
                        : (
                            m.result === 'Win'
                              ? t('matches.win')
                              : t('matches.lost')
                          )
                      }
                    </div>
                  </div>
                </div>
              ))
            ) : (
              <div className="text-center py-10 text-[#94A3B8]">{t('matches.no_matches')}</div>
            )}
          </div>
        </div>

        {/* Footer info */}
        <div className="text-center text-[#475569] text-xs">
          {loading ? 'Fetching history...' : `${t('matches.showing_last')} ${matches.length} ${t('matches.matches')}`}
        </div>
      </div>
    </div>
  );
}
