"use client";
import { useTranslation } from "@/hooks/useTranslation";

interface LogoutButtonProps {
  onLogout: () => void;
}

export default function LogoutButton({ onLogout }: LogoutButtonProps) {
  const { t } = useTranslation();

  return (
    <div className="w-full flex justify-end mb-8">
      <button onClick={onLogout} className="px-6 py-2 rounded-lg bg-red-500 text-white font-bold hover:bg-red-600 transition cursor-pointer">{t("profile.logout")}</button>
    </div>
  );
}
