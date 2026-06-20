
"use client";
import LoginForm from "./components/LoginForm";

export default function LoginPage() {
  return (
    <div className="h-full flex flex-col bg-[#0F172A] text-[#F8FAFC] font-sans">
      {/* Login Form */}
      <main className="flex flex-1 flex-col items-center justify-center px-4">
        <LoginForm />
      </main>
    </div>
  );
}
