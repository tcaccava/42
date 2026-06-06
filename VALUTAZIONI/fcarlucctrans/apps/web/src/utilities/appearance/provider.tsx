'use client';

import React, {
	createContext,
	useCallback,
	useContext,
	useLayoutEffect,
	useMemo,
	useState,
} from 'react';
import {
	APPEARANCE_STORAGE,
	applyAppearance,
	readHighContrast,
	readThemePreference,
	resolveTheme,
	systemPrefersDark,
	type ResolvedTheme,
	type ThemePreference,
} from './core';

type AppearanceContextValue = {
	theme: ThemePreference;
	resolvedTheme: ResolvedTheme;
	highContrast: boolean;
	setTheme: (theme: ThemePreference) => void;
	setHighContrast: (enabled: boolean) => void;
};

const AppearanceContext = createContext<AppearanceContextValue | undefined>(
	undefined,
);

export function AppearanceProvider({ children }: { children: React.ReactNode }) {
	const [theme, setThemeState] = useState<ThemePreference>('system');
	const [highContrast, setHighContrastState] = useState(false);
	const [resolvedTheme, setResolvedTheme] = useState<ResolvedTheme>(() => {
		if (typeof document === 'undefined') return 'light';
		return document.documentElement.dataset.resolvedTheme === 'dark' ? 'dark' : 'light';
	});
	const [ready, setReady] = useState(false);

	useLayoutEffect(() => {
		const savedTheme = readThemePreference();
		const savedContrast = readHighContrast();
		setThemeState(savedTheme);
		setHighContrastState(savedContrast);
		setResolvedTheme(applyAppearance(savedTheme, savedContrast));
		setReady(true);
	}, []);

	useLayoutEffect(() => {
		if (!ready) return;
		localStorage.setItem(APPEARANCE_STORAGE.theme, theme);
		setResolvedTheme(applyAppearance(theme, highContrast));
	}, [theme, highContrast, ready]);

	useLayoutEffect(() => {
		if (!ready || theme !== 'system') return;

		const media = window.matchMedia('(prefers-color-scheme: dark)');
		const onChange = () => {
			setResolvedTheme(applyAppearance('system', highContrast));
		};

		media.addEventListener('change', onChange);
		return () => media.removeEventListener('change', onChange);
	}, [theme, highContrast, ready]);

	const setTheme = useCallback((next: ThemePreference) => {
		setThemeState(next);
	}, []);

	const setHighContrast = useCallback((enabled: boolean) => {
		setHighContrastState(enabled);
		localStorage.setItem(APPEARANCE_STORAGE.highContrast, String(enabled));
	}, []);

	const value = useMemo(
		() => ({
			theme,
			resolvedTheme,
			highContrast,
			setTheme,
			setHighContrast,
		}),
		[theme, resolvedTheme, highContrast, setTheme, setHighContrast],
	);

	return (
		<AppearanceContext.Provider value={value}>
			{children}
		</AppearanceContext.Provider>
	);
}

export function useAppearance() {
	const context = useContext(AppearanceContext);
	if (!context) {
		throw new Error('useAppearance must be used within an AppearanceProvider');
	}
	return context;
}

/** Useful for status labels without subscribing to the full context. */
export function getResolvedThemeLabel(preference: ThemePreference): ResolvedTheme {
	return preference === 'system' ? resolveTheme('system') : preference;
}

export function useSystemThemeChanges(callback: () => void) {
	useLayoutEffect(() => {
		const media = window.matchMedia('(prefers-color-scheme: dark)');
		const handler = () => callback();
		media.addEventListener('change', handler);
		return () => media.removeEventListener('change', handler);
	}, [callback]);
}

export { systemPrefersDark };
