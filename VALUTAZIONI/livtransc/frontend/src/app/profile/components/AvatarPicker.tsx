"use client";
import { useState, useEffect, useRef } from "react";
import { useTranslation } from "@/hooks/useTranslation";
import Link from 'next/link';

interface AvatarPickerProps {
  avatar: string;
  setAvatar: (avatar: string) => void;
  profilePicture: string | null;
  setProfilePicture: (profilePicture: string | null) => void;
  API_BASE: string;
  dispatchNavbarUpdate: () => void;
}

export default function AvatarPicker({ avatar, setAvatar, profilePicture, setProfilePicture, API_BASE, dispatchNavbarUpdate }: AvatarPickerProps) {
  const { t } = useTranslation();
  const [showPicker, setShowPicker] = useState(false);
  const pickerRef = useRef<HTMLDivElement>(null);
  const avatarRef = useRef<HTMLDivElement>(null);
  const fileInputRef = useRef<HTMLInputElement>(null);

  useEffect(() => {
    const handleClickOutside = (event: Event) => {
      // Ignora se il clic avviene sull'avatar stesso, l'onClick gestirà il toggle
      if (avatarRef.current && avatarRef.current.contains(event.target as Node)) {
        return;
      }
      if (pickerRef.current && !pickerRef.current.contains(event.target as Node)) {
        setShowPicker(false);
      }
    };
    if (showPicker) {
      document.addEventListener("click", handleClickOutside);
    }
    return () => {
      document.removeEventListener("click", handleClickOutside);
    };
  }, [showPicker]);

  const changeEmoji = async (emoji: string) => {
    if (!emoji.trim()) {
      return;
    }

    if (emoji === avatar) {
      setShowPicker(false);
      return;
    }

    try {
      const response = await fetch(
        `${API_BASE}/api/change_emoji`,
        {
          credentials: 'include',
          method: 'POST',
          body: JSON.stringify({ emoji }),
        }
      );

      if (!response.ok) throw new Error('Error changing emoji');

      setAvatar(emoji);
      localStorage.setItem('avatar', emoji);
      dispatchNavbarUpdate();
      setShowPicker(false);
    } catch (err) {
      console.error('Error changing emoji:', err);
    }
  };

  const uploadProfilePicture = async (file: File) => {
    const formData = new FormData();
    formData.append('profile_picture', file);

    try {
      const response = await fetch(`${API_BASE}/api/upload_profile_picture`, {
        credentials: 'include',
        method: 'POST',
        body: formData,
      });

      const data = await response.json();
      if (!response.ok) throw new Error(data?.error || 'Error uploading profile picture');

      setProfilePicture(data.profile_picture);
      if (data.profile_picture) {
        localStorage.setItem('profile_picture', data.profile_picture);
      } else {
        localStorage.removeItem('profile_picture');
      }
      dispatchNavbarUpdate();
      setShowPicker(false);
    } catch (err) {
      console.error('Error uploading profile picture:', err);
    } finally {
      if (fileInputRef.current) {
        fileInputRef.current.value = '';
      }
    }
  };

  const handleFileChange = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;
    await uploadProfilePicture(file);
  };

  const removeProfilePicture = async () => {
    try {
      const response = await fetch(`${API_BASE}/api/remove_profile_picture`, {
        credentials: 'include',
        method: 'POST',
      });

      const data = await response.json();
      if (!response.ok) throw new Error(data?.error || 'Error removing profile picture');

      setProfilePicture(null);
      localStorage.removeItem('profile_picture');
      dispatchNavbarUpdate();
      setShowPicker(false);
    } catch (err) {
      console.error('Error removing profile picture:', err);
    }
  };

  const emojiOptions = [
    '👤', '😃', '😎', '🦊', '🐱', '🐶', '🦄', '🐸', '🐵', '👽', '🤖', '👾', '🧑', '👩', '🧙', '🧛', '🧟', '🧞', '🧜', '🧚', '🧝', '🧔', '👩‍🦰', '👨‍🦰', '👩‍🦱', '👨‍🦲', '👩‍🦳', '👨‍🦳'
  ];

  return (
    <div className="flex flex-col items-center">
      <div
        ref={avatarRef}
        className={`w-32 h-32 rounded-full bg-[#334155] flex items-center justify-center text-5xl mb-2 border-4 border-[#FACC15] relative ${!showPicker ? 'cursor-pointer' : ''}`}
        onClick={() => setShowPicker(!showPicker)}
      >
        {profilePicture ? (
          <img src={profilePicture} alt="Profile avatar" className="w-full h-full rounded-full object-cover" />
        ) : (
          <span role="img" aria-label="avatar">{avatar}</span>
        )}
        {showPicker && (
          <div
            ref={pickerRef}
            className="absolute top-full left-1/2 -translate-x-1/2 mt-2 z-20 bg-[#1E293B] border border-[#FACC15] rounded-xl shadow-lg p-4 flex flex-wrap gap-2 w-64"
            style={{ boxShadow: '0 0 0 100vmax rgba(15, 23, 42, 0.25)' }}
            onClick={(e) => e.stopPropagation()}
          >
            {emojiOptions.map((em) => (
              <button
                key={em}
                className={`text-2xl p-2 rounded hover:bg-[#FACC15] hover:text-[#1E293B] transition cursor-pointer ${avatar === em ? 'bg-[#FACC15] text-[#1E293B]' : ''}`}
                onClick={(e) => {
                  e.stopPropagation();
                  changeEmoji(em);
                }}
              >{em}</button>
            ))}
            <input
              ref={fileInputRef}
              type="file"
              accept="image/png,image/jpeg,image/webp,image/gif"
              className="hidden"
              onChange={handleFileChange}
            />
            <button
              className="w-full mt-2 px-3 py-2 rounded bg-[#334155] text-white text-sm font-semibold hover:bg-[#475569] transition cursor-pointer"
              onClick={(e) => {
                e.stopPropagation();
                fileInputRef.current?.click();
              }}
            >
              {t("profile.upload_photo")}
            </button>
            {profilePicture && (
              <button
                className="w-full px-3 py-2 rounded bg-[#7F1D1D] text-white text-sm font-semibold hover:bg-[#991B1B] transition cursor-pointer"
                onClick={(e) => {
                  e.stopPropagation();
                  removeProfilePicture();
                }}
              >
                {t("profile.remove_photo")}
              </button>
            )}
          </div>
        )}
      </div>
      <Link href="/profile/friends">
        <button className="mt-2 px-4 py-1 rounded bg-[#FACC15] text-[#1E293B] font-bold text-sm hover:bg-[#ffe066] transition cursor-pointer uppercase">{t("friends.friends")}</button>
      </Link>
    </div>
  );
}
