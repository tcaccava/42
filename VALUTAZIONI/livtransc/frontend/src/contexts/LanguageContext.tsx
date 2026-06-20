"use client";

import React, { createContext, useState, useEffect, useContext, ReactNode } from "react";
import en from "../locales/en.json";
import it from "../locales/it.json";
import es from "../locales/es.json";

export type Language = "en" | "it" | "es";

export type TranslationMap = Record<string, string>;

export interface LanguageContextValue {
  language: Language;
  setLanguage: (lang: Language) => void;
  translations: TranslationMap;
}

const STORAGE_KEY = "transcendence_language";
const SUPPORTED_LANGUAGES: Language[] = ["en", "it", "es"];

const translationFiles: Record<Language, TranslationMap> = {
  en,
  it,
  es,
};

function isValidLanguage(value: unknown): value is Language {
  return (
    typeof value === "string" &&
    SUPPORTED_LANGUAGES.includes(value as Language)
  );
}

export const LanguageContext = createContext<LanguageContextValue | undefined>(
  undefined
);

interface LanguageProviderProps {
  children: ReactNode;
}

export function LanguageProvider({ children }: LanguageProviderProps) {
  const [language, setLanguageState] = useState<Language>("en");

  useEffect(() => {
    try {
      const stored = localStorage.getItem(STORAGE_KEY);
      if (isValidLanguage(stored)) {
        setLanguageState(stored);
      }
    } catch {
      // localStorage unavailable (e.g., private browsing)
    }
  }, []);

  const setLanguage = (lang: Language) => {
    setLanguageState(lang);
    try {
      localStorage.setItem(STORAGE_KEY, lang);
    } catch {
      // localStorage unavailable — change applies for current session only
    }
  };

  const value: LanguageContextValue = {
    language,
    setLanguage,
    translations: translationFiles[language],
  };

  return (
    <LanguageContext.Provider value={value}>
      {children}
    </LanguageContext.Provider>
  );
}

export function useLanguageContext(): LanguageContextValue {
  const context = useContext(LanguageContext);
  if (!context) {
    throw new Error("useLanguageContext must be used within a LanguageProvider");
  }
  return context;
}
