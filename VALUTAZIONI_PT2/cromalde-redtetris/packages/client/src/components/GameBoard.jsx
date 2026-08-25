import { getCells, PENALTY } from '@red-tetris/shared';
import { cellClass } from '../tetris/render';

/**
 * @typedef {Object} ActivePiece
 * @property {import('@red-tetris/shared').PieceName} name
 * @property {number} rotation
 * @property {number} row
 * @property {number} col
 */

/**
 * Renders the 10x20 Tetris board as a CSS Grid of div cells.
 * @param {Object} props
 * @param {Array<Array<string|null>>} props.board
 * @param {ActivePiece|null} [props.activePiece]
 * @param {number|null} [props.ghostRow]
 */
function GameBoard({ board, activePiece, ghostRow }) {
  const rows = board.length;
  const cols = board[0].length;

  const activeSet = new Set();
  if (activePiece) {
    const cells = getCells(activePiece.name, activePiece.rotation);
    for (const [r, c] of cells) {
      activeSet.add(`${r + activePiece.row},${c + activePiece.col}`);
    }
  }

  const ghostSet = new Set();
  if (ghostRow !== null && ghostRow !== undefined && activePiece) {
    const cells = getCells(activePiece.name, activePiece.rotation);
    for (const [r, c] of cells) {
      ghostSet.add(`${r + ghostRow},${c + activePiece.col}`);
    }
  }

  const gridCells = [];
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      const key = `${row},${col}`;
      let type;
      let name;

      if (activeSet.has(key) && row >= 0) {
        type = 'active';
        name = activePiece.name;
      } else if (ghostSet.has(key) && !activeSet.has(key) && row >= 0) {
        type = 'ghost';
      } else if (board[row][col] === PENALTY) {
        type = 'penalty';
      } else if (board[row][col] !== null) {
        type = 'placed';
      } else {
        type = 'empty';
      }

      gridCells.push(
        <div key={key} className={`w-8 aspect-square ${cellClass(type, name)}`} />,
      );
    }
  }

  return (
    <div
      className="grid gap-0.5 bg-surface-container border-2 border-outline-variant p-0.5 shadow-[0_0_30px_rgba(255,95,0,0.1)]"
      style={{ gridTemplateColumns: `repeat(${cols}, 1fr)` }}
    >
      {gridCells}
    </div>
  );
}

export default GameBoard;
