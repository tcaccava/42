import { useContext } from "react";
import { LanguageContext } from "../contexts/LanguageContext";
import en from "../locales/en.json";

const englishTranslations: Record<string, string> = en;

/**
 * Hook that provides translation functionality with a fallback chain:
 * 1. Look up key in active language map → return value if found
 * 2. Look up key in English map → return value if found
 * 3. Return the key string itself
 */
export function useTranslation() {
  const context = useContext(LanguageContext);

  if (!context) {
    throw new Error("useTranslation must be used within a LanguageProvider");
  }

  const { language, translations } = context;

  const t = (key: string): string => {
    // 1. Look up in active language translations
    if (key in translations && translations[key] !== undefined) {
      return translations[key];
    }

    // 2. Fall back to English translations
    if (key in englishTranslations && englishTranslations[key] !== undefined) {
      return englishTranslations[key];
    }

    // 3. Return the key itself
    return key;
  };

  return { t, language };
}
