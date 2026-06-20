import { useTranslation } from "@/hooks/useTranslation";

interface Question {
  id: number;
  text: string;
  options: string[];
  number: number;
}

interface GameUIProps {
  question: Question | null;
  time: number;
  optionStatuses: Record<string, "green" | "red" | "yellow" | "none">;
  lifelines: Record<string, boolean>;
  isLocked: boolean;
  onAnswer: (ans: string) => void;
  onUseFiftyFifty: () => void;
  onUseChange: () => void;
  onUseDoubleChance: () => void;
  onUseRefillTime: () => void;
}

export default function GameUI({
  question,
  time,
  optionStatuses,
  lifelines,
  isLocked,
  onAnswer,
  onUseFiftyFifty,
  onUseChange,
  onUseDoubleChance,
  onUseRefillTime,
}: GameUIProps) {
  const { t } = useTranslation();
  return (
    <div className="h-full w-full flex items-center justify-center p-0 bg-[#0F172A] text-white overflow-hidden">
      <div className="relative -top-4 w-full max-w-9xl flex flex-row gap-8 items-center justify-center px-6">

        {/* Lateral HUD Column (Left) */}
        <div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-left-4 duration-700">
          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">{t("play.time")}</span>

          {/* Vertical Progress Bar */}
          <div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
            <div
              className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
              style={{ height: `${(time / 60) * 100}%` }}
            ></div>
            <div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
          </div>

          <div className="flex flex-col items-center gap-1">
            <span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">{t("play.left")}</span>
            <span className="text-4xl font-black text-[#FACC15] tracking-tighter">{time}</span>
          </div>
        </div>

        {/* Question Area (Center) */}
        <div className="flex-1 max-w-6xl flex flex-col gap-8 animate-in fade-in slide-in-from-bottom-4 duration-700">
          <div className="flex flex-col gap-2">
            <span className="text-[#FACC15] font-black tracking-widest text-sm text-center uppercase md:text-left">{t("play.questions")} {question?.number} {t("play.of_15")}</span>
            <h2 className={`font-black leading-tight text-center md:text-left ${
              (question?.text?.length ?? 0) > 150 ? "text-xl md:text-2xl" : "text-2xl md:text-3xl"
            }`}>
              {question?.text}
            </h2>
          </div>

          <div className="grid grid-cols-1 gap-4">
            {question?.options.map((option, i) => {
              const status = optionStatuses[option] || "none";

              return (
                <button
                  key={i}
                  onClick={() => onAnswer(option)}
                  disabled={isLocked || status !== "none"}
                  className={`group relative flex items-center p-6 border-2 transition-all duration-300 text-left overflow-hidden shadow-xl rounded-2xl
                    ${status !== "none"
                      ? (status === "yellow"
                        ? "bg-[#FACC15]/10 border-[#FACC15]"
                        : (status === "green" ? "bg-green-500/20 border-green-500" : "bg-red-500/20 border-red-500"))
                      : `bg-[#1E293B] border-transparent ${!isLocked ? "hover:bg-[#334155] hover:border-[#FACC15]/40" : ""}`}
                    ${isLocked ? "cursor-default" : "cursor-pointer"}
                  `}
                >
                  <span className={`w-10 h-10 rounded-xl font-black flex items-center justify-center transition-all duration-300 shadow-inner
                    ${status !== "none"
                      ? (status === "yellow"
                        ? "bg-[#FACC15] text-[#0F172A]"
                        : (status === "green" ? "bg-green-500 text-white" : "bg-red-500 text-white"))
                      : `bg-[#0F172A] text-[#FACC15] ${!isLocked ? "group-hover:bg-[#FACC15] group-hover:text-[#0F172A]" : ""}`}
                  `}>
                    {String.fromCharCode(65 + i)}
                  </span>
                  <span className={`ml-6 font-bold text-xl transition-colors
                    ${status !== "none"
                      ? (status === "yellow" ? "text-white" : (status === "green" ? "text-green-400" : "text-red-400"))
                      : ""}
                  `}>
                    {option}
                  </span>
                </button>
              );
            })}
          </div>
        </div>

        {/* Lifelines Column (Far Right) */}
        <div className="flex flex-col items-center gap-4 bg-[#1E293B] p-6 rounded-3xl border border-[#334155] shadow-2xl w-24 shrink-0 animate-in fade-in slide-in-from-right-4 duration-1000">
          <span className="text-[8px] font-black text-[#475569] uppercase tracking-[0.2em] mb-2">{t("singleplayer.lifelines")}</span>

          <button
            onClick={onUseFiftyFifty}
            disabled={lifelines.fiftyFifty || isLocked}
            className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
              ${lifelines.fiftyFifty || isLocked
                ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"}
            `}
            title="50:50"
          >
            🌓
          </button>

          <button
            onClick={onUseChange}
            disabled={lifelines.change || isLocked}
            className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
              ${lifelines.change || isLocked
                ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"}
                `}
            title={t("lifelines.change_question")}
          >
            ⏭️
          </button>

          <button
            onClick={onUseDoubleChance}
            disabled={lifelines.doubleChance || isLocked}
            className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
              ${lifelines.doubleChance || isLocked
                ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"}
                `}
            title={t("lifelines.double_chance")}
          >
            🛡️
          </button>

          <button
            onClick={onUseRefillTime}
            disabled={lifelines.refillTime || isLocked}
            className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
              ${lifelines.refillTime || isLocked
                ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"}
            `}
            title={t("lifelines.time_refill")}
          >
            ⏳
          </button>
        </div>

        {/* Lateral HUD Column (Right) */}
        <div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-right-4 duration-700">
          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">{t("leaderboard.streak")}</span>

          {/* Vertical Progress Bar */}
          <div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
            <div
              className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
              style={{ height: `${((question?.number || 1) - 1) / 15 * 100}%` }}
            ></div>
            <div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
          </div>

          <div className="flex flex-col items-center gap-1">
            <span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">{t("singleplayer.current")}</span>
            <span className="text-4xl font-black text-[#FACC15] tracking-tighter">{question ? question.number - 1 : 0}</span>
          </div>
        </div>
      </div>
    </div>
  );
}
