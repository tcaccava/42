"use client";
import { use, useState, useEffect, useRef } from "react";
import { notFound } from 'next/navigation';
import Link from 'next/link';
import AvatarInfo from "./components/AvatarInfo";
import Statistics from "../../profile/components/Statistics";
import Badges from "../../profile/components/Badges";

export default function Profile({ params }: { params: Promise<{ username: string }> }) {
	const { username: usernameParam } = use(params);
	const searchedUsername = decodeURIComponent(usernameParam);
	const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';

	const [stats, setStats] = useState({
		games: 0,
		sp_victories: 0,
		mp_victories: 0,
		lost: 0,
		xp: 0,
		correct_answers: 0,
		wrong_answers: 0,
		lifeline_used: 0,
		sp_average_score: 0,
		mp_average_score: 0,
		highest_streak: 0,
		emoji: '',
		profile_picture: null as string | null,
		badges: [] as { image: string; name: string; label: string }[],
	});
	const [isNotFound, setIsNotFound] = useState(false);

	useEffect(() => {
		const fetchStats = async () => {
			try {
				const response = await fetch(`${API_BASE}/api/get_statistics?username=${searchedUsername}`);
				if (response.ok) {
					const data = await response.json();
					setStats(data);
				} else if (response.status === 404) {
					setIsNotFound(true);
				}
			} catch (err) {
				console.error('Error loading statistics:', err);
			}
		};
		fetchStats();
	}, [API_BASE, searchedUsername]);

	if (isNotFound) {
		notFound();
	}

	const searchedAvatar = stats.emoji;
	const searchedProfilePicture = stats.profile_picture;
	const xp = stats.xp;
	const level = Math.floor((xp ** 0.6) / 10) + 1;
	const xpMax = Math.floor((10 * level) ** (5 / 3));
	const badges = (stats.badges || []).map((m: any) => ({
		image: m.image,
		name: m.name,
		label: m.description
	}));


	return (
		<div className="h-full bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col">

			{/* Profile Section */}
			<main className="flex flex-1 flex-col items-center py-10 px-4">
				<div className="w-full max-w-2xl flex flex-col items-center gap-8">
					{/* Avatar + Info */}
					<AvatarInfo avatar={searchedAvatar} profilePicture={searchedProfilePicture} username={searchedUsername} level={level} xp={xp} xpMax={xpMax} />

					<Statistics
						games={stats.games}
						sp_victories={stats.sp_victories}
						mp_victories={stats.mp_victories}
						xp={stats.xp}
						correct_answers={stats.correct_answers}
						wrong_answers={stats.wrong_answers}
						lifeline_used={stats.lifeline_used}
						sp_average_score={stats.sp_average_score}
						mp_average_score={stats.mp_average_score}
						highest_streak={stats.highest_streak}
					/>

					<Badges badges={badges} showViewAll={false} />

				</div>
			</main>
		</div>
	);
}
