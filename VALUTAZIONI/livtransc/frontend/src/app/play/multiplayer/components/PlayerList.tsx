"use client";
import { useTranslation } from "@/hooks/useTranslation";

interface PlayerInfo {
  user_id: number;
  display_name: string;
  connected: boolean;
}

interface PlayerListProps {
  players: PlayerInfo[];
  spectatorCount: number;
}

export default function PlayerList({ players, spectatorCount }: PlayerListProps) {
  const { t } = useTranslation();
  return (
    <div className="bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155] flex flex-col gap-4">
      <div className="flex justify-between items-center border-b border-[#334155] pb-3">
        <div className="flex items-center gap-3">
          <h3 className="text-[#FACC15] font-black text-[10px] uppercase tracking-[0.2em]">
            {t("lobby.players")}
          </h3>
          <span className="bg-[#FACC15] text-[#0F172A] text-[9px] font-black px-2 py-0.5 rounded-full uppercase tracking-tighter">
            {players.length} {t("lobby.joined")}
          </span>
          {spectatorCount > 0 && (
            <span className="bg-[#334155] text-[#94A3B8] text-[9px] font-black px-2 py-0.5 rounded-full uppercase tracking-tighter">
              👁 {spectatorCount} {t("lobby.watching")}
            </span>
          )}
        </div>
      </div>

      <div className="flex-1 flex flex-col gap-2 min-h-[150px]">
        {players.length === 0 ? (
          <div className="flex-1 flex flex-col items-center justify-center gap-4 text-[#475569]">
            <div className="w-10 h-10 rounded-full border-2 border-dashed border-[#334155] animate-spin border-t-[#FACC15]" />
            <span className="text-[10px] font-bold uppercase tracking-[0.2em] animate-pulse">
              {t("lobby.waiting")}
            </span>
          </div>
        ) : (
          players.map((player, idx) => (
            <div
              key={player.user_id}
              className={`flex items-center gap-3 p-3 rounded-xl border animate-in fade-in slide-in-from-right-2 duration-300 ${
                player.connected
                  ? "bg-[#1E293B] border-[#334155]"
                  : "bg-[#1E293B]/50 border-[#334155]/50 opacity-60"
              }`}
            >
              <div
                className={`w-8 h-8 rounded-lg flex items-center justify-center font-black text-xs ${
                  player.connected
                    ? "bg-gradient-to-br from-[#FACC15] to-[#EAB308] text-[#0F172A]"
                    : "bg-[#334155] text-[#94A3B8]"
                }`}
              >
                {idx + 1}
              </div>
              <span className={`font-bold text-sm ${player.connected ? "text-[#F8FAFC]" : "text-[#475569]"}`}>
                {player.display_name}
              </span>
              {!player.connected && (
                <span className="ml-auto text-[8px] font-black text-red-400 uppercase tracking-widest">
                  {t("lobby.disconnected")}
                </span>
              )}
            </div>
          ))
        )}
      </div>
    </div>
  );
}