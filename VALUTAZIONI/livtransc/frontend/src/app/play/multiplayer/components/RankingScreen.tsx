"use client";
import { useTranslation } from "@/hooks/useTranslation";

interface ScoreEntry {
  user_id: number;
  display_name: string;
  score: number;
  rank: number;
}

interface RankingScreenProps {
  scores: ScoreEntry[];
  questionNumber: number;
  totalQuestions: number;
}

export default function RankingScreen({ scores, questionNumber, totalQuestions }: RankingScreenProps) {
  const sortedScores = [...scores].sort((a, b) => b.score - a.score);
  const topScore = sortedScores.length > 0 ? sortedScores[0].score : 0;
  const { t } = useTranslation();

  return (
    <div className="min-h-screen flex flex-col items-center justify-center p-6 bg-[#0F172A] text-white pt-2 pb-12 overflow-y-auto">
      <div className="max-w-3xl w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col gap-8 animate-in fade-in zoom-in duration-500">
        <div className="flex flex-col gap-1 items-center">
          <span className="text-4xl">📊</span>
          <h1 className="text-4xl font-black text-[#FACC15] tracking-tighter">{t("play.current_ranking")}</h1>
          <p className="text-[#94A3B8] font-medium uppercase text-sm">
            {t("play.questions")} {questionNumber} {t("play.of_15")}
          </p>
        </div>

        <div className="flex flex-col gap-3 bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155]">
          {sortedScores.map((entry, idx) => {
            const isFirst = entry.score === topScore && entry.score > 0;

            return (
              <div
                key={entry.user_id}
                className={`flex items-center gap-4 p-4 rounded-xl border transition-all duration-500
                  ${isFirst ? "bg-[#FACC15]/10 border-[#FACC15]/30" : "bg-[#1E293B] border-[#334155]"}
                `}
              >
                <div
                  className={`w-10 h-10 rounded-lg flex items-center justify-center font-black text-lg
                    ${isFirst ? "bg-[#FACC15] text-[#0F172A]" : "bg-[#334155] text-[#94A3B8]"}
                  `}
                >
                  {entry.rank}
                </div>
                <div className="flex-1">
                  <p className="font-black text-lg tracking-tight">{entry.display_name}</p>
                </div>
                <div className="flex flex-col items-end">
                  <span className="text-2xl font-black text-[#FACC15]">{entry.score}</span>
                  <span className="text-[8px] font-bold text-[#475569] uppercase tracking-widest">{t("play.points")}</span>
                </div>
              </div>
            );
          })}
        </div>
      </div>
    </div>
  );
}
