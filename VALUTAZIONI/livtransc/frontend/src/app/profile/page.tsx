"use client";
import { useState, useEffect, useRef } from "react";
import { useUsername, useAvatar, useProfilePicture, dispatchNavbarUpdate } from "@/hooks/navbarDatas";
import AvatarPicker from "./components/AvatarPicker";
import UserInfo from "./components/UserInfo";
import Statistics from "./components/Statistics";
import Badges from "./components/Badges";
import MatchHistory from "./components/MatchHistory";
import LogoutButton from "./components/LogoutButton";

export default function Profile() {

	const username = useUsername();
	const baseAvatar = useAvatar();
	const baseProfilePicture = useProfilePicture();
	const [avatar, setAvatar] = useState(() => '');
	const [profilePicture, setProfilePicture] = useState<string | null>(null);
	const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

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
        badges: [] as { image: string; name: string; label: string }[],
        last_matches: [] as any[]
    });

	useEffect(() => {
		if (!username) return;
		const fetchStats = async () => {
			try {
				const response = await fetch(`${API_BASE}/api/get_statistics?username=${username}`);
				if (response.ok) {
					const data = await response.json();
					console.log(data);
					setStats(data);
				}
			} catch (err) {
				console.error('Error loading statistics:', err);
			}
		};
		fetchStats();
	}, [API_BASE, username]);


	useEffect(() => {
		if (baseAvatar) setAvatar(baseAvatar);
	}, [baseAvatar]);

	useEffect(() => {
		setProfilePicture(baseProfilePicture);
	}, [baseProfilePicture]);


	const handleLogout = async () => {
		try {
			sessionStorage.clear();
			localStorage.removeItem('username');
			localStorage.removeItem('avatar');
			localStorage.removeItem('profile_picture');
			localStorage.removeItem('user_id');

			await fetch(`${API_BASE}/auth/logout`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
			});
		} catch (error) {
			console.error('Logout error:', error);
		} finally {
			window.location.href = '/';
		}
	};

	// Dati dinamici da API
	const xp = stats.xp;
	const level = Math.floor((xp ** 0.6) / 10) + 1;
	const xpMax = Math.floor((10 * level) ** (5 / 3));

	const badges = (stats.badges || []).map((m: any) => ({
		image: m.image,
		name: m.name,
		label: m.description
	}));

	const old_matches = (stats.last_matches || []).slice(0, 3).map((m: any, index: number) => ({
		id: index,
		opponent: m.mode || 'AI',
		result: m.status === 0 ? 'Win' : 'Lost',
		score: `${m.correct_answers}/15`,
		ranking: m.ranking ?? index + 1,
		date: new Date(m.finished_at).toLocaleDateString(),
		duration: 'N/A'
	}));

	return (
		<div className="bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col relative">
			{/* Profile Section */}
			<main className="flex flex-1 flex-col items-center py-10 px-4">
				<div className="w-full max-w-2xl flex flex-col items-center gap-8">
					{/* Avatar + Info */}
					<div className="flex w-full gap-8 items-center justify-center">
						<AvatarPicker
							avatar={avatar}
							setAvatar={setAvatar}
							profilePicture={profilePicture}
							setProfilePicture={setProfilePicture}
							API_BASE={API_BASE}
							dispatchNavbarUpdate={dispatchNavbarUpdate}
						/>
						<UserInfo username={username} level={level} xp={xp} xpMax={xpMax} API_BASE={API_BASE} dispatchNavbarUpdate={dispatchNavbarUpdate} />
					</div>

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

					<Badges badges={badges} />

					<MatchHistory old_matches={old_matches} />

					<LogoutButton onLogout={handleLogout} />
				</div>
			</main>
		</div>
	);
}
