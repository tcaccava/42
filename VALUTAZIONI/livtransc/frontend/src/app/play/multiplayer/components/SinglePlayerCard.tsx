import Link from "next/link"

const singlePlayerCard =  () => {

	return(
		<Link
			href="/play/multiplayer/royale"
			className="bg-[#1E293B] rounded-xl shadow-lg p-8 flex flex-col items-start hover:scale-105 transition cursor-pointer"
			>
			<div className="flex items-center gap-3 mb-2">
				<span className="text-2xl">👤 vs 👥</span>
				<span className="text-lg font-bold">1 VS ALL</span>
			</div>
			<span className="text-[#94A3B8]">Play alone</span>
		</Link>
	)

}

export default singlePlayerCard;
