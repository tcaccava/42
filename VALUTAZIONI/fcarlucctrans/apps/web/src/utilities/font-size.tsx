"use client"

import React, { useLayoutEffect, useState } from "react"
import { useLocalization } from "./localization"

const FontSizeContext = React.createContext<{
	fontSize: number
	setFontSize: (size: number) => void
}>({
	fontSize: 1,
	setFontSize: () => {},
})

function getSavedFontSize(): number {
	if (typeof window === "undefined") {
		return 1
	}
	const saved = localStorage.getItem("fontSize")
	const parsed = saved ? parseFloat(saved) : 1
	return isNaN(parsed) ? 1 : parsed
}

export function FontSizeProvider({ children }: { children: React.ReactNode }) {
	const [fontSize, setFontSize] = useState(1)

	useLayoutEffect(() => {
		setFontSize(getSavedFontSize())
	}, [])

	useLayoutEffect(() => {
		if (typeof window === "undefined") {
			return
		}
		localStorage.setItem("fontSize", fontSize.toString())
		document.documentElement.style.setProperty("--font-size-multiplier", fontSize.toString())
	}, [fontSize])

	return (
		<FontSizeContext.Provider value={{ fontSize, setFontSize }}>
			{children}
		</FontSizeContext.Provider>
	)
}

export function useFontSize() {
	return React.useContext(FontSizeContext) as {
		fontSize: number
		setFontSize: (size: number) => void
	}
}

export const UseFontSize = useFontSize

export function UseFontSizeControl() {
	const [sliderValue, setSliderValue] = useState<number>(0.8)
	const { fontSize, setFontSize } = useFontSize()
	const { t } = useLocalization()

	useLayoutEffect(() => {
		setSliderValue(fontSize)
	}, [fontSize])

	const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
		const newValue = Number(e.target.value)
		setSliderValue(newValue)
		setFontSize(newValue)
	}

	return (
		<div className="form-group">
			<label id="font-size-label" htmlFor="font-size" className="block font-medium mb-2">{t.fontsizeLabel}</label>
			<input
				id="font-size"
				type="range"
				min={0.8}
				max={1.6}
				step={0.05}
				value={sliderValue}
				onChange={handleChange}
				aria-labelledby="font-size-label"
				aria-describedby="font-size-help"
				aria-valuemin={0.8}
				aria-valuemax={1.6}
				aria-valuenow={sliderValue}
				aria-valuetext={`${(sliderValue * 100).toFixed(0)}% of normal size`}
				className="w-full"
			/>
			<small id="font-size-help" className="block mt-1 text-sm text-gray-600">
				Current size: <strong>{(sliderValue * 100).toFixed(0)}%</strong> (minimum 80%, maximum 160%)
			</small>
		</div>
	)
}
