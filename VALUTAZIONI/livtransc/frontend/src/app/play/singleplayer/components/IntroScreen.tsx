import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";

interface IntroScreenProps {
  onStart: () => void;
}

export default function IntroScreen({ onStart }: IntroScreenProps) {
	const { t } = useTranslation();
  return (
    <div className="flex-1 w-full flex flex-col items-center justify-center bg-[#0F172A] text-white overflow-hidden p-0">
      <div className="max-w-xl w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center text-center gap-8">
        <div className="flex flex-col gap-2">
          <span className="text-5xl">🎯</span>
          <h1 className="text-4xl font-black text-[#FACC15] tracking-tighter uppercase">{t("play.singleplayer")}</h1>
        </div>

        <div className="bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155] w-full text-left flex flex-col gap-4">
          <h2 className="text-xl font-bold flex items-center gap-2">📜 {t("singleplayer.rules")}</h2>
          <ul className="space-y-3 text-[#94A3B8] text-sm">
            <li className="flex gap-3">
              <span className="text-[#FACC15]">●</span>
              <span>{t("singleplayer.rule1")}</span>
            </li>
            <li className="flex gap-3">
              <span className="text-[#FACC15]">●</span>
              <span>{t("singleplayer.rule2")}</span>
            </li>
            <li className="flex gap-3">
              <span className="text-[#FACC15]">●</span>
              <span>{t("singleplayer.rule3")}</span>
            </li>
          </ul>
        </div>

        <button
          onClick={onStart}
          className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black text-xl hover:scale-[1.02] active:scale-[0.98] transition-all shadow-[0_0_20px_rgba(250,204,21,0.3)] cursor-pointer"
        >
          {t("lobby.start_game")}
        </button>

        <Link href="/play" className="text-[#475569] hover:text-[#94A3B8] transition text-sm font-medium">
          {t("profile.back_to_profile")}
        </Link>
      </div>
    </div>
  );
}
