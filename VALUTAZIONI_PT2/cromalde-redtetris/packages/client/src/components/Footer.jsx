function Footer() {
  return (
    <footer className="fixed bottom-0 left-0 w-full z-40 flex flex-col md:flex-row justify-between items-center px-margin-desktop py-4 border-t-2 border-outline-variant bg-surface-container-lowest">
      <div className="font-label-caps text-label-caps text-on-surface mb-2 md:mb-0">
        MADE_BY_CRSLAR_FOR_ECOLE42_ROMA // [BLOCK_STRICT_PROTOCOL]
      </div>
      <nav className="flex gap-6 font-technical-sm text-technical-sm uppercase tracking-widest text-tertiary">
        <a className="text-outline hover:bg-surface-variant hover:text-primary transition-colors px-2 py-1" href="#">
          SYSTEM_STATUS
        </a>
        <a className="text-outline hover:bg-surface-variant hover:text-primary transition-colors px-2 py-1" href="#">
          RESOURCES
        </a>
        <a className="text-outline hover:bg-surface-variant hover:text-primary transition-colors px-2 py-1" href="#">
          TERMINAL_ACCESS
        </a>
      </nav>
    </footer>
  );
}

export default Footer;
