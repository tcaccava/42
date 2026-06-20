import { useTranslation } from "@/hooks/useTranslation";

interface Question {
	id: number;
	text: string;
	options: string[];
	number: number;
}

interface GameUIProps {
	question: Question | null;
	time: number;
	optionStatuses: Record<string, "green" | "red" | "yellow" | "none">;
	showNicknames: boolean;
	currentAnswers: Record<string, string>;
}

export default function GameUI({ question, time, optionStatuses, showNicknames, currentAnswers }: GameUIProps) {
	const { t } = useTranslation();
	return (
		<div className="min-h-screen flex items-center justify-center p-6 bg-[#0F172A] text-white pt-2 pb-12 overflow-y-auto">
			<div className="w-full max-w-9xl flex flex-row gap-8 items-center justify-center">

				{/* Lateral HUD Column (Left) */}
				<div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-left-4 duration-700">
					<span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">{t("play.time")}</span>

					{/* Vertical Progress Bar */}
					<div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
						<div
							className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
							style={{ height: `${(time / 20) * 100}%` }}
						></div>
						<div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
					</div>

					<div className="flex flex-col items-center gap-1">
						<span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">{t("play.left")}</span>
						<span className="text-4xl font-black text-[#FACC15] tracking-tighter">{time}</span>
					</div>
				</div>

				{/* Question Area (Center) */}
				<div className="flex-1 max-w-6xl flex flex-col gap-8 animate-in fade-in slide-in-from-bottom-4 duration-700">
					<div className="flex flex-col gap-2">
						<span className="text-[#FACC15] font-black tracking-widest text-sm text-center uppercase md:text-left">{t("play.questions")} {question?.number} {t("play.of_15")}</span>
						<h2 className={`font-black leading-tight text-center md:text-left ${(question?.text?.length ?? 0) > 150 ? "text-xl md:text-2xl" : "text-2xl md:text-3xl"
							}`}>
							{question?.text}
						</h2>
						{showNicknames}
					</div>

					<div className="grid grid-cols-1 gap-4">
						{question?.options.map((option, i) => {
							const status = optionStatuses[option] || "none";

							return (
								<div
									key={i}
									className={`relative flex items-center p-6 border-2 transition-all duration-300 text-left overflow-hidden shadow-xl rounded-2xl cursor-default
										${status !== "none"
											? (status === "yellow"
												? "bg-[#FACC15]/10 border-[#FACC15]"
												: (status === "green" ? "bg-green-500/20 border-green-500" : "bg-red-500/20 border-red-500"))
											: "bg-[#1E293B] border-transparent"}
									`}
								>
									<span className={`w-10 h-10 rounded-xl font-black flex items-center justify-center transition-all duration-300 shadow-inner
										${status !== "none"
											? (status === "yellow"
												? "bg-[#FACC15] text-[#0F172A]"
												: (status === "green" ? "bg-green-500 text-white" : "bg-red-500 text-white"))
											: "bg-[#0F172A] text-[#FACC15]"}
									`}>
										{String.fromCharCode(65 + i)}
									</span>
									<span className={`ml-6 font-bold text-xl transition-colors
										${status !== "none"
											? (status === "yellow" ? "text-white" : (status === "green" ? "text-green-400" : "text-red-400"))
											: ""}
									`}>
										{option}
									</span>

									{showNicknames && (
										<div className="ml-auto flex flex-wrap gap-2 justify-end max-w-[40%]">
											{Object.entries(currentAnswers)
												.filter(([_, ans]) => ans === String.fromCharCode(65 + i))
												.map(([name]) => (
													<span
														key={name}
														className="px-3 py-1 bg-[#FACC15] text-[#0F172A] rounded-lg text-[10px] font-black uppercase tracking-wider animate-in zoom-in duration-300"
													>
														{name}
													</span>
												))
											}
										</div>
									)}
								</div>
							);
						})}
					</div>
				</div>

				{/* Lateral HUD Column (Right) */}
				<div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-right-4 duration-700">
					<span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">15</span>

					{/* Vertical Progress Bar */}
					<div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
						<div
							className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
							style={{ height: `${((question?.number || 1) - 1) / 15 * 100}%` }}
						></div>
						<div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
					</div>

					<div className="flex flex-col items-center gap-1">
						<span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">{t("singleplayer.current")}</span>
						<span className="text-4xl font-black text-[#FACC15] tracking-tighter">{question?.number}</span>
					</div>
				</div>
			</div>
		</div>
	);
}
