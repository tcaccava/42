"use client"

import React, { useLayoutEffect, useState } from "react"
import { useLocalization } from "./localization"

const TextSpacingContext = React.createContext<{
	textSpacing: boolean
	setTextSpacing: (enabled: boolean) => void
} | undefined>({ textSpacing: false, setTextSpacing: () => {} })

function getSavedTextSpacing(): boolean {
	if (typeof window === "undefined") {
		return false
	}
	const saved = localStorage.getItem("textSpacing")
	return saved === "true"
}

export function TextSpacingProvider({ children }: { children: React.ReactNode }) {
	const [textSpacing, setTextSpacing] = useState(false)

	useLayoutEffect(() => {
		setTextSpacing(getSavedTextSpacing())
	}, [])

	useLayoutEffect(() => {
		if (typeof window === "undefined") {
			return
		}
		localStorage.setItem("textSpacing", textSpacing.toString())
		document.documentElement.classList.toggle("text-spacing-enhanced", textSpacing)
	}, [textSpacing])

	return (
		<TextSpacingContext.Provider value={{ textSpacing, setTextSpacing }}>
			{children}
		</TextSpacingContext.Provider>
	)
}

export function useTextSpacing() {
	const context = React.useContext(TextSpacingContext)
	if (context === undefined) {
		throw new Error("useTextSpacing must be used within a TextSpacingProvider")
	}
	return context
}

export const UseTextSpacing = useTextSpacing

export function UseTextSpacingToggle() {
	const [checked, setChecked] = useState<boolean>(false)
	const { textSpacing, setTextSpacing } = useTextSpacing()
	const { t } = useLocalization()

	useLayoutEffect(() => {
		setChecked(textSpacing)
	}, [textSpacing])

	const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
		setChecked(e.target.checked)
		setTextSpacing(e.target.checked)
	}

	return (
		<div className="flex items-center gap-2">
			<input id="text-spacing" checked={checked} onChange={handleChange} type="checkbox" className="h-4 w-4" />
			<label id="text-spacing-label" htmlFor="text-spacing">{t.textSpacingLabel}</label>
		</div>
	)
}
