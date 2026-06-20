"use client";
import Navbar from "./Navbar";
import GlobalChatWidget from "@/components/GlobalChatWidget";
import { usePathname } from "next/navigation";
import Link from "next/link";
import { LanguageProvider } from "@/contexts/LanguageContext";
import { useTranslation } from "@/hooks/useTranslation";

function Footer() {
  const { t } = useTranslation();
  return (
    <footer style={{
      position: "fixed",
      bottom: 0,
      left: 0,
      width: "100%",
      background: "#262626",
      borderTop: "1px solid #555555",
      textAlign: "center",
      padding: "0.5rem 0",
      fontSize: "0.95rem",
      color: "#6b6b6b",
      zIndex: 100
    }}>
      <span style={{ display: "flex", justifyContent: "center", gap: "10rem" }}>
        <Link href="/privacy-policy" className="hover:text-white transition">{t("footer.privacy_policy")}</Link>
        <Link href="/terms-of-service" className="hover:text-white transition">{t("footer.terms_of_service")}</Link>
      </span>
    </footer>
  );
}

export default function ClientLayout({ children, isLoggedIn }: { children: React.ReactNode, isLoggedIn: boolean }) {
  const pathname = usePathname();

  const showNavbar = pathname !== "/friends" && !pathname.startsWith("/play/local/");
  return (
    <LanguageProvider>
      {showNavbar && (
        <div style={{
          position: "fixed",
          top: 0,
          left: 0,
          width: "100%",
          zIndex: 101
        }}>
          <Navbar isLoggedIn={isLoggedIn} />
        </div>
      )}
        <div style={{
          paddingTop: showNavbar ? "68px" : 0,   // adatta all'altezza reale della Navbar
          paddingBottom: showNavbar ? "40px" : 0, // stessa altezza del footer
          minHeight: "100vh",                     // minHeight invece di height
          boxSizing: "border-box",
          overflow: "auto",
          display: "flex",
          flexDirection: "column",
        }}>
        {children}
      </div>
      {showNavbar && <Footer />}
      {isLoggedIn && <GlobalChatWidget />}
    </LanguageProvider>
  );
}
