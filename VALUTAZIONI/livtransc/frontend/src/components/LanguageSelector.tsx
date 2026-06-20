"use client";

import { useLanguageContext, Language } from "@/contexts/LanguageContext";

const LANGUAGE_OPTIONS: { code: Language; label: string }[] = [
  { code: "en", label: "English 🇬🇧" },
  { code: "it", label: "Italiano 🇮🇹" },
  { code: "es", label: "Español 🇪🇸" },
];

export default function LanguageSelector() {
  const { language, setLanguage } = useLanguageContext();

  return (
    <select
      value={language}
      onChange={(e) => setLanguage(e.target.value as Language)}
      className="bg-[#1E293B] border border-[#334155] text-white text-sm rounded px-2 py-1 focus:outline-none focus:ring-2 focus:ring-[#FACC15] focus:border-[#FACC15] cursor-pointer"
    >
      {LANGUAGE_OPTIONS.map((opt) => (
        <option key={opt.code} value={opt.code}>
          {opt.label}
        </option>
      ))}
    </select>
  );
}
