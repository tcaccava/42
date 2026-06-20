interface ResultsScreenProps {
	message: string;
	onRestart: () => void;
}

export default function ResultsScreen({ message, onRestart }: ResultsScreenProps) {
	return (
		<div className="h-full flex flex-col items-center justify-center p-6 bg-[#0F172A] text-white">
			<div className="max-w-md w-full bg-[#1E293B] border border-[#334155] rounded-3xl p-10 shadow-2xl flex flex-col items-center text-center gap-8 animate-in fade-in zoom-in duration-500">
				<span className="text-6xl">🏁</span>
				<div className="flex flex-col gap-1">
					<h2 className="text-3xl font-black text-[#FACC15]">GAME OVER</h2>
					<p className="text-[#94A3B8] font-medium">{message}</p>
				</div>
				<button
					onClick={onRestart}
					className="w-full py-4 rounded-xl bg-[#FACC15] text-[#0F172A] font-black text-center hover:scale-[1.02] transition cursor-pointer"
				>
					RESTART
				</button>
			</div>
		</div>
	);
}
