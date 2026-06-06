"use client"

import React, { useCallback, useLayoutEffect, useMemo, useState } from "react"
import {
	LOCALES,
	LOCALE_META,
	LOCALE_STORAGE_KEY,
	applyDocumentLocale,
	buildMessages,
	normalizeLocale,
	readStoredLocale,
	translate,
	writeLocaleCookie,
	type LocaleCode,
} from "./i18n/core"

type LocalizationContextValue = {
	locale: LocaleCode
	setLocale: (loc: string) => void
	t: Record<string, string>
	tr: (key: string, params?: Record<string, string | number>) => string
}

const LocalizationContext = React.createContext<LocalizationContextValue>({
	locale: "en",
	setLocale: () => {},
	t: buildMessages("en"),
	tr: (key, params) => translate("en", key, params),
})

type LocalizationProviderProps = {
	children: React.ReactNode
	initialLocale?: LocaleCode
}

export function LocalizationProvider({
	children,
	initialLocale = "en",
}: LocalizationProviderProps) {
	const [locale, setLocaleState] = useState<LocaleCode>(initialLocale)

	const applyLocale = useCallback((next: string) => {
		const normalized = normalizeLocale(next)
		setLocaleState(normalized)
		if (typeof window !== "undefined") {
			localStorage.setItem(LOCALE_STORAGE_KEY, normalized)
			writeLocaleCookie(normalized)
		}
		applyDocumentLocale(normalized)
	}, [])

	useLayoutEffect(() => {
		const stored = readStoredLocale()
		if (stored !== initialLocale) {
			applyLocale(stored)
			return
		}
		applyDocumentLocale(stored)
		writeLocaleCookie(stored)
	}, [applyLocale, initialLocale])

	const t = useMemo(() => buildMessages(locale), [locale])

	const tr = useCallback(
		(key: string, params?: Record<string, string | number>) =>
			translate(locale, key, params),
		[locale],
	)

	const value = useMemo(
		() => ({ locale, setLocale: applyLocale, t, tr }),
		[locale, applyLocale, t, tr],
	)

	return (
		<LocalizationContext.Provider value={value}>
			{children}
		</LocalizationContext.Provider>
	)
}

export function useLocalization() {
	return React.useContext(LocalizationContext)
}

/** @deprecated Use LanguagePanel from @/components/settings/LanguagePanel */
export function UseLocalization({ className }: { className?: string }) {
	const { locale, setLocale } = useLocalization()
	return (
		<div className={className}>
			<label htmlFor="locale-select-legacy">{translate(locale, "localeSelectLabel")}</label>
			<select
				id="locale-select-legacy"
				value={locale}
				onChange={(e) => setLocale(normalizeLocale(e.target.value))}
				className="w-full rounded-lg border-2 border-border bg-background px-4 py-2 text-foreground"
			>
				{LOCALES.map((code) => (
					<option key={code} value={code}>
						{LOCALE_META[code].nativeName}
					</option>
				))}
			</select>
		</div>
	)
}

export { LOCALES, LOCALE_META, normalizeLocale, type LocaleCode }
