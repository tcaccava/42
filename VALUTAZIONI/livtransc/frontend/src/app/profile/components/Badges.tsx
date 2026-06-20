"use client";
import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";

interface Badge {
  image: string;
  name: string;
  label: string;
}

interface BadgesProps {
  badges: Badge[];
  showViewAll?: boolean;
}

export default function Badges({ badges, showViewAll = true }: BadgesProps) {
  const { t } = useTranslation();

  return (
    <div className="w-full">
      <div className="flex justify-between items-center mb-2">
        <span className="block text-lg font-bold mb-2">🏅 {t("profile.badges_title")}</span>
        {showViewAll && (
          <Link href="/profile/badges">
            <button className="text-sm font-semibold text-[#FACC15] hover:underline transition cursor-pointer">{t("profile.view_all")}</button>
          </Link>
        )}
      </div>
      <div className="bg-[#1E293B] rounded-xl shadow p-6 flex flex-col gap-2">
        <div className="grid grid-cols-[repeat(auto-fill,minmax(6rem,1fr))] gap-4">
          {badges.map((b, i) => (
            <div
              key={i}
              className="group relative flex flex-col items-center justify-center w-24 rounded-2xl bg-[#1E293B] px-3 py-4 cursor-pointer transition hover:border-[#FACC15]"
            >
              {/* Emoji */}
              <span className="text-4xl mb-2">{b.image}</span>

              {/* Nome */}
              <span className="text-xs font-bold text-[#F8FAFC] text-center leading-tight">
                {t(b.name)}
              </span>

              {/* Tooltip on hover */}
              <div className="absolute bottom-full mb-2 left-1/2 -translate-x-1/2 opacity-0 group-hover:opacity-100 transition pointer-events-none z-10">
                <div className="bg-[#FACC15] text-[#0F172A] text-xs font-bold px-3 py-1 rounded-lg whitespace-nowrap shadow-lg">
                  {t(b.label)}
                </div>
                <div className="w-2 h-2 bg-[#FACC15] rotate-45 mx-auto -mt-1" />
              </div>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}
