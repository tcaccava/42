export type ThemePreference = 'light' | 'dark' | 'system';
export type ResolvedTheme = 'light' | 'dark';

export const APPEARANCE_STORAGE = {
	theme: 'theme',
	highContrast: 'highContrast',
} as const;

const VALID_THEMES: ThemePreference[] = ['light', 'dark', 'system'];

export function isThemePreference(value: string | null): value is ThemePreference {
	return value !== null && VALID_THEMES.includes(value as ThemePreference);
}

export function readThemePreference(): ThemePreference {
	if (typeof window === 'undefined') return 'system';

	const saved = localStorage.getItem(APPEARANCE_STORAGE.theme);
	return isThemePreference(saved) ? saved : 'system';
}

export function readHighContrast(): boolean {
	if (typeof window === 'undefined') return false;
	return localStorage.getItem(APPEARANCE_STORAGE.highContrast) === 'true';
}

export function systemPrefersDark(): boolean {
	if (typeof window === 'undefined') return false;
	return window.matchMedia('(prefers-color-scheme: dark)').matches;
}

export function resolveTheme(preference: ThemePreference): ResolvedTheme {
	if (preference === 'dark') return 'dark';
	if (preference === 'light') return 'light';
	return systemPrefersDark() ? 'dark' : 'light';
}

export function applyAppearance(
	preference: ThemePreference,
	highContrast: boolean,
): ResolvedTheme {
	if (typeof document === 'undefined') return resolveTheme(preference);

	const resolved = resolveTheme(preference);
	const root = document.documentElement;

	root.classList.toggle('dark', resolved === 'dark');
	root.classList.toggle('high-contrast', highContrast);
	root.dataset.theme = preference;
	root.dataset.resolvedTheme = resolved;
	root.style.colorScheme = resolved;

	return resolved;
}

/** Inline boot script — must stay in sync with applyAppearance(). */
export const APPEARANCE_BOOT_SCRIPT = `(function(){try{var k='theme',h='highContrast',t=localStorage.getItem(k),c=localStorage.getItem(h)==='true',d=t==='dark'||(t!=='light'&&window.matchMedia('(prefers-color-scheme: dark)').matches),r=document.documentElement;r.classList.toggle('dark',d);r.classList.toggle('high-contrast',c);r.dataset.theme=t||'system';r.dataset.resolvedTheme=d?'dark':'light';r.style.colorScheme=d?'dark':'light';}catch(e){}})();`;
