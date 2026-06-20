interface GameUIProps {
	optionStatuses: Record<string, "green" | "red" | "yellow" | "none">;
	isLocked: boolean;
	onAnswer: (letter: string) => void;
}

export default function GameUI({ optionStatuses, isLocked, onAnswer }: GameUIProps) {
	return (
		<div
			className="fixed inset-0 w-full bg-[#0F172A] flex flex-col p-2 z-50 overflow-hidden"
			style={{ height: "100svh", overscrollBehavior: "none", touchAction: "none" }}
		>
			<div className="flex-1 flex flex-col gap-2">
				{["A", "B", "C", "D"].map((letter, i) => {
					const status = optionStatuses[letter] || "none";

					return (
						<button
							key={i}
							onClick={() => onAnswer(letter)}
							disabled={isLocked}
							className={`flex-1 relative flex items-center justify-center border-2 transition-all duration-300 shadow-xl rounded-2xl
                                    ${
									status !== "none"
										? status === "yellow"
											? "bg-[#FACC15]/20 border-[#FACC15]"
											: status === "green"
											? "bg-green-500/20 border-green-500"
											: "bg-red-500/20 border-red-500"
										: `bg-[#1E293B] border-transparent ${!isLocked ? "active:bg-[#334155] active:border-[#FACC15]/40" : "opacity-30"}`
								}
                                    ${isLocked ? "cursor-default" : "cursor-pointer"}
                                `}
						>
							<span
								className={`text-6xl font-black transition-all duration-300
                                    ${
									status !== "none"
										? status === "yellow"
											? "text-[#FACC15]"
											: status === "green"
											? "text-green-400"
											: "text-red-400"
										: "text-[#FACC15]"
								}
                                `}
							>
								{letter}
							</span>
						</button>
					);
				})}
			</div>
		</div>
	);
}
