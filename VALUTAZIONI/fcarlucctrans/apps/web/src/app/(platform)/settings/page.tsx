import type { Metadata } from "next"
import SettingsClient from "./settings-client"

export const metadata: Metadata = {
	title: "Settings",
	description: "Customize theme, text size, motion, and language preferences.",
}

export default function SettingsPage() {
	return <SettingsClient />
}

