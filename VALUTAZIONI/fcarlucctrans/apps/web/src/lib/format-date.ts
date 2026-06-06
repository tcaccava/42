import type { LocaleCode } from '@/utilities/i18n/core';

/** BCP 47 tags used for deterministic SSR + client date formatting. */
const BCP47: Record<LocaleCode, string> = {
	en: 'en-GB',
	fr: 'fr-FR',
	de: 'de-DE',
	it: 'it-IT',
	uk: 'uk-UA',
};

function parseInstant(iso: string): Date | null {
	const d = new Date(iso);
	return Number.isNaN(d.getTime()) ? null : d;
}

/** Format a UTC ISO timestamp as a short date (feed cards, etc.). */
export function formatAppDate(iso: string, locale: LocaleCode = 'en'): string {
	const d = parseInstant(iso);
	if (!d) return '';
	return d.toLocaleDateString(BCP47[locale] ?? 'en-GB', {
		month: 'short',
		day: 'numeric',
		year: 'numeric',
		timeZone: 'UTC',
	});
}

/** Format a UTC ISO timestamp as date + time (comments, etc.). */
export function formatAppDateTime(iso: string, locale: LocaleCode = 'en'): string {
	const d = parseInstant(iso);
	if (!d) return '';
	return d.toLocaleString(BCP47[locale] ?? 'en-GB', {
		year: 'numeric',
		month: 'short',
		day: 'numeric',
		hour: '2-digit',
		minute: '2-digit',
		timeZone: 'UTC',
	});
}
