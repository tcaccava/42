"use client";

import { useTranslation } from "@/hooks/useTranslation";

interface PauseOverlayProps {
  reason: "disconnect" | "manual";
  disconnectedPlayers: string[]; // display names
  isHost: boolean;
  onResume: () => void;
}

export default function PauseOverlay({
  reason,
  disconnectedPlayers,
  isHost,
  onResume,
}: PauseOverlayProps) {
  const { t } = useTranslation();

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-[#0F172A]/90 backdrop-blur-sm p-6">
      <div className="max-w-md w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center gap-6 animate-in fade-in fill-mode-both duration-500 slide-in-from-bottom-6">
        <div className="flex flex-col items-center gap-3">
          <span className="text-6xl">⏸</span>
          <h2 className="text-3xl font-black text-[#FACC15] tracking-tighter uppercase text-center">
            {t("pause.title")}
          </h2>
        </div>

        <div className="w-full flex flex-col gap-3 bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155]">
          {reason === "disconnect" ? (
            <>
              <p className="text-[#94A3B8] font-bold text-center">
                {t("pause.disconnect_message")}
              </p>
              {disconnectedPlayers.length > 0 && (
                <div className="flex flex-col items-center gap-2">
                  <span className="text-[#475569] font-black text-[10px] uppercase tracking-[0.3em]">
                    {disconnectedPlayers.length > 1
                      ? t("pause.disconnected_players")
                      : t("pause.disconnected_player")}
                  </span>
                  <div className="flex flex-wrap gap-2 justify-center">
                    {disconnectedPlayers.map((name) => (
                      <span
                        key={name}
                        className="px-3 py-1 bg-[#FACC15] text-[#0F172A] rounded-lg text-xs font-black uppercase tracking-wider"
                      >
                        {name}
                      </span>
                    ))}
                  </div>
                </div>
              )}
            </>
          ) : (
            <p className="text-[#94A3B8] font-bold text-center">
              {t("pause.manual_message")}
            </p>
          )}
        </div>

        <div className="w-full flex flex-col gap-3">
          {isHost ? (
            <button
              onClick={onResume}
              className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black text-xl hover:scale-[1.02] active:scale-[0.98] transition-all shadow-[0_0_30px_rgba(250,204,21,0.2)] cursor-pointer"
            >
              {t("pause.resume")}
            </button>
          ) : (
            <div className="w-full py-4 rounded-xl bg-[#0F172A] border border-[#334155] text-[#94A3B8] font-black text-sm uppercase tracking-widest flex items-center justify-center cursor-default text-center">
              {t("pause.waiting_for_host")}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
