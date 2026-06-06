'use client';

import Link from 'next/link';
import { FileText, Shield } from 'lucide-react';
import { local } from '@/utilities';
import { LegalLinks } from '@/components/legal/LegalLinks';
import { LinkButton } from '@/components/ui/link-button';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';

type LegalSection = {
	titleKey: string;
	bodyKey: string;
};

type LegalDocumentPageProps = {
	kind: 'terms' | 'privacy-policy';
	sections: readonly LegalSection[];
};

export function LegalDocumentPage({ kind, sections }: LegalDocumentPageProps) {
	const t = local.useLocalization().t;
	const isTerms = kind === 'terms';

	return (
		<PageShell maxWidth="4xl">
			<PageHeader
				icon={
					isTerms ? (
						<FileText className="h-5 w-5 text-teal-600" />
					) : (
						<Shield className="h-5 w-5 text-teal-600" />
					)
				}
				title={isTerms ? t.termsPageTitle : t.privacyPolicyPageTitle}
				description={isTerms ? t.termsPageDescription : t.privacyPolicyPageDescription}
			/>

			<div className="mb-6 flex flex-wrap items-center gap-3">
				<LinkButton href="/" variant="secondary">
					{t.legalBackHome}
				</LinkButton>
				<LegalLinks
					className="text-sm"
					exclude={isTerms ? 'terms' : 'privacy-policy'}
				/>
			</div>

			<p className="mb-6 text-sm text-foreground/60">
				{isTerms ? t.termsLastUpdated : t.privacyPolicyLastUpdated}
			</p>

			<div className="space-y-6">
				{sections.map(({ titleKey, bodyKey }) => (
					<SectionCard key={titleKey} title={t[titleKey]}>
						<p className="whitespace-pre-line text-justify text-sm leading-relaxed text-foreground/80">
							{t[bodyKey]}
						</p>
					</SectionCard>
				))}
			</div>

			{!isTerms ? (
				<p className="mt-8 text-sm text-foreground/70">
					{t.privacyPolicyManageData}{' '}
					<Link
						href="/settings/privacy"
						className="font-medium text-teal-600 underline-offset-2 hover:underline"
					>
						{t.privacyAndData}
					</Link>
					.
				</p>
			) : null}
		</PageShell>
	);
}
