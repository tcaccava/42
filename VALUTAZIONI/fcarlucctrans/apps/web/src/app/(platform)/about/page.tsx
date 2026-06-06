'use client';

import { local } from '../../../utilities';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { LinkButton } from '@/components/ui/link-button';
import { Info } from 'lucide-react';

export default function AboutUs() {
	const t = local.useLocalization().t;
	return (
		<PageShell maxWidth="4xl">
			<PageHeader
				icon={<Info className="h-5 w-5 text-teal-600" />}
				title="About Us"
				description={t.accessibilityFooter}
			/>

			<div className="mb-6">
				<LinkButton href="/" variant="secondary">
					{t.goHome}
				</LinkButton>
			</div>

			<SectionCard contentClassName="space-y-4">
				<p className="text-justify text-gray-700">{t.aboutUs1}</p>
				<p className="text-justify text-gray-700">{t.aboutUs2}</p>
				<p className="text-justify text-gray-700">{t.aboutUs3}</p>
			</SectionCard>

			<footer className="sr-only" aria-label="Footer">
				{t.accessibilityFooter}
			</footer>
		</PageShell>
	)
}
