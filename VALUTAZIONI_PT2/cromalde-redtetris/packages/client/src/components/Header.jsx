function Header() {
  return (
    <header className="fixed top-0 left-0 w-full z-50 flex justify-between items-center px-margin-desktop py-unit overflow-hidden crt-lines bg-surface/80 backdrop-blur-xl border-b-2 border-outline-variant shadow-[0_0_15px_rgba(255,181,153,0.1)]">
      <div className="relative flex items-center gap-4">
        <span className="material-symbols-outlined text-primary text-3xl" style={{ fontVariationSettings: "'FILL' 1" }}>
          grid_4x4
        </span>
        <h1 className="font-display-lg text-display-lg text-primary drop-shadow-[0_0_8px_rgba(255,181,153,0.4)] tracking-tighter">
          RED_TETRIS_v4.2
        </h1>
      </div>
      <div className="font-technical-sm text-technical-sm text-primary opacity-80 flicker-text">
        23:59:59_UTC
      </div>
    </header>
  );
}

export default Header;
