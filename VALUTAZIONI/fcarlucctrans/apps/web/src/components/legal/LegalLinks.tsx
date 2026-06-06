'use client';

import Link from 'next/link';
import { useLocalization } from '@/utilities/localization';

type LegalLinksProps = {
	className?: string;
	exclude?: 'terms' | 'privacy-policy';
	linkClassName?: string;
};

export function LegalLinks({
	className = '',
	exclude,
	linkClassName = 'text-foreground/70 underline-offset-2 hover:text-teal-600 hover:underline',
}: LegalLinksProps) {
	const { t } = useLocalization();

	return (
		<nav aria-label={t.legalPagesNavLabel} className={`flex flex-wrap items-center gap-x-4 gap-y-2 ${className}`}>
			{exclude !== 'terms' ? (
				<Link href="/legal/terms" className={linkClassName}>
					{t.termsOfService}
				</Link>
			) : null}
			{exclude !== 'privacy-policy' ? (
				<Link href="/legal/privacy-policy" className={linkClassName}>
					{t.privacyPolicy}
				</Link>
			) : null}
		</nav>
	);
}
