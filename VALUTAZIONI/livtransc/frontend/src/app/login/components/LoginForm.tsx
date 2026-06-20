"use client";
import { useState } from "react";
import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";

export default function LoginForm() {
  const { t } = useTranslation();
  const [showPassword, setShowPassword] = useState(false);
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [errors, setErrors] = useState<any>({});
  const [screenError, setScreenError] = useState("");
  const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';
  const OAUTH_REDIRECT_URI = process.env.NEXT_PUBLIC_OAUTH_REDIRECT_URI || `${API_BASE}/oauth/callback`;
  const INTRA_AUTH = `https://api.intra.42.fr/oauth/authorize?client_id=${process.env.NEXT_PUBLIC_42_CLIENT_ID}&redirect_uri=${encodeURIComponent(OAUTH_REDIRECT_URI)}&response_type=code`;

  function validateUsername(username: string) {
    return username.trim() === '';
  }

  function validatePassword(password: string) {
    return password.trim() === '';
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    const target = e.currentTarget as HTMLFormElement;
    const currentUsername = (target.elements.namedItem("username") as HTMLInputElement)?.value || "";
    const currentPassword = (target.elements.namedItem("password") as HTMLInputElement)?.value || "";

    // Sync values with React state
    setUsername(currentUsername);
    setPassword(currentPassword);

    const newErrors: any = {};
    setScreenError("");
    if (validateUsername(currentUsername)) {
      newErrors.username = t("login.error_username_empty");
    }
    if (validatePassword(currentPassword)) {
      newErrors.password = t("login.error_password_empty");
    }
    setErrors(newErrors);
    if (Object.keys(newErrors).length === 0) {
      try {
        const response = await fetch(`${API_BASE}/auth/login`, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ username: currentUsername.trim(), password: currentPassword }),
          credentials: 'include',
        });

        let data: any = null;
        const contentType = response.headers.get('content-type') || '';

        if (contentType.includes('application/json')) {
          data = await response.json();
        } else {
          const text = await response.text();
          data = { message: text };
        }

        if (response.ok) {
          localStorage.setItem('user_id', data.user_id);
          localStorage.setItem('username', data.username);
          localStorage.setItem('avatar', data.emoji);
          if (data.profile_picture) {
            localStorage.setItem('profile_picture', data.profile_picture);
          } else {
            localStorage.removeItem('profile_picture');
          }
          window.location.href = "/play";
        } else {
          setScreenError(
            data?.message ||
              (response.status === 401
                ? t("login.error_invalid_credentials")
                : t("login.error_generic"))
          );
        }
      } catch (error) {
        console.error('Network/server error:', error);
        setScreenError(t("login.error_network"));
      }
    }
  };

  return (
    <div className="w-full max-w-md bg-[#1E293B] rounded-xl shadow-lg p-8 mt-8">
      <h2 className="text-2xl font-bold mb-6 text-center">{t("login.title")}</h2>
      <form className="flex flex-col gap-4" onSubmit={handleSubmit} noValidate>
        <input
          type="text"
          name="username"
          placeholder={t("login.username_placeholder")}
          value={username}
          onChange={e => setUsername(e.target.value)}
          className={`px-4 py-3 rounded bg-[#0F172A] text-[#F8FAFC] placeholder-[#94A3B8] focus:outline-none focus:ring-2 focus:ring-[#3B82F6] ${errors.username ? 'ring-2 ring-red-500' : ''}`}
          required
        />
        {errors.username && <span className="text-red-400 text-xs">{errors.username}</span>}
        <input
          type={showPassword ? "text" : "password"}
          name="password"
          placeholder={t("login.password_placeholder")}
          value={password}
          onChange={e => setPassword(e.target.value)}
          className={`px-4 py-3 rounded bg-[#0F172A] text-[#F8FAFC] placeholder-[#94A3B8] focus:outline-none focus:ring-2 focus:ring-[#3B82F6] ${errors.password ? 'ring-2 ring-red-500' : ''}`}
          required
        />
        {errors.password && <span className="text-red-400 text-xs">{errors.password}</span>}
        <div className="flex items-center gap-2">
          <input
            id="show-password"
            type="checkbox"
            checked={showPassword}
            onChange={() => setShowPassword((v) => !v)}
            className="accent-[#3B82F6]"
          />
          <label htmlFor="show-password" className="text-[#94A3B8] text-sm select-none cursor-pointer">
            {t("login.show_password")}
          </label>
        </div>
        <button
          type="submit"
          className="mt-2 py-3 rounded-lg bg-[#FACC15] text-[#1E293B] font-bold text-lg hover:bg-[#ffe066] transition cursor-pointer"
        >
          {t("login.submit")}
        </button>
        <a
          href={INTRA_AUTH}
          className="mt-2 py-3 rounded-lg bg-[#111827] text-[#F8FAFC] font-bold text-lg hover:bg-[#0b1220] transition cursor-pointer text-center block"
        >
          {t("login.continue_with_42")}
        </a>
        {screenError && (
          <div className="mt-2 rounded bg-red-500/20 border border-red-500 px-3 py-2 text-sm text-red-300">
            {screenError}
          </div>
        )}
      </form>
      <p className="mt-4 text-center text-[#94A3B8] text-sm">
        {t("login.no_account")}{' '}
        <Link href="/signup" className="text-[#3B82F6] hover:underline">{t("login.register_link")}</Link>
      </p>
    </div>
  );
}
