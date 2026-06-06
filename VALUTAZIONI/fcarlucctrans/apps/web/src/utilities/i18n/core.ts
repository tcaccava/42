import en from '../../locales/en.json';
import fr from '../../locales/fr.json';
import de from '../../locales/de.json';
import it from '../../locales/it.json';
import uk from '../../locales/uk.json';

export const LOCALES = ['en', 'fr', 'de', 'it', 'uk'] as const;
export type LocaleCode = (typeof LOCALES)[number];

export const LOCALE_STORAGE_KEY = 'language';

export const LOCALE_META: Record<
	LocaleCode,
	{ nativeName: string; englishName: string; flag: string }
> = {
	en: { nativeName: 'English', englishName: 'English', flag: '🇬🇧' },
	fr: { nativeName: 'Français', englishName: 'French', flag: '🇫🇷' },
	de: { nativeName: 'Deutsch', englishName: 'German', flag: '🇩🇪' },
	it: { nativeName: 'Italiano', englishName: 'Italian', flag: '🇮🇹' },
	uk: { nativeName: 'Українська', englishName: 'Ukrainian', flag: '🇺🇦' },
};

const LOCALE_MESSAGES: Record<LocaleCode, Record<string, string>> = {
	en,
	fr,
	de,
	it,
	uk,
};

export function normalizeLocale(value: string | null | undefined): LocaleCode {
	const code = (value ?? 'en').split('-')[0]?.toLowerCase();
	return LOCALES.includes(code as LocaleCode) ? (code as LocaleCode) : 'en';
}

export function readStoredLocale(): LocaleCode {
	if (typeof window === 'undefined') return 'en';
	return normalizeLocale(localStorage.getItem(LOCALE_STORAGE_KEY));
}

/** Persist locale in a cookie so SSR matches the user's language choice. */
export function writeLocaleCookie(locale: LocaleCode): void {
	if (typeof document === 'undefined') return;
	const secure =
		window.location.protocol === 'https:' ? ';secure' : '';
	document.cookie = `${LOCALE_STORAGE_KEY}=${locale};path=/;max-age=31536000;sameSite=lax${secure}`;
}

export function interpolate(
	template: string,
	params?: Record<string, string | number>,
): string {
	if (!params) return template;
	return Object.entries(params).reduce(
		(text, [key, value]) => text.replaceAll(`{${key}}`, String(value)),
		template,
	);
}

/** Resolve a message with English fallback for missing keys. */
export function translate(
	locale: LocaleCode,
	key: string,
	params?: Record<string, string | number>,
): string {
	const localized = LOCALE_MESSAGES[locale][key];
	const fallback = LOCALE_MESSAGES.en[key];
	const text = localized ?? fallback ?? key;

	if (
		process.env.NODE_ENV === 'development' &&
		localized === undefined &&
		fallback === undefined
	) {
		console.warn(`[i18n] Missing translation key: "${key}"`);
	}

	return interpolate(text, params);
}

/** Proxy so existing `t.someKey` call sites keep working with fallback. */
export function buildMessages(locale: LocaleCode): Record<string, string> {
	return new Proxy({} as Record<string, string>, {
		get(_target, prop) {
			if (typeof prop !== 'string') return undefined;
			return translate(locale, prop);
		},
	});
}

export function applyDocumentLocale(locale: LocaleCode): void {
	if (typeof document === 'undefined') return;
	document.documentElement.lang = locale;
	document.documentElement.dataset.locale = locale;
}

export const LOCALE_BOOT_SCRIPT = `(function(){try{var k='language',l=localStorage.getItem(k),c=['en','fr','de','it','uk'],n=(l||'en').split('-')[0].toLowerCase();if(c.indexOf(n)===-1)n='en';document.documentElement.lang=n;document.documentElement.dataset.locale=n;if(l&&document.cookie.indexOf(k+'=')===-1){var s=location.protocol==='https:'?';secure':'';document.cookie=k+'='+n+';path=/;max-age=31536000;sameSite=lax'+s;}}catch(e){}})();`;
