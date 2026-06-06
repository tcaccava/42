import type { Metadata } from "next";
import { cookies } from "next/headers";
import { Geist, Geist_Mono } from "next/font/google";
import "./globals.css";
import { providers } from "@/utilities";
import { APPEARANCE_BOOT_SCRIPT } from "@/utilities/appearance/core";
import { LOCALE_BOOT_SCRIPT, LOCALE_STORAGE_KEY, normalizeLocale } from "@/utilities/i18n/core";

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const geistMono = Geist_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "42Share - Connect with 42 Students",
  description: "The social network for 42 students. Share projects, join events, and connect with peers.",
};

export default async function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const cookieStore = await cookies();
  const initialLocale = normalizeLocale(
    cookieStore.get(LOCALE_STORAGE_KEY)?.value ?? "en",
  );

  return (
    <html lang={initialLocale} suppressHydrationWarning>
      <head>
        <script
          dangerouslySetInnerHTML={{ __html: LOCALE_BOOT_SCRIPT + APPEARANCE_BOOT_SCRIPT }}
        />
      </head>
      <body
        className={`${geistSans.variable} ${geistMono.variable} antialiased`}
        suppressHydrationWarning
      >
        {/* SARA: The page context provider is the necessary provider for all the accessibility and internationalization settings */}
        <providers.PageContext initialLocale={initialLocale}>
          {children}
        </providers.PageContext>
      </body>
    </html>
  );
}
