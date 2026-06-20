import type { Metadata } from "next";
import { Geist, Geist_Mono } from "next/font/google";
import Script from "next/script";
import "./globals.css";

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const geistMono = Geist_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "Trivia Master",
  description: "Transcendence project",
  icons: {
    icon: "/favicon.ico",
    apple: "/apple-touch-icon.png",
  },
};

import { cookies } from "next/headers";

export default async function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const cookieStore = await cookies();
  const API_BASE = process.env.BACKEND_URL || 'http://localhost:8000';
  const response = await fetch(`${API_BASE}/api/get_session`, {
    headers: { Cookie: cookieStore.toString() },
    cache: 'no-store'
  }).catch(() => null);

  const isLoggedIn = response?.ok ?? false;

  const ClientLayout = require("./ClientLayout").default;
  return (
    <html lang="en">
      <body suppressHydrationWarning>
        <ClientLayout isLoggedIn={isLoggedIn}>{children}</ClientLayout>
        <Script
          src="https://embeds.iubenda.com/widgets/8f9cfe2b-1c0f-49d2-93ba-d6297133b739.js"
          strategy="lazyOnload"
        />
      </body>
    </html>
  );
}
