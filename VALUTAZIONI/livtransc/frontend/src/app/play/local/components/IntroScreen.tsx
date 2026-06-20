import Link from "next/link";
import { copyToClipboard } from "@/lib/clipboard";
import { useTranslation } from "@/hooks/useTranslation";

interface IntroScreenProps {
	roomCode: string;
	players: string[];
	onStart: () => void;
}

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';


export default function IntroScreen({ roomCode, players, onStart }: IntroScreenProps) {
	const { t } = useTranslation();
	return (
		<div className="flex-1 w-full flex flex-col items-center justify-center bg-[#0F172A] text-white overflow-hidden p-0">
			<div className="max-w-4xl w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-5 shadow-2xl flex flex-col gap-5">
				<div className="flex flex-col gap-1 items-center">
					<span className="text-3xl">👥</span>
					<h1 className="text-3xl font-black text-[#FACC15] tracking-tighter uppercase">{t("play.multiplayer")}</h1>
					<p className="text-[#94A3B8] font-medium text-xs">{t("lobby.scan_to_join")}</p>
				</div>

				<div className="grid grid-cols-1 lg:grid-cols-2 gap-6 items-stretch">
					{/* Left: Join Section */}
					<div className="bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155] flex flex-col items-center justify-center gap-4">
						<div className="relative group">
							<div className="absolute -inset-2 bg-gradient-to-r from-[#FACC15] to-[#EAB308] rounded-3xl blur opacity-20 group-hover:opacity-40 transition duration-1000"></div>
							<div className="relative bg-white p-2 rounded-2xl shadow-2xl">
								<img
									src={`https://api.qrserver.com/v1/create-qr-code/?size=150x150&data=${encodeURIComponent(`${API_BASE}/play/local/${roomCode}`)}`}
									alt="Game QR Code"
									className="w-32 h-32"
								/>
							</div>
						</div>

						<div
							className="w-full bg-[#1E293B] px-4 py-2 rounded-xl border border-[#334155] flex items-center justify-between gap-4 group hover:border-[#FACC15]/30 transition-all cursor-pointer select-all"
							onClick={() => copyToClipboard(`${API_BASE}/play/local/${roomCode}`)}
						>
							<span className="font-mono font-bold text-[24px] text-[#FACC15] truncate">{roomCode}</span>
							<span className="text-[8px] font-bold text-[#475569] uppercase tracking-widest">{t("lobby.copy_url")}</span>
						</div>
					</div>

					{/* Right: Players Section */}
					<div className="bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155] flex flex-col gap-4">
						<div className="flex justify-between items-center border-b border-[#334155] pb-3">
							<h3 className="text-[#FACC15] font-black text-[10px] uppercase tracking-[0.2em]">{t("lobby.players")}</h3>
							<span className="bg-[#FACC15] text-[#0F172A] text-[9px] font-black px-2 py-0.5 rounded-full uppercase tracking-tighter">
								{players.length} {t("lobby.joined")}
							</span>
						</div>

						<div className="flex-1 flex flex-col gap-2 min-h-[150px]">
							{players.length === 0 ? (
								<div className="flex-1 flex flex-col items-center justify-center gap-4 text-[#475569]">
									<div className="w-10 h-10 rounded-full border-2 border-dashed border-[#334155] animate-spin border-t-[#FACC15]"></div>
									<span className="text-[10px] font-bold uppercase tracking-[0.2em] animate-pulse">{t("lobby.waiting")}</span>
								</div>
							) : (
								players.map((p, idx) => (
									<div key={idx} className="flex items-center gap-3 p-3 bg-[#1E293B] rounded-xl border border-[#334155] animate-in fade-in slide-in-from-right-2 duration-300">
										<div className="w-8 h-8 rounded-lg bg-gradient-to-br from-[#FACC15] to-[#EAB308] flex items-center justify-center text-[#0F172A] font-black text-xs">
											{idx + 1}
										</div>
										<span className="font-bold text-sm text-[#F8FAFC]">{p}</span>
									</div>
								))
							)}
						</div>
					</div>
				</div>

				<div className="flex flex-col gap-3">
					<button
						onClick={onStart}
						disabled={players.length < 2}
						className={`w-full py-3 rounded-xl font-black text-lg transition-all shadow-[0_0_20px_rgba(250,204,21,0.15)]
							${players.length < 2
								? "bg-slate-700 text-slate-500 cursor-not-allowed opacity-50 shadow-none"
								: "bg-[#FACC15] text-[#0F172A] hover:scale-[1.01] active:scale-[0.99] cursor-pointer"
							}
						`}
					>
						{t("lobby.start_game")}
					</button>

					<Link href="/play" className="text-[#475569] hover:text-[#94A3B8] transition text-center text-[10px] font-bold uppercase tracking-widest">
						← {t("game.back_to_menu")}
					</Link>
				</div>
			</div>
		</div>
	);
}
