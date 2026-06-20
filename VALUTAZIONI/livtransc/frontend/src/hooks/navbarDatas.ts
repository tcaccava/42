import { useState, useEffect } from "react";
const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

let sessionPromise: Promise<any> | null = null;

export async function fetchUsername() {
  if (sessionPromise) return sessionPromise;

  sessionPromise = (async () => {
    try {
      const response = await fetch(
        `${API_BASE}/api/get_session`,
        { credentials: 'include' }
      );

      if (response.status === 401) {
        localStorage.removeItem("avatar");
        localStorage.removeItem("profile_picture");
        localStorage.removeItem("username");
        localStorage.removeItem("user_id");
        dispatchNavbarUpdate();
        return { error: "unauthorized", status: 401 };
      }


      const data = await response.json();


      if (!response.ok) {
        localStorage.removeItem("avatar");
        localStorage.removeItem("profile_picture");
        localStorage.removeItem("username");
        localStorage.removeItem("user_id");
        dispatchNavbarUpdate();
        document.cookie = "sessionid=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
        return data;
      }

      localStorage.setItem("avatar", data.emoji);
      if (data.profile_picture) {
        localStorage.setItem("profile_picture", data.profile_picture);
      } else {
        localStorage.removeItem("profile_picture");
      }
      localStorage.setItem('username', data.username);
      localStorage.setItem('user_id', data.user_id);
      dispatchNavbarUpdate();
      return data;
    } catch (err) {
      console.error('Search error:', err);
      return null;
    } finally {
      sessionPromise = null;
    }
  })();

  return sessionPromise;
}

export function dispatchNavbarUpdate() {
  if (typeof window !== 'undefined') {
    window.dispatchEvent(new Event('navbarUpdate'));
  }
}

export function useUsername() {
  const [username, setUsername] = useState<string | null>(null);

  useEffect(() => {
    const update = () => setUsername(localStorage.getItem("username"));

    if (!localStorage.getItem("username")) {
      fetchUsername();
    }

    update();
    window.addEventListener('navbarUpdate', update);
    window.addEventListener('storage', update);
    return () => {
      window.removeEventListener('navbarUpdate', update);
      window.removeEventListener('storage', update);
    };
  }, []);

  return username;
}

export function useAvatar() {
  const [avatar, setAvatar] = useState<string | null>(null);

  useEffect(() => {
    const update = () => setAvatar(localStorage.getItem("avatar"));

    if (!localStorage.getItem("avatar")) {
      fetchUsername();
    }

    update();
    window.addEventListener('navbarUpdate', update);
    window.addEventListener('storage', update);
    return () => {
      window.removeEventListener('navbarUpdate', update);
      window.removeEventListener('storage', update);
    };
  }, []);

  return avatar;
}

export function useProfilePicture() {
  const [profilePicture, setProfilePicture] = useState<string | null>(null);

  useEffect(() => {
    const update = () => setProfilePicture(localStorage.getItem("profile_picture"));

    if (!localStorage.getItem("profile_picture")) {
      fetchUsername();
    }

    update();
    window.addEventListener('navbarUpdate', update);
    window.addEventListener('storage', update);
    return () => {
      window.removeEventListener('navbarUpdate', update);
      window.removeEventListener('storage', update);
    };
  }, []);

  return profilePicture;
}
