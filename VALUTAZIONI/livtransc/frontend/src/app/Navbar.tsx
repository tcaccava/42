"use client";
import { useUsername, useAvatar, useProfilePicture } from "@/hooks/navbarDatas";
import Link from "next/link";
import { useEffect, useRef, useState } from "react";
import LanguageSelector from "@/components/LanguageSelector";
import { useTranslation } from "@/hooks/useTranslation";
import { usePathname } from "next/navigation";

function UserSection() {
  const { t } = useTranslation();
  const username = useUsername();
  const avatar = useAvatar();
  const profilePicture = useProfilePicture();
  const [showNotifications, setShowNotifications] = useState(false);
  const pickerRef = useRef<HTMLDivElement>(null);
  const bellRef = useRef<HTMLSpanElement>(null);
  const [notifications, setNotifications] = useState<any[]>([]);
  const [hasNewNotifications, setHasNewNotifications] = useState(false);
  const [unreadCount, setUnreadCount] = useState(0);
  const fetchRef = useRef<() => Promise<void>>(() => Promise.resolve());

  const API_BASE = process.env.NEXT_PUBLIC_API_URL!;
  const WS_BASE = (process.env.NEXT_PUBLIC_API_URL!).replace('https', 'wss');
  const pathname = usePathname();

  useEffect(() => {
    if (!username) return;

    // 1. Fetch historical notifications
    const fetchNotifications = async () => {
      try {
        const response = await fetch(`${API_BASE}/api/get_notifications`, { credentials: 'include' });
        const data = await response.json();
        if (data.success) {
          setNotifications(data.notifications);
          const pendingCount = data.notifications.length;
          setUnreadCount(pendingCount);
        }
      } catch (err) {
        console.error("Error fetching notifications:", err);
      }
    };

    fetchNotifications();
    fetchRef.current = fetchNotifications;

    // 2. WebSocket setup
    const socket = new WebSocket(`${WS_BASE}/ws/notifications/`);

    socket.onmessage = (event) => {
      const data = JSON.parse(event.data);
      if (data.type === 'notification') {
        setHasNewNotifications(true);
      }
    };

    socket.onclose = () => console.log("Notification socket closed");

    return () => {
      socket.close();
    };
  }, [username]);

  const handleAction = async (requestId: number, notificationId: number, type: 'accept' | 'reject') => {
    try {
      const endpoint = type === 'accept' ? '/api/accept_request' : '/api/remove_friend';
      const method = type === 'accept' ? 'POST' : 'DELETE';
      const response = await fetch(`${API_BASE}${endpoint}`, {
        method,
        credentials: 'include',
        body: JSON.stringify({ request_id: requestId })
      });
      if (response.ok) {
        setNotifications(prev => prev.filter(n => n.id !== notificationId));
        if (unreadCount > 0) setUnreadCount(prev => prev - 1);
      }
    } catch (err) {
      console.error("Action error:", err);
    }
  };

  const handleInvite = async (action: 'accept' | 'reject' | 'spectate', eventId?: string, id?: number) => {
    if (id != null && id >= 0) {
      try {
        await fetch(`${API_BASE}/api/delete_notification`, {
          method: 'DELETE',
          credentials: 'include',
          body: JSON.stringify({ notification_id: id })
        });
      } catch (err) {
        console.error("Error deleting notification:", err);
      }
      setNotifications(prev => prev.filter(item => item.id !== id));
      if (unreadCount > 0) setUnreadCount(prev => prev - 1);
    }
    if (action === 'accept' && eventId != null) {
      window.location.href = `/play/multiplayer/${eventId}`;
    }
    else if (action === 'spectate' && eventId != null) {
      window.location.href = `/play/multiplayer/${eventId}?spectator=true`;
    }
  };

  useEffect(() => {
    const handleClickOutside = (event: Event) => {
      if (bellRef.current && bellRef.current.contains(event.target as Node)) {
        return;
      }
      if (pickerRef.current && !pickerRef.current.contains(event.target as Node)) {
        setShowNotifications(false);
      }
    };
    if (showNotifications) {
      document.addEventListener("click", handleClickOutside);
    }
    return () => {
      document.removeEventListener("click", handleClickOutside);
    };
  }, [showNotifications]);

  return (
    <>
      <div className="flex gap-[10vw] items-center">
        <Link
          href="/play"
          aria-current={pathname?.startsWith('/play') ? 'page' : undefined}
          className={`${pathname?.startsWith('/play') ? 'text-white' : 'text-[#94A3B8]'} text-sm font-medium cursor-pointer transform transition-transform duration-150 hover:scale-110`}
        >
          {t("navbar.play")}
        </Link>
        <Link
          href="/profile"
          aria-current={pathname?.startsWith('/profile') ? 'page' : undefined}
          className={`${pathname?.startsWith('/profile') ? 'text-white' : 'text-[#94A3B8]'} text-sm font-medium cursor-pointer transform transition-transform duration-150 hover:scale-110`}
        >
          {t("navbar.profile")}
        </Link>
        <Link
          href="/leaderboard"
          aria-current={pathname?.startsWith('/leaderboard') ? 'page' : undefined}
          className={`${pathname?.startsWith('/leaderboard') ? 'text-white' : 'text-[#94A3B8]'} text-sm font-medium cursor-pointer transform transition-transform duration-150 hover:scale-110`}
        >
          {t("navbar.leaderboard")}
        </Link>
      </div>
      <div className="flex items-center gap-6">
        <LanguageSelector />
        <div className="relative">
          <span
            ref={bellRef}
            onClick={() => {
              if (!showNotifications) {
                fetchRef.current();
              }
              setShowNotifications(!showNotifications);
              if (!showNotifications) {
                setHasNewNotifications(false);
              }
            }}
            className="text-xl cursor-pointer relative"
            title="Notifications"
          >
            🔔
            {hasNewNotifications && (
              <span className="absolute top-0 right-0 w-2 h-2 bg-red-500 rounded-full border border-[#0F172A]"></span>
            )}
          </span>

          {showNotifications && (
            <div
              ref={pickerRef}
              className="absolute top-full left-1/2 -translate-x-1/2 mt-3 z-50 bg-[#1E293B] border border-[#94A3B8] rounded-xl shadow-xl p-4 w-64"
              style={{ boxShadow: '0 0 0 100vmax rgba(15, 23, 42, 0.25)' }}
              onClick={(e) => e.stopPropagation()}
            >
              <div className="text-sm font-semibold mb-2 border-b border-[#334155] pb-2 text-white flex justify-between items-center">
                {t("navbar.notifications")}
                {unreadCount > 0 && <span className="text-[10px] bg-blue-600 px-1.5 py-0.5 rounded-full">{unreadCount}</span>}
              </div>
              <div className="max-h-60 overflow-y-auto pr-1 custom-scrollbar">
                {notifications.length > 0 ? (
                  notifications.map((n, i) => {
                    const content = n.status === 'badge' ? (
                        <span className="font-semibold text-yellow-400">
                          🏅 {t("navbar.badgeunlocked")} {t(n.text)}
                        </span>
                      ) : n.status === 'pending' ? (
                        `${n.text} ${t("navbar.friend_request")}`
                      ) : n.status === 'invite_to_game' ? (
                        `🎮 ${n.text} ${t("navbar.invite_to_game")}`
                      ) : n.status === 'accepted' ? (
                        `${n.text} ${t("navbar.friend_request_accepted")}`
                      ) : n.text;

                    const href = n.status === 'pending'
                        ? "/profile/friends"
                        : n.status === 'badge'
                          ? "/profile/badges"
                          : null;

                    return (
                      <div key={i} className={`p-2 mb-1 rounded text-xs transition-colors ${n.status === 'pending' || n.status === 'invite_to_game' ? 'bg-[#334155] border-l-2 border-blue-500' : 'bg-[#1E293B] opacity-70'}`}>
                        {href ? (
                          <Link href={href} className="text-[#F8FAFC] hover:text-blue-400 transition-colors block mb-1">
                            {content}
                          </Link>
                        ) : (
                          <span className="block mb-1 text-[#F8FAFC]">{content}</span>
                        )}
                        {n.status === 'pending' && n.event_id && (
                        <div className="flex gap-2 mt-2">
                          <button
                            onClick={() => handleAction(Number(n.event_id), n.id ?? -1, 'accept')}
                            className="flex-1 py-1 bg-green-600 hover:bg-green-700 rounded text-[10px] font-bold text-white transition cursor-pointer"
                          >
                            {t("navbar.accept")}
                          </button>
                          <button
                            onClick={() => handleAction(Number(n.event_id), n.id ?? -1, 'reject')}
                            className="flex-1 py-1 bg-red-600 hover:bg-red-700 rounded text-[10px] font-bold text-white transition cursor-pointer"
                          >
                            {t("navbar.reject")}
                          </button>
                        </div>
                      )}
                      {n.status === 'invite_to_game' && n.event_id && (
                        <div className="flex gap-2 mt-2">
                          <button
                            onClick={() => handleInvite('accept', n.event_id, n.id ?? -1)}
                            className="flex-1 py-1 bg-green-600 hover:bg-green-700 rounded text-[10px] font-bold text-white transition cursor-pointer"
                          >
                            {t("navbar.accept")}
                          </button>
                          <button
                            onClick={() => handleInvite('reject', undefined, n.id ?? -1)}
                            className="flex-1 py-1 bg-red-600 hover:bg-red-700 rounded text-[10px] font-bold text-white transition cursor-pointer"
                          >
                            {t("navbar.reject")}
                          </button>
                          <button
                            onClick={() => handleInvite('spectate', n.event_id, n.id ?? -1)}
                            className="flex items-center justify-center h-8 w-8 bg-blue-600 hover:bg-blue-700 rounded text-base transition cursor-pointer"
                          >
                            👁️
                          </button>
                        </div>
                      )}
                    </div>
                  );
                })
                ) : (
                  <p className="text-[#94A3B8] text-xs py-2">{t("navbar.no_new_notifications")}</p>
                )}
              </div>
            </div>
          )}
        </div>
        <Link href="/profile" className="flex items-center gap-2 text-[#F8FAFC] text-sm font-semibold bg-[#1E293B] px-3 py-1 rounded-lg">
          {profilePicture ? (
            <img
              src={profilePicture}
              alt="Profile avatar"
              className="w-7 h-7 rounded-full object-cover border border-[#334155]"
            />
          ) : (
            <span className="text-lg">{avatar || "👤"}</span>
          )}
          {username || "Loading..."}
        </Link>
      </div>
    </>
  );
}

export default function Navbar({ isLoggedIn }: { isLoggedIn: boolean }) {
  const { t } = useTranslation();
  return (
    <nav className={isLoggedIn ? "flex items-center justify-between px-8 py-1.5 border-b border-[#1E293B] bg-[#0F172A]" : "flex items-center justify-between px-8 py-1.5"}>
      <div className="flex items-center gap-2">
        <Link
          href={isLoggedIn ? "/play" : "/"}
          className={
            isLoggedIn
              ? "text-lg font-bold tracking-tight text-white"
              : "text-lg font-bold tracking-tight"
          }
        >
          🧠 {t("navbar.trivia_master")}
        </Link>
      </div>
      {isLoggedIn ? (
        <UserSection />
      ) : (
        <div className="flex items-center gap-6">
          <LanguageSelector />
          <Link href="/login" className="text-[#3B82F6] font-semibold px-4 py-1.5 rounded hover:bg-[#1E293B] transition text-sm">{t("navbar.login")}</Link>
        </div>
      )}
    </nav>
  );
}
