"use client";

import { useTranslation } from "@/hooks/useTranslation";

interface StatisticsProps {
  games: number;
  sp_victories: number;
  mp_victories: number;
  xp: number;
  correct_answers: number;
  wrong_answers: number;
  lifeline_used: number;
  sp_average_score: number;
  mp_average_score: number;
  highest_streak: number;
}

export default function Statistics({
  games = 0,
  sp_victories = 0,
  mp_victories = 0,
  xp = 0,
  correct_answers = 0,
  wrong_answers = 0,
  lifeline_used = 0,
  sp_average_score = 0,
  mp_average_score = 0,
  highest_streak = 0,
}: StatisticsProps) {
  const { t } = useTranslation();
  const accuracy =
    correct_answers + wrong_answers > 0
      ? Math.round((correct_answers / (correct_answers + wrong_answers)) * 100)
      : 0;

  return (
    <div className="w-full">
      <span className="block text-lg font-bold mb-2">📊 {t("profile.statistics_title")}</span>
      <div className="bg-[#1E293B] rounded-xl shadow p-6 flex flex-col gap-6">

        {/* Overview */}
        <div>
          <p className="text-xs font-medium text-slate-400 uppercase tracking-widest mb-2">
            {t("profile.stats_overview")}
          </p>
          <div className="grid grid-cols-2 sm:grid-cols-4 gap-2">
            <StatCard label={t("profile.xp")} value={xp.toLocaleString()} accent="purple" />
            <StatCard label={t("profile.games_played")} value={games} />
            <StatCard label={t("profile.sp_victories")} value={sp_victories} accent="green" />
            <StatCard label={t("profile.mp_victories")} value={mp_victories} accent="green" />
          </div>
        </div>

        {/* Answers */}
        <div>
          <p className="text-xs font-medium text-slate-400 uppercase tracking-widest mb-2">
            {t("profile.stats_answers")}
          </p>
          <div className="grid grid-cols-2 sm:grid-cols-4 gap-2">
            <StatCard label={t("profile.correct_answers")} value={correct_answers.toLocaleString()} accent="green" />
            <StatCard label={t("profile.best_streak")} value={highest_streak} />
            <StatCard label={t("profile.accuracy")} value={`${accuracy}%`} />
            <StatCard label={t("profile.lifelines_used")} value={lifeline_used} />
          </div>
        </div>

        {/* Averages */}
        <div>
          <p className="text-xs font-medium text-slate-400 uppercase tracking-widest mb-2">
            {t("profile.stats_averages")}
          </p>
          <div className="grid grid-cols-2 gap-2">
            <StatCard label={t("profile.avg_score_sp")} value={sp_average_score.toFixed(1)} />
            <StatCard label={t("profile.avg_score_mp")} value={mp_average_score.toFixed(1)} />
          </div>
        </div>

      </div>
    </div>
  );
}

function StatCard({
  label,
  value,
  accent,
}: {
  label: string;
  value: string | number;
  accent?: "purple" | "green" | "red";
}) {
  const accentClass =
    accent === "purple"
      ? "text-violet-400"
      : accent === "green"
      ? "text-emerald-400"
      : accent === "red"
      ? "text-red-400"
      : "text-white";

  return (
    <div className="bg-[#0F172A] rounded-lg p-3">
      <p className="text-xs text-slate-400 mb-1">{label}</p>
      <p className={`text-2xl font-medium ${accentClass}`}>{value}</p>
    </div>
  );
}