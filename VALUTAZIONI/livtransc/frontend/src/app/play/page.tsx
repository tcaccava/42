"use client";
import Link from "next/link";
import { useRouter } from "next/navigation";
import { useTranslation } from "@/hooks/useTranslation";

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';

export default function Play() {
	const router = useRouter();
	const { t } = useTranslation();

	const createLobby = async () => {
		try {
			const response = await fetch(`${API_BASE}/api/multiplayer`, {
				method: "POST",
				body: JSON.stringify({ host_id: localStorage.getItem('user_id') }),
				headers: { "Content-Type": "application/json" },
				credentials: 'include'
			})
			const data = await response.json()
			if (!response.ok) {
				console.error('Error creating lobby:', data.error);
				alert(t("login.error_generic"));
				return;
			}
			router.push(`/play/multiplayer/${data.session_id}`)
		} catch (error) {
			console.error('Error fetching session:', error);
		}
	}

	return (
		<div className="flex-1 bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col">
			{/* Main content */}
			<main className="flex flex-1 flex-col items-center justify-center gap-10 px-4">
				<div className="flex flex-col gap-8 w-full max-w-lg">
					{/* Singleplayer Card */}
					<Link
					href="/play/singleplayer"
					className="bg-[#1E293B] rounded-xl shadow-lg p-8 flex flex-col items-start hover:scale-105 transition cursor-pointer"
					>
					<div className="flex items-center gap-3 mb-2">
						<span className="text-2xl">🎯</span>
						<span className="text-lg font-bold">{t("play.singleplayer")}</span>
					</div>
					<span className="text-[#94A3B8]">{t("play.singleplayer_desc")}</span>
					</Link>
					{/* Multiplayer Card */}
					<button onClick={createLobby} className="bg-[#1E293B] rounded-xl shadow-lg p-8 flex flex-col items-start hover:scale-105 transition cursor-pointer">
						<div className="flex items-center gap-3 mb-2">
							<span className="text-2xl">🌐</span>
							<span className="text-lg font-bold">{t("play.multiplayer")}</span>
						</div>
						<span className="text-[#94A3B8]">{t("play.multiplayer_desc")}</span>
					</button>
					{/* Local Multiplayer Card */}
					<Link
					href="/play/local"
					className="bg-[#1E293B] rounded-xl shadow-lg p-8 flex flex-col items-start hover:scale-105 transition cursor-pointer">
						<div className="flex items-center gap-3 mb-2">
							<span className="text-2xl">👥</span>
							<span className="text-lg font-bold">{t("play.local_multiplayer")}</span>
						</div>
						<span className="text-[#94A3B8]">{t("play.local_multiplayer_desc")}</span>
					</Link>
				</div>
			</main>
		</div>
	);
}
