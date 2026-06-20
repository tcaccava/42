import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";

interface ResultsScreenProps {
  isWrong: boolean;
  message: string;
  questionNumber: number;
  onRetry: () => void;
}

export default function ResultsScreen({ isWrong, message, questionNumber, onRetry }: ResultsScreenProps) {
  const { t } = useTranslation();
  return (
    <div className="flex-1 w-full flex flex-col items-center justify-center bg-[#0F172A] text-white p-0 overflow-hidden">
      <div className="max-w-md w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center text-center gap-8">
        <span className="text-6xl">{isWrong ? "💀" : "🏆"}</span>
        <div className="flex flex-col gap-1">
          <h2 className={`text-3xl font-black ${isWrong ? 'text-red-500' : 'text-green-400'}`}>
            {isWrong ? t("singleplayer.game_over") : t("singleplayer.victory")}
          </h2>
          <p className="text-[#94A3B8] font-medium">{message}</p>
        </div>

        <div className="flex flex-col gap-1 bg-[#0F172A]/50 px-8 py-4 rounded-2xl border border-[#334155] w-full">
          <span className="text-[#94A3B8] text-xs font-bold uppercase tracking-widest">{t("singleplayer.questions_cleared")}</span>
          <span className="text-5xl font-black text-[#FACC15] tracking-tighter">
            {isWrong ? questionNumber : 15}
            <span className="text-xl text-[#475569]"> / 15</span>
          </span>
        </div>

        <div className="w-full flex flex-col gap-3">
          <button
            onClick={onRetry}
            className="w-full py-4 rounded-xl bg-[#334155] hover:bg-[#475569] text-white font-black transition cursor-pointer"
          >
            {isWrong ? t("singleplayer.retry") : t("singleplayer.play_again")}
          </button>
          <Link
            href="/play"
            className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black hover:scale-[1.02] transition cursor-pointer"
          >
            {t("singleplayer.menu")}
          </Link>
        </div>
      </div>
    </div>
  );
}
