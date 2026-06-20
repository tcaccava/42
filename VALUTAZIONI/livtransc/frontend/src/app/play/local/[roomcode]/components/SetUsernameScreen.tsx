import Link from "next/link";

interface SetUsernameScreenProps {
	roomcode: string;
	inputUsername: string;
	onUsernameChange: (username: string) => void;
	onSubmit: () => void;
	isJoining: boolean;
	error: string;
}

export default function SetUsernameScreen({
	roomcode,
	inputUsername,
	onUsernameChange,
	onSubmit,
	isJoining,
	error,
}: SetUsernameScreenProps) {
	const handleKeyDown = (e: React.KeyboardEvent) => {
		if (e.key === "Enter" && !isJoining && inputUsername.trim()) {
			onSubmit();
		}
	};

	return (
		<div className="min-h-screen flex flex-col items-center justify-center p-6 bg-[#0F172A] text-white">
			<div className="max-w-md w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center text-center gap-8 animate-in fade-in zoom-in duration-500">
				<div className="flex flex-col gap-2">
					<span className="text-5xl">👋</span>
					<h1 className="text-3xl font-black text-[#FACC15] tracking-tighter uppercase">Join Room</h1>
					<p className="text-[#94A3B8] text-sm font-medium">
						Enter your username to join room <span className="text-[#FACC15] font-mono">{roomcode}</span>
					</p>
				</div>

				<div className="w-full flex flex-col gap-4">
					<div className="relative group">
						<input
							type="text"
							value={inputUsername}
							onChange={(e) => onUsernameChange(e.target.value)}
							onKeyDown={handleKeyDown}
							placeholder="Your Username"
							className="w-full bg-[#0F172A] border-2 border-[#334155] rounded-xl px-5 py-4 font-bold text-white placeholder-[#475569] focus:border-[#FACC15] focus:outline-none transition-all"
							autoFocus
						/>
					</div>

					{error && <p className="text-red-500 text-xs font-bold animate-pulse">{error}</p>}

					<button
						onClick={onSubmit}
						disabled={!inputUsername.trim() || isJoining}
						className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black text-lg hover:scale-[1.02] active:scale-[0.98] transition-all disabled:opacity-50 disabled:grayscale disabled:cursor-not-allowed shadow-[0_0_20px_rgba(250,204,21,0.2)] cursor-pointer"
					>
						{isJoining ? "JOINING..." : "JOIN SESSION"}
					</button>
				</div>

				<Link href="/play" className="text-[#475569] hover:text-[#94A3B8] transition text-xs font-bold uppercase tracking-widest">
					Cancel
				</Link>
			</div>
		</div>
	);
}
