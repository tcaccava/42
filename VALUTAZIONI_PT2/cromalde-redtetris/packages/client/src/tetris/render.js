/** @type {Object<import('@red-tetris/shared').PieceName, {colorClass: string, glowClass: string}>} */
const PIECE_RENDER = {
  I: { colorClass: 'bg-secondary-container border-secondary-container', glowClass: 'piece-glow-I' },
  O: { colorClass: 'bg-primary border-primary', glowClass: 'piece-glow-O' },
  T: { colorClass: 'bg-tertiary-container border-tertiary-container', glowClass: 'piece-glow-T' },
  S: { colorClass: 'bg-secondary-fixed-dim border-secondary-fixed-dim', glowClass: 'piece-glow-S' },
  Z: { colorClass: 'bg-error border-error', glowClass: 'piece-glow-Z' },
  J: { colorClass: 'bg-tertiary-fixed-dim border-tertiary-fixed-dim', glowClass: 'piece-glow-J' },
  L: { colorClass: 'bg-primary-container border-primary-container', glowClass: 'piece-glow-L' },
};

/**
 * Returns Tailwind class string for a board cell.
 * @param {'empty'|'active'|'ghost'|'placed'|'penalty'} type
 * @param {import('@red-tetris/shared').PieceName} [pieceName]
 * @returns {string}
 */
function cellClass(type, pieceName) {
  switch (type) {
    case 'empty':
      return 'bg-white/[0.03] border border-white/[0.05]';
    case 'ghost':
      return 'bg-transparent border border-dashed border-primary/50';
    case 'penalty':
      return 'bg-surface-container-high border border-primary/30 border-l-2 border-r-2';
    case 'placed':
      return 'bg-surface-variant border border-surface-variant';
    case 'active': {
      if (!pieceName || !PIECE_RENDER[pieceName]) return '';
      const p = PIECE_RENDER[pieceName];
      return `${p.colorClass} border ${p.glowClass}`;
    }
    default:
      return '';
  }
}

export { cellClass, PIECE_RENDER };
