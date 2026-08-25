import { getCells } from './pieces.js';

/** Marker for indestructible penalty rows. */
const PENALTY = 'PENALTY';

/**
 * @typedef {Array<Array<string|null>>} Board
 */

/**
 * Creates an empty board.
 * @param {number} rows
 * @param {number} cols
 * @returns {Board}
 */
function createBoard(rows, cols) {
  return Array.from({ length: rows }, () => Array(cols).fill(null));
}

/**
 * Checks collision for piece cells at (row, col).
 * @param {Board} board
 * @param {string} pieceName
 * @param {number} rotation
 * @param {number} row
 * @param {number} col
 * @returns {boolean}
 */
function checkCollision(board, pieceName, rotation, row, col) {
  const cells = getCells(pieceName, rotation);
  const rows = board.length;
  const cols = board[0].length;

  for (const [r, c] of cells) {
    const boardRow = row + r;
    const boardCol = col + c;
    if (boardRow < 0 || boardRow >= rows || boardCol < 0 || boardCol >= cols) {
      return true;
    }
    if (board[boardRow][boardCol] !== null) {
      return true;
    }
  }
  return false;
}

/**
 * Places piece cells on a copy of the board.
 * @param {Board} board
 * @param {string} pieceName
 * @param {number} rotation
 * @param {number} row
 * @param {number} col
 * @returns {Board}
 */
function placePiece(board, pieceName, rotation, row, col) {
  const cells = getCells(pieceName, rotation);
  const next = board.map((r) => [...r]);
  for (const [r, c] of cells) {
    const boardRow = row + r;
    const boardCol = col + c;
    if (
      boardRow >= 0 &&
      boardRow < next.length &&
      boardCol >= 0 &&
      boardCol < next[0].length
    ) {
      next[boardRow][boardCol] = pieceName;
    }
  }
  return next;
}

/**
 * Removes full lines (rows with no null and no PENALTY cells).
 * @param {Board} board
 * @returns {{ board: Board, linesCleared: number }}
 */
function clearLines(board) {
  const remaining = board.filter((row) => {
    const hasNull = row.some((cell) => cell === null);
    const hasPenalty = row.some((cell) => cell === PENALTY);
    return hasNull || hasPenalty;
  });
  const linesCleared = board.length - remaining.length;
  const emptyRows = Array.from({ length: linesCleared }, () =>
    Array(board[0].length).fill(null),
  );
  return { board: [...emptyRows, ...remaining], linesCleared };
}

/**
 * Adds indestructible penalty rows from the bottom up.
 * Converts the bottommost non-PENALTY rows to PENALTY.
 * Returns gameOver if the spawn zone (top 4 rows) would be affected.
 * @param {Board} board
 * @param {number} count — number of penalty rows to add
 * @returns {{ board: Board, gameOver: boolean }}
 */
function addPenaltyLines(board, count) {
  const rows = board.length;
  const cols = board[0].length;
  const result = board.map((r) => [...r]);

  for (let i = 0; i < count; i++) {
    let targetRow = -1;
    for (let r = rows - 1; r >= 0; r--) {
      if (!result[r].every((cell) => cell === PENALTY)) {
        targetRow = r;
        break;
      }
    }

    if (targetRow === -1 || targetRow < 4) {
      return { board: result, gameOver: true };
    }

    for (let c = 0; c < cols; c++) {
      result[targetRow][c] = PENALTY;
    }
  }

  return { board: result, gameOver: false };
}

/**
 * Returns the lowest valid row for a piece (ghost position).
 * @param {Board} board
 * @param {string} pieceName
 * @param {number} rotation
 * @param {number} col
 * @param {number} startRow
 * @returns {number}
 */
function getGhostRow(board, pieceName, rotation, col, startRow) {
  let row = startRow;
  while (!checkCollision(board, pieceName, rotation, row + 1, col)) {
    row++;
  }
  return row;
}

/**
 * Returns all column indices where a piece can be placed without collision.
 * @param {Board} board
 * @param {string} pieceName
 * @param {number} rotation
 * @param {number} row
 * @returns {number[]}
 */
function getValidCols(board, pieceName, rotation, row) {
  const valid = [];
  for (let c = 0; c < board[0].length; c++) {
    if (!checkCollision(board, pieceName, rotation, row, c)) valid.push(c);
  }
  return valid;
}

export {
  PENALTY,
  createBoard,
  checkCollision,
  placePiece,
  clearLines,
  addPenaltyLines,
  getGhostRow,
  getValidCols,
};
