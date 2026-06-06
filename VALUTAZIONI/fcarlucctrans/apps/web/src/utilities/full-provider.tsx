'use client'

import React from 'react'

import { LocalizationProvider, type LocaleCode } from './localization'
import { AppearanceProvider } from './appearance'
import { KeyboardNavProvider } from './keyboard-navigation'
import { ReduceMotionProvider } from './reduce-motion'
import { TextSpacingProvider } from './text-spacing'
import { FontSizeProvider } from './font-size'

export function PageContext({
	children,
	initialLocale,
}: {
	children: React.ReactNode
	initialLocale?: LocaleCode
}) {
	return (
		<LocalizationProvider initialLocale={initialLocale}>
			<AppearanceProvider>
				<KeyboardNavProvider>
					<ReduceMotionProvider>
						<TextSpacingProvider>
							<FontSizeProvider>
								{children}
							</FontSizeProvider>
						</TextSpacingProvider>
					</ReduceMotionProvider>
				</KeyboardNavProvider>
			</AppearanceProvider>
		</LocalizationProvider>
	)
}
