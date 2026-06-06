"use client"

import React, { useLayoutEffect, useState } from "react"
import { useLocalization } from "./localization"

const ReduceMotionContext = React.createContext<{
	reduceMotion: boolean
	setReduceMotion: (enabled: boolean) => void
} | undefined>({ reduceMotion: false, setReduceMotion: () => {} })

function getSavedReduceMotion(): boolean {
	if (typeof window === "undefined") {
		return false
	}
	const saved = localStorage.getItem("reduceMotion")
	return saved === "true"
}

export function ReduceMotionProvider({ children }: { children: React.ReactNode }) {
	const [reduceMotion, setReduceMotion] = useState(false)

	useLayoutEffect(() => {
		setReduceMotion(getSavedReduceMotion())
	}, [])

	useLayoutEffect(() => {
		if (typeof window === "undefined") {
			return
		}
		localStorage.setItem("reduceMotion", reduceMotion.toString())
		document.documentElement.classList.toggle("reduce-motion", reduceMotion)
	}, [reduceMotion])

	return (
		<ReduceMotionContext.Provider value={{ reduceMotion, setReduceMotion }}>
			{children}
		</ReduceMotionContext.Provider>
	)
}

export function useReduceMotion() {
	const context = React.useContext(ReduceMotionContext)
	if (context === undefined) {
		throw new Error("useReduceMotion must be used within a ReduceMotionProvider")
	}
	return context
}

export const UseReduceMotion = useReduceMotion

export function UseReduceMotionToggle() {
	const [checked, setChecked] = useState<boolean>(false)
	const { reduceMotion, setReduceMotion } = useReduceMotion()
	const { t } = useLocalization()

	useLayoutEffect(() => {
		setChecked(reduceMotion)
	}, [reduceMotion])

	const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
		setChecked(e.target.checked)
		setReduceMotion(e.target.checked)
	}

	return (
		<div className="flex items-center gap-2">
			<input id="reduce-motion" checked={checked} onChange={handleChange} type="checkbox" className="h-4 w-4" />
			<label id="reduce-motion-label" htmlFor="reduce-motion">{t.reduceMotionLabel}</label>
		</div>
	)
}
