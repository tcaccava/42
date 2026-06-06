"use client"

/**
 * Backward-compatible re-exports. Contrast logic lives in `utilities/appearance`.
 */
import { useAppearance } from './appearance';
import { Switch } from '@/components/ui/Switch';
import { useLocalization } from './localization';

export const HighContrastProvider = ({
	children,
}: {
	children: React.ReactNode;
}) => children;

export function UseHighContrast() {
	const { highContrast, setHighContrast } = useAppearance();
	return { highContrast, setHighContrast };
}

export const useHighContrast = UseHighContrast;

/** @deprecated Use ThemeDisplayPanel from @/components/settings/ThemeDisplayPanel */
export function UseHighContrastToggle() {
	const { t } = useLocalization();
	const { highContrast, setHighContrast } = UseHighContrast();

	return (
		<Switch
			id="high-contrast-legacy"
			checked={highContrast}
			onCheckedChange={setHighContrast}
			label={t.highContrastLabel}
			description={t.highContrastDescription}
		/>
	);
}
