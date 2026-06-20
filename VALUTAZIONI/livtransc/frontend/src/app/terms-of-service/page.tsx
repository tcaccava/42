"use client";

import { useTranslation } from "@/hooks/useTranslation";

export default function TermsOfService() {
  const { t } = useTranslation();

  const sections = [
    { title: t("tos.s1_title"), body: t("tos.s1_body") },
    { title: t("tos.s2_title"), body: t("tos.s2_body") },
    { title: t("tos.s3_title"), body: t("tos.s3_body") },
    { title: t("tos.s4_title"), body: t("tos.s4_body") },
    { title: t("tos.s5_title"), body: t("tos.s5_body") },
    { title: t("tos.s6_title"), body: t("tos.s6_body") },
    { title: t("tos.s7_title"), body: t("tos.s7_body") },
    { title: t("tos.s8_title"), body: t("tos.s8_body") },
    { title: t("tos.s9_title"), body: t("tos.s9_body") },
    { title: t("tos.s10_title"), body: t("tos.s10_body") },
  ];

  return (
    <div className="h-full flex flex-col bg-[#0F172A] text-[#F8FAFC] font-sans">
      <main className="flex-1 w-full max-w-4xl mx-auto px-6 py-12 text-left">
        <h1 className="text-4xl font-black mb-8 text-[#FACC15]">{t("tos.title")}</h1>
        <p className="text-[#94A3B8] max-w-2xl whitespace-pre-line mb-8">
          {t("tos.toc")}
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
