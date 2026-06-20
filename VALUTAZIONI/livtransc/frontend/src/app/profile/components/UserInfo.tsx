"use client";
import { useState } from "react";
import { useTranslation } from "@/hooks/useTranslation";

interface UserInfoProps {
  username: string | null;
  level: number;
  xp: number;
  xpMax: number;
  API_BASE: string;
  dispatchNavbarUpdate: () => void;
}

export default function UserInfo({ username, level, xp, xpMax, API_BASE, dispatchNavbarUpdate }: UserInfoProps) {
  const { t } = useTranslation();
  const [isEditingUsername, setIsEditingUsername] = useState(false);
  const [newUsername, setNewUsername] = useState("");

  const handleUsernameChange = async () => {
    if (!newUsername.trim() || newUsername === username) {
      setIsEditingUsername(false);
      return;
    }
    try {
      const response = await fetch(`${API_BASE}/api/change_username`, {
        method: 'POST',
        body: JSON.stringify({ username: newUsername }),
        credentials: 'include'
      });

      const json: any = await response.json();
      if (!response.ok) {
        alert(t(json?.error));
        return;
      }

      localStorage.setItem('username', newUsername);
      dispatchNavbarUpdate();
      setIsEditingUsername(false);
    } catch (err) {
      console.error("Error updating username:", err);
    }
  };

  return (
    <div className="flex flex-col gap-2 items-start">
      {isEditingUsername ? (
        <div className="flex items-center gap-2">
          <input
            type="text"
            value={newUsername}
            onChange={(e) => setNewUsername(e.target.value)}
            className="bg-[#1E293B] border border-[#FACC15] rounded px-2 py-1 text-base outline-none w-40"
            autoFocus
            onKeyDown={(e) => e.key === 'Enter' && handleUsernameChange()}
          />
          <button onClick={handleUsernameChange} className="hover:scale-110 transition cursor-pointer">✅</button>
          <button onClick={() => setIsEditingUsername(false)} className="hover:scale-110 transition cursor-pointer">❌</button>
        </div>
      ) : (
        <div className="flex items-center gap-2 group">
          <span className="text-2xl font-bold">{username}</span>
          <button
            onClick={() => {
              setNewUsername(username || "");
              setIsEditingUsername(true);
            }}
            className="text-sm opacity-50 group-hover:opacity-100 font-bold hover:scale-110 transition cursor-pointer"
            title={t("profile.edit_username")}
          >
            ✏️
          </button>
        </div>
      )}
      <span className="text-[#FACC15] font-semibold">{t("profile.level")} {level}</span>
      <span className="text-[#94A3B8]">XP: {xp} / {xpMax}</span>
    </div>
  );
}

