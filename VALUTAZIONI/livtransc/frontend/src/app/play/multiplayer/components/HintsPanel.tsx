"use client";
import { useTranslation } from "@/hooks/useTranslation";

interface HintsPanelProps {
  hints: {
    fifty_fifty: boolean; // true = available, false = used
    double_chance: boolean;
    scrying: boolean;
  };
  onUseHint: (hintName: string) => void;
  isLocked: boolean; // true when answer submitted or not interactable
  readOnly: boolean; // true during WAITING/RANKING/RESULTS phases
  isSpectator: boolean; // if true, component returns null
}

export default function HintsPanel({
  hints,
  onUseHint,
  isLocked,
  readOnly,
  isSpectator,
}: HintsPanelProps) {
  if (isSpectator) return null;
  const { t } = useTranslation();

  const disabled = isLocked || readOnly;

  return (
    <div className="flex flex-col items-center gap-4 bg-[#1E293B] p-6 rounded-3xl border border-[#334155] shadow-2xl w-24 shrink-0 animate-in fade-in slide-in-from-right-4 duration-1000">
      <span className="text-[8px] font-black text-[#475569] uppercase tracking-[0.2em] mb-2">
        {t("singleplayer.lifelines")}
      </span>

      <button
        onClick={() => onUseHint("fifty_fifty")}
        disabled={!hints.fifty_fifty || disabled}
        className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
          ${
            !hints.fifty_fifty
              ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
              : disabled
                ? "bg-[#0F172A] border-[#334155] opacity-60 cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"
          }
        `}
        title="50:50"
      >
        🌓
      </button>

      <button
        onClick={() => onUseHint("double_chance")}
        disabled={!hints.double_chance || disabled}
        className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
          ${
            !hints.double_chance
              ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
              : disabled
                ? "bg-[#0F172A] border-[#334155] opacity-60 cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"
          }
        `}
        title={t("lifelines.double_chance")}
      >
        🛡️
      </button>

      <button
        onClick={() => onUseHint("scrying")}
        disabled={!hints.scrying || disabled}
        className={`w-12 h-12 rounded-xl flex items-center justify-center text-xl border-2 transition-all duration-300
          ${
            !hints.scrying
              ? "bg-slate-800 border-slate-700 opacity-40 grayscale cursor-not-allowed"
              : disabled
                ? "bg-[#0F172A] border-[#334155] opacity-60 cursor-not-allowed"
                : "bg-[#0F172A] border-[#334155] hover:border-[#FACC15] hover:scale-110 active:scale-95 cursor-pointer shadow-lg"
          }
        `}
        title={t("lifelines.scrying")}
      >
        👁️
      </button>
    </div>
  );
}
