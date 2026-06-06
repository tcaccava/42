"use client"

import { local, knav, motion, spacing, size } from '@/utilities';
import { ThemeDisplayPanel } from '@/components/settings/ThemeDisplayPanel';
import { LanguagePanel } from '@/components/settings/LanguagePanel';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { SettingsNav } from '@/components/SettingsNav';
import { Settings } from 'lucide-react';

const fieldClass =
	'space-y-4 [&_label]:text-sm [&_label]:font-medium [&_label]:text-foreground [&_select]:w-full [&_select]:rounded-lg [&_select]:border-2 [&_select]:border-border [&_select]:bg-background [&_select]:px-4 [&_select]:py-2 [&_select]:text-foreground [&_select]:focus:border-[var(--btn-background)] [&_select]:focus:outline-none [&_select]:focus:ring-2 [&_select]:focus:ring-[var(--btn-background)]/30';

export default function SettingsClient() {
	const { t } = local.useLocalization();
	return (
		<PageShell>
			<PageHeader
				icon={<Settings className="h-5 w-5 text-[var(--hvr-background)]" />}
				title={t.settingsTitle}
				description={t.settingsDescription}
			/>

			<SettingsNav />

			<div className="space-y-6">
				<SectionCard
					title={t.themeDisplay}
					description={t.themeDisplayDescription}
					contentClassName="p-6"
				>
					<ThemeDisplayPanel />
				</SectionCard>

				<SectionCard title={t.motionAnimation} contentClassName={fieldClass}>
					<motion.UseReduceMotionToggle />
					<knav.UseKeyboardNavToggle />
				</SectionCard>

				<SectionCard title={t.textContent} contentClassName={fieldClass}>
					<size.UseFontSizeControl />
					<spacing.UseTextSpacingToggle />
				</SectionCard>

				<SectionCard title={t.language} description={t.languageDescription} contentClassName="p-6">
					<LanguagePanel />
				</SectionCard>
			</div>
		</PageShell>
	)
}
