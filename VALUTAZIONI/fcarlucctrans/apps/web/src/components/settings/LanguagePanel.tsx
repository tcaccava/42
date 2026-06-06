'use client';

import { Check, Globe } from 'lucide-react';
import { LOCALE_META, LOCALES } from '@/utilities/i18n/core';
import { useLocalization } from '@/utilities/localization';

export function LanguagePanel() {
	const { locale, setLocale, t, tr } = useLocalization();

	return (
		<div className="space-y-4">
			<p className="text-sm text-foreground/70">{t.languageDescription}</p>

			<fieldset>
				<legend className="sr-only">{t.language}</legend>
				<div
					className="grid gap-3 sm:grid-cols-2"
					role="radiogroup"
					aria-label={t.language}
				>
					{LOCALES.map((code) => {
						const meta = LOCALE_META[code];
						const selected = locale === code;
						return (
							<button
								key={code}
								type="button"
								role="radio"
								aria-checked={selected}
								onClick={() => setLocale(code)}
								className={`group flex items-center gap-3 rounded-2xl border-2 p-4 text-left transition-all focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[var(--color-hvr-background)] focus-visible:ring-offset-2 focus-visible:ring-offset-background ${
									selected
										? 'border-[var(--btn-background)] bg-[var(--btn-background)]/5 shadow-sm'
										: 'border-border bg-background hover:border-[var(--btn-background)]/40 hover:bg-darker-background/60'
								}`}
							>
								<span
									className="flex h-10 w-10 shrink-0 items-center justify-center rounded-xl bg-darker-background text-xl"
									aria-hidden
								>
									{meta.flag}
								</span>
								<span className="min-w-0 flex-1">
									<span className="block text-sm font-bold text-foreground">
										{meta.nativeName}
									</span>
									<span className="block text-xs text-foreground/60">
										{meta.englishName}
									</span>
								</span>
								{selected && (
									<span className="rounded-full bg-[var(--btn-background)]/15 p-1 text-[var(--hvr-background)]">
										<Check className="h-4 w-4" aria-hidden />
									</span>
								)}
							</button>
						);
					})}
				</div>
			</fieldset>

			<div className="flex items-start gap-2 rounded-xl border border-border bg-darker-background/40 p-4 text-sm text-foreground/70">
				<Globe className="mt-0.5 h-4 w-4 shrink-0 text-[var(--hvr-background)]" aria-hidden />
				<p>{t.languageSavedNote}</p>
			</div>

			<p className="text-sm text-foreground/70" aria-live="polite">
				{tr('languageActiveStatus', { language: LOCALE_META[locale].nativeName })}
			</p>
		</div>
	);
}
