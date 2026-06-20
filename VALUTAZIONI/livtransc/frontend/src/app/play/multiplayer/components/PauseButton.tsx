"use client";

import { useTranslation } from "@/hooks/useTranslation";

interface PauseButtonProps {
  onClick: () => void;
  disabled: boolean;
}

export default function PauseButton({ onClick, disabled }: PauseButtonProps) {
  const { t } = useTranslation();

  return (
    <button
      type="button"
      onClick={() => !disabled && onClick()}
      disabled={disabled}
      aria-label={t("pause.button_aria")}
      title={disabled ? t("pause.unavailable") : t("pause.button_aria")}
      className={`flex items-center justify-center gap-2 px-6 py-3 rounded-xl font-black text-lg tracking-tight transition-all
        ${
          disabled
            ? "bg-slate-700 text-slate-500 opacity-50 shadow-none cursor-default"
            : "bg-[#1E293B] text-[#FACC15] border-2 border-[#FACC15]/40 shadow-[0_0_20px_rgba(250,204,21,0.10)] hover:bg-[#334155] hover:border-[#FACC15] hover:scale-[1.01] active:scale-[0.99] cursor-pointer"
        }
      `}
    >
      <span className="text-xl leading-none" aria-hidden="true">
        ⏸
      </span>
      <span>{t("pause.button")}</span>
    </button>
  );
}
