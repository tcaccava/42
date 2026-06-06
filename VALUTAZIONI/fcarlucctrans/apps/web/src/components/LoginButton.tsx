'use client';

import { loginAction } from '@/server/login';
import { useTransition } from 'react';
import Image from 'next/image';
import { local } from '@/utilities'

interface LoginButtonProps {
	className?: string;
}

export function LoginButton({ className = '' }: LoginButtonProps) {
	const [isPending, startTransition] = useTransition();
	const t = local.useLocalization().t;

	const handleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
		e.preventDefault();
		startTransition(async () => {
			await loginAction();
		});
	};

	return (
		<form onSubmit={handleSubmit} className="relative">
			<button
				type="submit"
				disabled={isPending}
				className={`group relative inline-flex items-center gap-3 px-8 py-4 bg-[#05BBBB] text-white rounded-xl font-bold text-lg transition-all duration-300 hover:scale-105 hover:shadow-[0_0_40px_-10px_rgba(5,187,187,0.3)] disabled:opacity-70 disabled:hover:scale-100 ${className}`}
			>
				{isPending ? (
					<>
						<div className="w-6 h-6 border-2 border-white border-t-transparent rounded-full animate-spin" />
						<span>{t.connecting}...</span>
					</>
				) : (
					<>
						<Image
							src="/42logo.png"
							alt="42 Logo"
							width={24}
							height={24}
							className="w-8 h-8 object-contain brightness-0 invert"
						/>
						<span>{t.signIn}</span>
					</>
				)}
			</button>
		</form>
	);
}

