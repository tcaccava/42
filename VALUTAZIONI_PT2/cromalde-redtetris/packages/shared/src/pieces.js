/**
 * @typedef {'I'|'O'|'T'|'S'|'Z'|'J'|'L'} PieceName
 */

/**
 * @typedef {Object} PieceDef
 * @property {PieceName} name
 * @property {number} size - Bounding box dimension
 * @property {Array<Array<[number, number]>>} cells - 4 rotations of [row,col] offsets
 */

/** @type {Object<PieceName, PieceDef>} */
const PIECES = {
  I: {
    name: 'I',
    size: 4,
    cells: [
      [[1, 0], [1, 1], [1, 2], [1, 3]],
      [[0, 2], [1, 2], [2, 2], [3, 2]],
      [[2, 0], [2, 1], [2, 2], [2, 3]],
      [[0, 1], [1, 1], [2, 1], [3, 1]],
    ],
  },
  O: {
    name: 'O',
    size: 4,
    cells: [
      [[1, 1], [1, 2], [2, 1], [2, 2]],
      [[1, 1], [1, 2], [2, 1], [2, 2]],
      [[1, 1], [1, 2], [2, 1], [2, 2]],
      [[1, 1], [1, 2], [2, 1], [2, 2]],
    ],
  },
  T: {
    name: 'T',
    size: 3,
    cells: [
      [[0, 1], [1, 0], [1, 1], [1, 2]],
      [[0, 1], [1, 1], [1, 2], [2, 1]],
      [[1, 0], [1, 1], [1, 2], [2, 1]],
      [[0, 1], [1, 0], [1, 1], [2, 1]],
    ],
  },
  S: {
    name: 'S',
    size: 3,
    cells: [
      [[0, 1], [0, 2], [1, 0], [1, 1]],
      [[0, 1], [1, 1], [1, 2], [2, 2]],
      [[1, 1], [1, 2], [2, 0], [2, 1]],
      [[0, 0], [1, 0], [1, 1], [2, 1]],
    ],
  },
  Z: {
    name: 'Z',
    size: 3,
    cells: [
      [[0, 0], [0, 1], [1, 1], [1, 2]],
      [[0, 2], [1, 1], [1, 2], [2, 1]],
      [[1, 0], [1, 1], [2, 1], [2, 2]],
      [[0, 1], [1, 0], [1, 1], [2, 0]],
    ],
  },
  J: {
    name: 'J',
    size: 3,
    cells: [
      [[0, 0], [1, 0], [1, 1], [1, 2]],
      [[0, 1], [0, 2], [1, 1], [2, 1]],
      [[1, 0], [1, 1], [1, 2], [2, 2]],
      [[0, 1], [1, 1], [2, 0], [2, 1]],
    ],
  },
  L: {
    name: 'L',
    size: 3,
    cells: [
      [[0, 2], [1, 0], [1, 1], [1, 2]],
      [[0, 1], [1, 1], [2, 1], [2, 2]],
      [[1, 0], [1, 1], [1, 2], [2, 0]],
      [[0, 0], [0, 1], [1, 1], [2, 1]],
    ],
  },
};

/**
 * Returns the cell offsets for a given piece and rotation.
 * @param {PieceName} name
 * @param {number} rotation - 0-3
 * @returns {Array<[number, number]>}
 */
function getCells(name, rotation) {
  const piece = PIECES[name];
  if (!piece) return [];
  return piece.cells[rotation & 3];
}

/**
 * Returns the next rotation index (0-3) given current and direction.
 * @param {number} current - current rotation 0-3
 * @param {number} direction - 1 for CW, -1 for CCW
 * @returns {number}
 */
function nextRotation(current, direction) {
  return (current + direction + 4) & 3;
}

export { PIECES, getCells, nextRotation };
