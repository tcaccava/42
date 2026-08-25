import { PIECES, getCells } from '@red-tetris/shared';
import { PIECE_RENDER } from '../tetris/render';

/**
 * Renders a single tetrimino in a centered bounding-box grid.
 * @param {Object} props
 * @param {import('@red-tetris/shared').PieceName} props.name
 * @param {number} [props.rotation]
 * @param {'sm'|'md'} [props.size]
 */
function PiecePreview({ name, rotation = 0, size = 'md' }) {
  const piece = PIECES[name];
  if (!piece) return null;

  const cells = getCells(name, rotation);
  const boxSize = piece.size;
  const cellSize = size === 'sm' ? 'w-5 h-5' : 'w-6 h-6';
  const render = PIECE_RENDER[name];

  const divs = [];
  for (let r = 0; r < boxSize; r++) {
    for (let c = 0; c < boxSize; c++) {
      const filled = cells.some(([cr, cc]) => cr === r && cc === c);
      divs.push(
        <div
          key={`${r}-${c}`}
          className={`${cellSize} ${filled ? `${render.colorClass} border ${render.glowClass}` : 'bg-white/3 border border-white/5'}`}
        />,
      );
    }
  }

  return (
    <div
      className="grid justify-center"
      style={{ gridTemplateColumns: `repeat(${boxSize}, auto)` }}
    >
      {divs}
    </div>
  );
}

export default PiecePreview;
