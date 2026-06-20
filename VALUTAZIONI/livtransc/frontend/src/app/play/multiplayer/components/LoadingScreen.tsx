"use client";

interface LoadingScreenProps {
  message?: string;
}

export default function LoadingScreen({ message = "Contacting Trivia Master" }: LoadingScreenProps) {
  return (
    <div className="h-full flex items-center justify-center bg-[#0F172A] text-[#FACC15]">
      <span className="text-xs font-black tracking-[0.3em] uppercase animate-pulse">
        {message}
      </span>
    </div>
  );
}
