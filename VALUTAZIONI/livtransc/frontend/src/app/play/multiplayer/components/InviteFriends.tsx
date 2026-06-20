"use client";

import { useState } from "react";
import { useTranslation } from "@/hooks/useTranslation";

interface Friend {
  name: string;
  status: string;
  avatar: string;
}

interface InviteFriendsButtonProps {
  username: string;
  roomCode: string;
}

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

export default function InviteFriendsButton({ username, roomCode }: InviteFriendsButtonProps) {
  const { t } = useTranslation();
  const [isInviteModalOpen, setIsInviteModalOpen] = useState(false);
  const [friends, setFriends] = useState<Friend[]>([]);
  const [invitedFriends, setInvitedFriends] = useState<string[]>([]);

const fetchFriends = async () => {
    try {
      const res = await fetch(`${API_BASE}/api/friends`, {
        method: 'GET',
        headers: { 'Content-Type': 'application/json' },
        credentials: 'include',
      });
      if (res.ok) {
        const data = await res.json();
        console.log("Fetched friends:", data);
        if (data.friends && data.friends.accepted) {
          const formattedFriends = data.friends.accepted.map((f: any) => {
            return {
              // Ensure we fallback to a safe string if partner is somehow still null
              name: f.username || "Unknown Player", 
              status: f.online ? "online" : "offline",
              avatar: f.emoji || "👤",
            };
          });
          setFriends(formattedFriends);
        }
      }
    } catch (err) {
      console.error("Error fetching friends:", err);
    }
  };

  const handleInviteFriends = () => {
    fetchFriends();
    setIsInviteModalOpen(true);
  };

  const sendInvite = (friendName: string) => {
    if (invitedFriends.includes(friendName)) return;
    console.log("Inviting friend:", friendName, "to room", roomCode);
    fetch(`${API_BASE}/api/invite_player`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ roomcode: roomCode, friend_name: friendName }),
      credentials: 'include',
    }).then(res => res.json());
    setInvitedFriends(prev => [...prev, friendName]);
  };

  return (
    <>
      <button
        onClick={handleInviteFriends}
        className="w-full py-3 rounded-xl font-black text-xs uppercase tracking-widest bg-[#1E293B] border border-[#334155] text-[#FACC15] hover:bg-[#FACC15]/10 transition-all cursor-pointer"
      >
        + {t("matches.invite_friends")}
      </button>

      {isInviteModalOpen && (
        <div className="fixed inset-0 z-50 flex items-center justify-center p-4 animate-in fade-in duration-300">
          <div
            className="absolute inset-0 bg-black/60 backdrop-blur-md"
            onClick={() => setIsInviteModalOpen(false)}
          />
          <div className="relative w-full max-w-md bg-[#1E293B] border border-[#334155] rounded-3xl p-8 shadow-2xl flex flex-col gap-6 animate-in zoom-in-95 duration-300">
            <div className="flex justify-between items-center border-b border-[#334155] pb-4">
              <div className="flex flex-col">
                <h2 className="text-xl font-black text-[#FACC15] tracking-tighter">{t("matches.invite_friends")}</h2>
                <p className="text-[#94A3B8] text-[10px] font-bold uppercase tracking-widest">{t("matches.online_friends")}</p>
              </div>
              <button
                onClick={() => setIsInviteModalOpen(false)}
                className="text-[#475569] hover:text-white transition-colors p-2"
              >
                ✕
              </button>
            </div>

            <div className="flex flex-col gap-3 max-h-[300px] overflow-y-auto pr-2 custom-scrollbar">
              {friends.length === 0 ? (
                <div className="flex items-center justify-center p-4 text-[#94A3B8] font-bold text-sm">
                  {t("friends.no_friends")}
                </div>
              ) : (
                friends.map((friend) => (
                  <div key={friend.name} className="flex items-center justify-between p-3 bg-[#0F172A]/50 rounded-2xl border border-[#334155] group transition-all hover:border-[#FACC15]/30">
                    <div className="flex items-center gap-3">
                      <div className="w-10 h-10 rounded-xl bg-[#1E293B] border border-[#334155] flex items-center justify-center text-xl">
                        {friend.avatar}
                      </div>
                      <div className="flex flex-col">
                        <span className="font-bold text-sm text-[#F8FAFC]">{friend.name}</span>
                        <div className="flex items-center gap-1.5">
                          <div className={`w-1.5 h-1.5 rounded-full ${friend.status === "online" ? "bg-green-500 animate-pulse" : "bg-[#475569]"}`} />
                          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-wider">{friend.status}</span>
                        </div>
                      </div>
                    </div>
                    <button
                      onClick={() => sendInvite(friend.name)}
                      disabled={friend.status !== "online" || invitedFriends.includes(friend.name)}
                      className={`px-4 py-1.5 rounded-lg font-black text-[10px] uppercase tracking-widest transition-all
                        ${invitedFriends.includes(friend.name)
                          ? "bg-green-500/20 text-green-500 border border-green-500/30 cursor-default"
                          : friend.status === "online"
                            ? "bg-[#FACC15] text-[#0F172A] hover:scale-105 active:scale-95 cursor-pointer"
                            : "bg-slate-700 text-slate-500 opacity-50 cursor-not-allowed"
                        }
                      `}
                    >
                      {invitedFriends.includes(friend.name) ? t("matches.sent") : t("matches.invite")}
                    </button>
                  </div>
                ))
              )}
            </div>

            <button
              onClick={() => setIsInviteModalOpen(false)}
              className="w-full py-3 rounded-xl bg-[#334155] text-white font-black text-xs uppercase tracking-widest hover:bg-[#475569] transition-all"
            >
              {t("matches.done")}
            </button>
          </div>
        </div>
      )}
    </>
  );
}