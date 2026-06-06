'use client';

import { Monitor, Moon, Sun } from 'lucide-react';
import { Switch } from '@/components/ui/Switch';
import { useAppearance } from '@/utilities/appearance';
import { useLocalization } from '@/utilities/localization';
import type { ThemePreference } from '@/utilities/appearance/core';

const previewPrimaryClass =
	'inline-flex items-center justify-center rounded-lg px-3 py-1.5 text-[length:calc(0.875rem*var(--font-size-multiplier,1))] font-medium bg-[var(--btn-background)] text-white';
const previewSecondaryClass =
	'inline-flex items-center justify-center rounded-lg border-2 border-[var(--color-border)] bg-[var(--color-background)] px-3 py-1.5 text-[length:calc(0.875rem*var(--font-size-multiplier,1))] font-medium text-[var(--color-foreground)]';

type ThemeOption = {
	value: ThemePreference;
	label: string;
	description: string;
	icon: React.ReactNode;
	preview: 'light' | 'dark' | 'split';
};

function ThemePreview({ variant }: { variant: ThemeOption['preview'] }) {
	const lightBar = 'bg-slate-200';
	const darkBar = 'bg-slate-700';
	const lightSurface = 'bg-white';
	const darkSurface = 'bg-slate-900';
	const accent = 'bg-teal-500';

	if (variant === 'split') {
		return (
			<div className="mt-3 flex h-16 overflow-hidden rounded-lg border border-border/80">
				<div className={`flex flex-1 flex-col gap-1.5 p-2 ${lightSurface}`}>
					<div className={`h-2 w-8 rounded ${lightBar}`} />
					<div className={`h-2 w-full rounded ${lightBar}`} />
					<div className={`h-2 w-2/3 rounded ${accent}`} />
				</div>
				<div className={`flex flex-1 flex-col gap-1.5 p-2 ${darkSurface}`}>
					<div className={`h-2 w-8 rounded ${darkBar}`} />
					<div className={`h-2 w-full rounded ${darkBar}`} />
					<div className={`h-2 w-2/3 rounded ${accent}`} />
				</div>
			</div>
		);
	}

	const isDark = variant === 'dark';
	return (
		<div
			className={`mt-3 flex h-16 flex-col gap-1.5 rounded-lg border p-2 ${
				isDark
					? 'border-slate-700 bg-slate-900'
					: 'border-slate-200 bg-white'
			}`}
		>
			<div className={`h-2 w-8 rounded ${isDark ? darkBar : lightBar}`} />
			<div className={`h-2 w-full rounded ${isDark ? darkBar : lightBar}`} />
			<div className={`h-2 w-2/3 rounded ${accent}`} />
		</div>
	);
}

export function ThemeDisplayPanel() {
	const { t, tr } = useLocalization();
	const { theme, resolvedTheme, highContrast, setTheme, setHighContrast } =
		useAppearance();

	const options: ThemeOption[] = [
		{
			value: 'light',
			label: t.themeLight,
			description: t.themeLightDescription,
			icon: <Sun className="h-5 w-5" aria-hidden />,
			preview: 'light',
		},
		{
			value: 'dark',
			label: t.themeDark,
			description: t.themeDarkDescription,
			icon: <Moon className="h-5 w-5" aria-hidden />,
			preview: 'dark',
		},
		{
			value: 'system',
			label: t.themeSystem,
			description: t.themeSystemDescription,
			icon: <Monitor className="h-5 w-5" aria-hidden />,
			preview: 'split',
		},
	];

	const statusText =
		theme === 'system'
			? tr('themeStatusSystem', {
					mode: resolvedTheme === 'dark' ? t.themeDark : t.themeLight,
				})
			: tr('themeStatusFixed', {
					mode: resolvedTheme === 'dark' ? t.themeDark : t.themeLight,
				});

	return (
		<div className="space-y-6">
			<fieldset>
				<legend className="sr-only">{t.themeLabel}</legend>
				<div
					className="grid gap-3 sm:grid-cols-3"
					role="radiogroup"
					aria-label={t.themeLabel}
				>
					{options.map((option) => {
						const selected = theme === option.value;
						return (
							<button
								key={option.value}
								type="button"
								role="radio"
								aria-checked={selected}
								onClick={() => setTheme(option.value)}
								className={`group relative rounded-2xl border-2 p-4 text-left transition-all focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[var(--color-hvr-background)] focus-visible:ring-offset-2 focus-visible:ring-offset-background ${
									selected
										? 'border-[var(--btn-background)] bg-[var(--btn-background)]/5 shadow-sm'
										: 'border-border bg-background hover:border-[var(--btn-background)]/40 hover:bg-darker-background/60'
								}`}
							>
								<div className="flex items-center gap-2">
									<span
										className={`rounded-lg p-2 ${
											selected
												? 'bg-[var(--btn-background)]/15 text-[var(--hvr-background)]'
												: 'bg-darker-background text-foreground/70 group-hover:text-foreground'
										}`}
									>
										{option.icon}
									</span>
									<div>
										<span className="block text-sm font-bold text-foreground">
											{option.label}
										</span>
										<span className="block text-xs text-foreground/60">
											{option.description}
										</span>
									</div>
								</div>
								<ThemePreview variant={option.preview} />
							</button>
						);
					})}
				</div>
			</fieldset>

			<p className="text-sm text-foreground/70" aria-live="polite">
				{statusText}
			</p>

			<Switch
				id="high-contrast"
				checked={highContrast}
				onCheckedChange={setHighContrast}
				label={t.highContrastLabel}
				description={t.highContrastDescription}
			/>

			<div
				className="rounded-2xl border border-dashed border-border bg-darker-background/40 p-5"
				role="group"
				aria-labelledby="theme-preview-heading"
				aria-describedby="theme-preview-note"
			>
				<div className="mb-3 flex flex-wrap items-center gap-2">
					<p
						id="theme-preview-heading"
						className="text-xs font-semibold uppercase tracking-wide text-foreground/50"
					>
						{t.themePreviewLabel}
					</p>
					<span className="rounded-full border border-border bg-background px-2 py-0.5 text-[10px] font-semibold uppercase tracking-wide text-foreground/60">
						{t.themePreviewSampleBadge}
					</span>
				</div>
				<p id="theme-preview-note" className="mb-4 text-sm text-foreground/70">
					{t.themePreviewBody}
				</p>
				<div
					className="space-y-3 rounded-xl border border-border bg-background p-4 shadow-sm"
					aria-hidden="true"
				>
					<div className="flex items-center gap-2">
						<div className="h-8 w-8 rounded-full bg-[var(--btn-background)]/20" />
						<div className="space-y-1">
							<div className="h-2.5 w-24 rounded bg-foreground/20" />
							<div className="h-2 w-16 rounded bg-foreground/10" />
						</div>
					</div>
					<p className="text-sm text-foreground/80">
						{t.themePreviewSampleText}
					</p>
					<div className="flex flex-wrap gap-2 pointer-events-none select-none">
						<span className={previewPrimaryClass}>{t.themePreviewPrimary}</span>
						<span className={previewSecondaryClass}>
							{t.themePreviewSecondary}
						</span>
					</div>
				</div>
			</div>
		</div>
	);
}
