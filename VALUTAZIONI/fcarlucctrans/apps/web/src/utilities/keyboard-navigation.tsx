"use client"

import React, { useEffect, useLayoutEffect, useState } from "react"
import { useLocalization } from "./localization"

const KeyboardNavContext = React.createContext<{
	keyboardNav: boolean
	setKeyboardNav: (enabled: boolean) => void
} | undefined>({ keyboardNav: true, setKeyboardNav: () => {} })

function getSavedKeyboardNav(): boolean {
	if (typeof window === "undefined") {
		return true
	}
	const saved = localStorage.getItem("keyboardNav")
	return saved === "false" ? false : true
}

function applyKeyboardNav(enabled: boolean) {
	if (typeof window === "undefined") {
		return
	}
	if (enabled) {
		document.documentElement.classList.add("keyboard-nav")
		document.documentElement.dataset.inputModality = "keyboard"
	} else {
		document.documentElement.classList.remove("keyboard-nav")
		delete document.documentElement.dataset.inputModality
	}
}

export function KeyboardNavProvider({ children }: { children: React.ReactNode }) {
	const [keyboardNav, setKeyboardNav] = useState(true)

	useLayoutEffect(() => {
		setKeyboardNav(getSavedKeyboardNav())
	}, [])

	useLayoutEffect(() => {
		if (typeof window === "undefined") {
			return
		}
		localStorage.setItem("keyboardNav", keyboardNav.toString())
		applyKeyboardNav(keyboardNav)
	}, [keyboardNav])

	useEffect(() => {
		if (!keyboardNav || typeof window === "undefined") {
			return
		}

		const handleKeyDown = (event: KeyboardEvent) => {
			if (event.key === "Tab" || event.key.startsWith("Arrow")) {
				document.documentElement.dataset.inputModality = "keyboard"
			}
		}

		const handlePointerDown = () => {
			document.documentElement.dataset.inputModality = "pointer"
		}

		window.addEventListener("keydown", handleKeyDown, true)
		window.addEventListener("mousedown", handlePointerDown, true)
		window.addEventListener("touchstart", handlePointerDown, true)

		return () => {
			window.removeEventListener("keydown", handleKeyDown, true)
			window.removeEventListener("mousedown", handlePointerDown, true)
			window.removeEventListener("touchstart", handlePointerDown, true)
		}
	}, [keyboardNav])

	return (
		<KeyboardNavContext.Provider value={{ keyboardNav, setKeyboardNav }}>
			{children}
		</KeyboardNavContext.Provider>
	)
}

export function useKeyboardNav() {
	const context = React.useContext(KeyboardNavContext)
	if (context === undefined) {
		throw new Error("useKeyboardNav must be used within a KeyboardNavProvider")
	}
	return context
}

export const UseKeyboardNav = useKeyboardNav

export function UseKeyboardNavToggle() {
	const [checked, setChecked] = useState<boolean>(true)
	const { keyboardNav, setKeyboardNav } = useKeyboardNav()
	const { t } = useLocalization()

	useLayoutEffect(() => {
		setChecked(keyboardNav)
	}, [keyboardNav])

	const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
		setChecked(e.target.checked)
		setKeyboardNav(e.target.checked)
	}

	return (
		<div className="flex items-center gap-2">
			<input id="keyboard-nav" checked={checked} onChange={handleChange} type="checkbox" className="h-4 w-4" />
			<label id="keyboard-nav-label" htmlFor="keyboard-nav">{t.keyboardNavLabel}</label>
			<span id="keyboard-nav-help" className="text-sm opacity-80">
				Shows stronger focus rings while navigating with keyboard keys.
			</span>
		</div>
	)
}
