interface IntroScreenProps {
	roomcode: string;
	players: string[];
	myUsername: string;
	onChangeName: () => void;
}

export default function IntroScreen({ roomcode, players, myUsername, onChangeName }: IntroScreenProps) {
	return (
		<div className="h-full flex flex-col items-center justify-center p-6 bg-[#0F172A] text-white">
			<div className="max-w-md w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center text-center gap-8 animate-in fade-in zoom-in duration-500">
				<div className="flex flex-col gap-2">
					<span className="text-4xl">🎮</span>
					<h1 className="text-3xl font-black text-[#FACC15] tracking-tighter uppercase">Waiting Room</h1>
					<p className="text-[#94A3B8] text-sm font-medium">
						Room Code: <span className="text-[#FACC15] font-mono">{roomcode}</span>
					</p>
				</div>

				<div className="w-full bg-[#0F172A]/50 p-6 rounded-2xl border border-[#334155] flex flex-col gap-6">
					<div className="flex justify-between items-center border-b border-[#334155] pb-4">
						<h3 className="text-[#FACC15] font-black text-xs uppercase tracking-[0.2em]">Joined Players</h3>
						<span className="bg-[#FACC15] text-[#0F172A] text-[10px] font-black px-3 py-1 rounded-full uppercase">
							{players.length}
						</span>
					</div>

					<div className="flex-1 flex flex-col gap-3 min-h-[200px]">
						{players.length === 0 ? (
							<div className="flex-1 flex flex-col items-center justify-center gap-4 text-[#475569]">
								<div className="w-10 h-10 rounded-full border-2 border-dashed border-[#334155] animate-spin border-t-[#FACC15]"></div>
								<span className="text-[10px] font-bold uppercase tracking-[0.2em] animate-pulse">Waiting for others...</span>
							</div>
						) : (
							players.map((p, idx) => (
								<div key={idx} className="flex items-center gap-3 p-3 bg-[#1E293B] rounded-xl border border-[#334155] animate-in fade-in slide-in-from-right-2 duration-300">
									<div className="w-8 h-8 rounded-lg bg-gradient-to-br from-[#FACC15] to-[#EAB308] flex items-center justify-center text-[#0F172A] font-black text-xs">
										{idx + 1}
									</div>
									<div className="flex flex-col items-start translate-y-[1px]">
										<span className="font-bold text-sm text-[#F8FAFC]">{p}</span>
										{p === myUsername && <span className="text-[8px] font-bold text-[#FACC15] uppercase tracking-widest">You</span>}
									</div>
								</div>
							))
						)}
					</div>
				</div>

				<div className="flex flex-col gap-2">
					<p className="text-[10px] font-black text-[#475569] uppercase tracking-[0.3em] animate-pulse">
						Wait for the host to start
					</p>
				</div>

				<button
					onClick={onChangeName}
					className="text-[#475569] hover:text-red-400 transition text-[10px] font-bold uppercase tracking-[0.2em] border-t border-[#334155] pt-4 w-full"
				>
					Change Name / Leave
				</button>
			</div>
		</div>
	);
}
