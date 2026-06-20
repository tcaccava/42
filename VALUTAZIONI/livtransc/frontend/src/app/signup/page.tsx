"use client";

import SignupForm from "./components/SignupForm";

export default function Register() {
  return (
    <div className="h-full flex flex-col bg-[#0F172A] text-[#F8FAFC] font-sans">
      {/* Register Form */}
      <main className="flex flex-1 flex-col items-center justify-center px-4">
        <SignupForm />
      </main>
    </div>
  );
}
