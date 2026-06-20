"use client";
import { useState, useEffect } from "react";
import Link from "next/link";
import { useTranslation } from "@/hooks/useTranslation";
import { useUsername } from "@/hooks/navbarDatas";

export default function Leaderboard() {
	const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';
	const { t } = useTranslation();
	const currentUsername = useUsername();
	const [activeTab, setActiveTab] = useState("XP");


	const tabs = [
		{ key: "Victories", label: t("leaderboard.victories") },
		{ key: "XP", label: t("leaderboard.xp") },
		{ key: "Streak", label: t("leaderboard.streak") }
	];

	const [leaderboard, setLeaderboard] = useState<Record<string, any[]>>({
		victories: [],
		xp: [],
		streak: []
	});

	useEffect(() => {
		const fetchStats = async () => {
			try {
				const response = await fetch(`${API_BASE}/api/get_leaderboard`);
				if (response.ok) {
					const data = await response.json();
					console.log(data);
					if (data.success) {
						setLeaderboard({
							victories: data.victories || [],
							xp: data.xp || [],
							streak: data.streak || []
						});
					}
				}
			} catch (err) {
				console.error('Error loading statistics:', err);
			}
		};
		fetchStats();
	}, [API_BASE, activeTab]);

	return (
		<div className="min-h-screen bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col items-center py-12 px-4">
			{/* 1) Title with Emojis */}
			<header className="text-center mb-12 animate-fade-in">
				<h1 className="text-5xl md:text-6xl font-black mb-2">
					🏆 {t("leaderboard.title")} 🏆
				</h1>
				<p className="text-slate-400 uppercase tracking-widest text-sm font-semibold">
					{t("leaderboard.subtitle")}
				</p>
			</header>

			<main className="w-full max-w-2xl flex flex-col gap-8">
				{/* 2) Three Wide Selection Buttons */}
				<div className="grid grid-cols-3 gap-3 p-1 bg-slate-800/50 backdrop-blur-md rounded-2xl border border-slate-700/50 shadow-2xl">
					{tabs.map((tab) => (
						<button
							key={tab.key}
							onClick={() => setActiveTab(tab.key)}
							className={`py-3 px-4 rounded-xl text-sm font-bold transition-all duration-300 transform ${
								activeTab === tab.key
									? "bg-gradient-to-r from-blue-600 to-purple-600 text-white shadow-lg scale-100 shadow-blue-500/20"
									: "text-slate-400 hover:text-white hover:bg-slate-700/50 scale-100 active:scale-95"
							}`}
						>
							{tab.label}
						</button>
					))}
				</div>

				{/* 3) Placeholder List */}
				<div className="bg-slate-900/40 backdrop-blur-xl border border-slate-700/30 rounded-3xl overflow-hidden shadow-2xl">
					<div className="space-y-1 p-2">
						{leaderboard[activeTab.toLowerCase()].map((player, index) => {
							const rank = index + 1;
							const scoreKey = activeTab === "Streak" ? "highest_streak" : activeTab.toLowerCase();
							const score = player[scoreKey] ?? 0;
							const isCurrentUser = player.user__username === currentUsername;

							return (
								<Link
									key={index}
									href={`/user/${player.user__username}`}
									className={`flex items-center justify-between p-4 rounded-2xl transition-all cursor-pointer group ${
										isCurrentUser
											? "bg-gradient-to-r from-blue-600/30 to-purple-600/10 border border-blue-500/40 shadow-[0_0_15px_rgba(59,130,246,0.15)] hover:from-blue-600/40"
											: "bg-gradient-to-r from-slate-800/20 to-transparent hover:from-slate-800/50"
									}`}
								>
									<div className="flex items-center gap-6">
										<span className={`w-8 text-center font-black  ${rank < 4 ? "text-3xl":"text-xl"}  text-slate-600`}>
											{rank === 1 ? "🥇" : rank === 2 ? "🥈" : rank === 3 ? "🥉" : `#${rank}`}
										</span>
										<div className="w-10 h-10 rounded-full bg-gradient-to-br from-blue-500 to-purple-500 flex items-center justify-center text-white font-bold border-2 border-slate-700 group-hover:border-blue-400 transition-colors">
											{player.user__emoji}
										</div>
										<span className="font-bold text-lg group-hover:text-blue-400 transition-colors tracking-tight flex items-center gap-2">
											{player.user__username}
											{isCurrentUser && (
												<span className="text-xs bg-blue-500/20 text-blue-300 px-2 py-0.5 rounded-full border border-blue-400/30 font-semibold uppercase tracking-wider">
													{t("leaderboard.you")}
												</span>
											)}
										</span>
									</div>
									<div className="text-right">
										<div className="text-xs text-slate-500 uppercase font-bold tracking-tighter">{t("leaderboard.score")}</div>
										<div className="font-black text-blue-400 text-xl tabular-nums">
											{score.toLocaleString()}
										</div>
									</div>
								</Link>
							);
						})}
					</div>
				</div>

				<div className="flex justify-center mt-4">
					<Link 
						href="/play"
						className="text-slate-500 hover:text-white text-xs font-bold uppercase tracking-widest flex items-center gap-2 group transition-all"
					>
						<span className="opacity-0 group-hover:opacity-100 transition-opacity">←</span>
						{t("leaderboard.back_to_arena")}
					</Link>
				</div>
			</main>
		</div>
	);
}

