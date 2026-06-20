"use client";
import Link from 'next/link';

export default function UserNotFound() {
  return (
    <div className="h-full bg-[#0F172A] text-[#F8FAFC] font-sans flex flex-col">
      <main className="flex flex-1 flex-col items-center justify-center py-10 px-4">
        <div className="w-full max-w-md flex flex-col items-center gap-8">
          <span className="text-3xl font-bold text-red-500">404 - User not found</span>
          <Link
            href="/profile/friends"
            className="px-6 py-2 rounded-lg bg-blue-600 text-white font-bold text-base hover:bg-blue-700 transition text-center w-full max-w-xs"
          >
            Search users
          </Link>
        </div>
      </main>
    </div>
  );
}