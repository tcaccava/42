"use client";
import { useTranslation } from "@/hooks/useTranslation";

interface ScoreEntry {
  user_id: number;
  display_name: string;
  score: number;
  rank: number;
}

interface ResultsScreenProps {
  rankings: ScoreEntry[];
  onPlayAgain?: () => void;
  onBackToMenu: () => void;
}

export default function ResultsScreen({ rankings, onPlayAgain, onBackToMenu }: ResultsScreenProps) {
  const sortedRankings = [...rankings].sort((a, b) => b.score - a.score);
  const topScore = sortedRankings.length > 0 ? sortedRankings[0].score : 0;
  const { t } = useTranslation();

  return (
    <div className="min-h-screen flex flex-col items-center justify-center p-6 bg-[#0F172A] text-white pt-2 pb-12 overflow-y-auto">
      <div className="max-w-xl w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center gap-8 animate-in fade-in fill-mode-both duration-1000 slide-in-from-bottom-10">
        <div className="flex flex-col items-center gap-2">
          <span className="text-7xl animate-bounce">🏆</span>
          <h2 className="text-4xl font-black text-[#FACC15] tracking-tighter">{t("play.challenge_ends")}</h2>
        </div>

        <div className="w-full flex flex-col gap-3 bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155]">
          <h3 className="text-[#475569] font-black text-[10px] uppercase tracking-[0.3em] text-center mb-2">
            {t("play.final_standings")}
          </h3>
          {sortedRankings.map((entry) => {
            const isFirst = entry.score === topScore && entry.score > 0;

            return (
              <div
                key={entry.user_id}
                className={`flex items-center gap-4 p-4 rounded-xl border transition-all duration-700
                  ${isFirst ? "bg-[#FACC15]/10 border-[#FACC15] shadow-[0_0_20px_rgba(250,204,21,0.1)]" : "bg-[#1E293B] border-[#334155]"}
                `}
              >
                <div
                  className={`w-8 h-8 rounded-lg flex items-center justify-center font-black
                    ${isFirst ? "bg-[#FACC15] text-[#0F172A]" : "bg-[#334155] text-[#94A3B8]"}
                  `}
                >
                  {entry.rank}
                </div>
                <div className="flex-1 font-bold text-lg">{entry.display_name}</div>
                <div className={`font-black text-2xl ${isFirst ? "text-[#FACC15]" : "text-white"}`}>
                  {entry.score}{" "}
                  <span className="text-[10px] font-bold text-[#475569] uppercase tracking-tighter">{t("play.points")}</span>
                </div>
              </div>
            );
          })}
        </div>

        <div className="w-full flex flex-col gap-3 mt-4">
          {onPlayAgain && (
            <button
              onClick={onPlayAgain}
              className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black text-xl hover:scale-[1.02] active:scale-[0.98] transition-all shadow-[0_0_30px_rgba(250,204,21,0.2)] cursor-pointer"
            >
              {t("singleplayer.play_again")}
            </button>
          )}
          <button
            onClick={onBackToMenu}
            className="w-full py-3 rounded-xl bg-[#0F172A] border border-[#334155] text-[#94A3B8] font-black text-sm hover:text-white transition text-center cursor-pointer"
          >
            {t("game.back_to_menu")}
          </button>
        </div>
      </div>
    </div>
  );
}
