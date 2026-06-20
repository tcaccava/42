"use client";

import { useTranslation } from "@/hooks/useTranslation";

export default function Home() {
  const { t } = useTranslation();

  return (
    <div className="flex-1 flex flex-col bg-[#0F172A] text-[#F8FAFC] font-sans">
      {/* Main Content */}
      <main className="flex flex-1 flex-col items-center justify-center text-center px-4">
        <h1 className="text-4xl sm:text-5xl font-extrabold mb-2 tracking-tight">🧠 {t("home.title")}</h1>
        <p className="text-xl sm:text-2xl text-[#94A3B8] mb-10">{t("home.subtitle")}</p>
        <div className="flex flex-col sm:flex-row gap-4 w-full max-w-md justify-center">
          <a
            href="/login"
            className="flex-1 py-3 rounded-lg bg-[#1E293B] text-[#F8FAFC] text-base font-semibold hover:bg-[#334155] transition"
          >
            {t("home.login")}
          </a>
          <a
            href="/signup"
            className="flex-1 py-3 rounded-lg border border-[#FACC15] text-[#FACC15] text-base font-semibold hover:bg-[#FACC15] hover:text-[#1E293B] transition"
          >
            {t("home.signup")}
          </a>
        </div>
      </main>
    </div>
  );
}
