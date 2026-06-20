"use client";
import Link from "next/link";
import SinglePlayerCard from './components/SinglePlayerCard'
import MultiplayerCard from "./components/MultiPlayerCard";

export default function Play() {

	return (
		<div className="h-full bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col">
			{/* Main content */}
			<main className="flex flex-1 flex-col items-center justify-center gap-10 px-4 relative -top-7">
				<div className="flex flex-col gap-8 w-full max-w-lg">
					{/* Singleplayer Card */}
					<SinglePlayerCard/>
					{/* Multiplayer Card */}
					<MultiplayerCard/>
				</div>
			</main>
		</div>
	);
}
