"use client";
import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";

interface Match {
  id: number;
  opponent: string;
  result: string;
  score: string;
  date: string;
  duration: string;
  ranking: number;
}

interface MatchHistoryProps {
  old_matches: Match[];
}

export default function MatchHistory({ old_matches }: MatchHistoryProps) {
  const { t } = useTranslation();

  return (
    <div className="w-full">
      <div className="flex justify-between items-center mb-2">
        <span className="text-lg font-bold">📜 {t("profile.last_matches_title")}</span>
        <Link href="/profile/matches">
          <button className="text-sm font-semibold text-[#FACC15] hover:underline transition cursor-pointer">{t("profile.view_all")}</button>
        </Link>
      </div>
      <div className="bg-[#1E293B] rounded-xl shadow p-6 flex flex-col gap-4">
        {old_matches.map((m, i) => (
          <div key={i} className="flex justify-between items-center border-b border-[#334155] pb-2 last:border-0 last:pb-0">
            <div className="flex flex-col">
              <span className="font-semibold text-[#F8FAFC]">{t(m.opponent)}</span>
              <span className="text-sm text-[#94A3B8]">{m.date}</span>
            </div>
            <div className="flex gap-4 items-center">
              <span className="font-mono text-lg">{m.score}</span>
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
        ))}
      </div>
    </div>
  );
}
