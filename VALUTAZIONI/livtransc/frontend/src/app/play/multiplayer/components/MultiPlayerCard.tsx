import Link from "next/link";

const MultiplayerCard = () => {
	return(
		<Link
		href="/play/multiplayer2"
		className="bg-[#1E293B] rounded-xl shadow-lg p-8 flex flex-col items-start hover:scale-105 transition cursor-pointer">
			<div className="flex items-center gap-3 mb-2">
				<span className="text-2xl">👥 vs 👥</span>
				<span className="text-lg font-bold">TEAM VS TEAM</span>
			</div>
			<span className="text-[#94A3B8]">Play with your friends</span>
		</Link>
	)
}

export default MultiplayerCard;
