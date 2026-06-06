"use client"

/**
 * Backward-compatible re-exports. Theme logic lives in `utilities/appearance`.
 */
import {
	AppearanceProvider,
	useAppearance,
	type ThemePreference,
} from './appearance';
import { useLocalization } from './localization';

export type Theme = ThemePreference;

export const DarkModeProvider = AppearanceProvider;

export function useDarkMode() {
	const { theme, setTheme } = useAppearance();
	return { theme, setTheme };
}

export const UseDarkMode = useDarkMode;

/** @deprecated Use ThemeDisplayPanel from @/components/settings/ThemeDisplayPanel */
export function DarkModeSelector({ className }: { className?: string }) {
	const { theme, setTheme } = useAppearance();
	const { t } = useLocalization();

	return (
		<div className={className}>
			<label id="theme-label" htmlFor="theme" className="block font-medium">
				{t.themeLabel}
			</label>
			<select
				id="theme"
				value={theme}
				onChange={(e) => setTheme(e.target.value as ThemePreference)}
				className="w-full rounded-lg border-2 border-border bg-background px-4 py-2 text-foreground"
			>
				<option value="light">{t.themeLight}</option>
				<option value="dark">{t.themeDark}</option>
				<option value="system">{t.themeSystem}</option>
			</select>
		</div>
	);
}
