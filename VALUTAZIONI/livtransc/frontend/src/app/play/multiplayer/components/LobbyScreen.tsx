"use client";
import Link from "next/link";
import PlayerList from "./PlayerList";
import InviteFriendsButton from "./InviteFriends";
import { useTranslation } from "@/hooks/useTranslation";
import { copyToClipboard } from "@/lib/clipboard";

interface PlayerInfo {
  user_id: number;
  display_name: string;
  connected: boolean;
}

interface LobbyScreenProps {
  players: PlayerInfo[];
  spectatorCount: number;
  isHost: boolean;
  onStart: () => void;
  sessionId: string;
  username: string;
  roomCode: string;
}

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';

export default function LobbyScreen({
  players,
  spectatorCount,
  isHost,
  onStart,
  sessionId,
  username,
  roomCode,
}: LobbyScreenProps) {
  const canStart = isHost && players.length >= 2;
  const { t } = useTranslation();

  return (
    <div className="flex-1 w-full flex flex-col items-center justify-center bg-[#0F172A] text-white overflow-hidden p-0">
      <div className="max-w-4xl w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-5 shadow-2xl flex flex-col gap-5">
        <div className="flex flex-col gap-1 items-center">
          <span className="text-3xl">👥</span>
          <h1 className="text-3xl font-black text-[#FACC15] tracking-tighter uppercase">
            {t("play.multiplayer")}
          </h1>
        </div>

        <PlayerList
          players={players}
          spectatorCount={spectatorCount}
        />

        <div
          className="flex items-center justify-center gap-2 cursor-pointer"
          onClick={() => copyToClipboard(`${API_BASE}/play/multiplayer/${sessionId}`)}
        >
          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">
            Session Code:
          </span>
          <span className="text-sm font-black text-[#FACC15] tracking-wider bg-[#0F172A] px-3 py-1 rounded-lg border border-[#334155]">
            {sessionId}
          </span>
          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">
            {t("lobby.click_to_copy_link")}
          </span>
        </div>

        <div className="flex flex-col gap-3">

          {/* Bottone invite — visibile a tutti */}
          <InviteFriendsButton
            username={username}
            roomCode={roomCode}
          />

          {isHost ? (
            <button
              onClick={onStart}
              disabled={!canStart}
              className={`w-full py-3 rounded-xl font-black text-lg transition-all flex items-center justify-center
                ${
                  canStart
                    ? "bg-[#FACC15] text-[#0F172A] shadow-[0_0_20px_rgba(250,204,21,0.15)] hover:scale-[1.01] active:scale-[0.99] cursor-pointer"
                    : "bg-slate-700 text-slate-500 opacity-50 shadow-none cursor-default"
                }
              `}
            >
              {canStart ? t("lobby.start_game") : t("lobby.waiting_for_players")}
            </button>
          ) : (
            <div className="w-full py-3 rounded-xl font-black text-lg bg-slate-700 text-slate-500 opacity-50 shadow-none flex items-center justify-center cursor-default">
              {t("lobby.waiting_for_host")}
            </div>
          )}

          <Link
            href="/play"
            className="text-[#475569] hover:text-[#94A3B8] transition text-center text-[10px] font-bold uppercase tracking-widest"
          >
            ← {t("game.back_to_menu")}
          </Link>
        </div>
      </div>
    </div>
  );
}