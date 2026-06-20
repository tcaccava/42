"use client";

import { useTranslation } from "@/hooks/useTranslation";

export default function PrivacyPolicy() {
  const { t } = useTranslation();

  const sections = [
    { title: t("privacy.s1_title"), body: t("privacy.s1_body") },
    { title: t("privacy.s2_title"), body: t("privacy.s2_body") },
    { title: t("privacy.s3_title"), body: t("privacy.s3_body") },
    { title: t("privacy.s4_title"), body: t("privacy.s4_body") },
    { title: t("privacy.s5_title"), body: t("privacy.s5_body") },
    { title: t("privacy.s6_title"), body: t("privacy.s6_body") },
    { title: t("privacy.s7_title"), body: t("privacy.s7_body") },
    { title: t("privacy.s8_title"), body: t("privacy.s8_body") },
    { title: t("privacy.s9_title"), body: t("privacy.s9_body") },
  ];

  return (
    <div className="h-full flex flex-col bg-[#0F172A] text-[#F8FAFC] font-sans">
      <main className="flex-1 w-full max-w-4xl mx-auto px-6 py-12 text-left">
        <h1 className="text-4xl font-black mb-8 text-[#FACC15]">{t("privacy.title")}</h1>
        <p className="text-[#94A3B8] max-w-2xl whitespace-pre-line mb-8">
          {t("privacy.toc")}
        </p>
        {sections.map((section, idx) => (
          <div key={idx} className="mb-8">
            <h2 className="font-bold mb-2">{section.title}</h2>
            <p>{section.body}</p>
          </div>
        ))}
      </main>
    </div>
  );
}
