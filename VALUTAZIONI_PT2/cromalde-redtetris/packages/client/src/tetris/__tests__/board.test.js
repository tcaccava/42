import { describe, it, expect, beforeEach } from 'vitest';
import { createBoard, checkCollision, placePiece, clearLines, getGhostRow, getValidCols } from '../board';

describe('createBoard', () => {
  it('creates a 20x10 board filled with null', () => {
    const board = createBoard(20, 10);
    expect(board).toHaveLength(20);
    for (const row of board) {
      expect(row).toHaveLength(10);
      for (const cell of row) {
        expect(cell).toBeNull();
      }
    }
  });

  it('creates a 1x1 board', () => {
    const board = createBoard(1, 1);
    expect(board[0][0]).toBeNull();
  });

  it('rows are independent arrays', () => {
    const board = createBoard(3, 3);
    board[0][0] = 'T';
    expect(board[1][0]).toBeNull();
  });
});

describe('checkCollision', () => {
  let board;

  beforeEach(() => {
    board = createBoard(20, 10);
  });

  it('returns false for piece in empty space', () => {
    expect(checkCollision(board, 'T', 0, 0, 3)).toBe(false);
  });

  it('returns true when piece touches bottom border', () => {
    expect(checkCollision(board, 'T', 0, 19, 3)).toBe(true);
  });

  it('returns true when piece is below bottom border', () => {
    expect(checkCollision(board, 'T', 0, 20, 3)).toBe(true);
  });

  it('returns true when piece touches left wall', () => {
    expect(checkCollision(board, 'T', 0, 0, -1)).toBe(true);
  });

  it('returns true when piece touches right wall', () => {
    expect(checkCollision(board, 'T', 0, 0, 8)).toBe(true);
  });

  it('returns true when piece overlaps a placed block', () => {
    board[1][4] = 'S';
    expect(checkCollision(board, 'T', 0, 0, 3)).toBe(true);
  });

  it('returns true when piece is partially above the board (cell row < 0)', () => {
    expect(checkCollision(board, 'T', 0, -1, 3)).toBe(true);
  });

  it('returns true when piece is fully above the board (all cells < 0)', () => {
    expect(checkCollision(board, 'T', 0, -2, 3)).toBe(true);
  });

  it('detects collision with a block on the left side', () => {
    board[1][0] = 'I';
    expect(checkCollision(board, 'T', 0, 0, 0)).toBe(true);
  });

  it('detects collision with a block far right', () => {
    board[1][6] = 'I';
    expect(checkCollision(board, 'T', 0, 0, 6)).toBe(true);
  });
});

describe('placePiece', () => {
  it('places piece cells on the board', () => {
    const board = createBoard(20, 10);
    const next = placePiece(board, 'T', 0, 0, 3);

    expect(next[0][4]).toBe('T');
    expect(next[1][3]).toBe('T');
    expect(next[1][4]).toBe('T');
    expect(next[1][5]).toBe('T');
  });

  it('does not mutate original board', () => {
    const board = createBoard(20, 10);
    placePiece(board, 'T', 0, 0, 3);

    for (const row of board) {
      for (const cell of row) {
        expect(cell).toBeNull();
      }
    }
  });

  it('places I piece horizontally', () => {
    const board = createBoard(20, 10);
    const next = placePiece(board, 'I', 0, 0, 0);

    expect(next[1][0]).toBe('I');
    expect(next[1][1]).toBe('I');
    expect(next[1][2]).toBe('I');
    expect(next[1][3]).toBe('I');
  });

  it('places I piece vertically', () => {
    const board = createBoard(20, 10);
    const next = placePiece(board, 'I', 1, 0, 0);

    expect(next[0][2]).toBe('I');
    expect(next[1][2]).toBe('I');
    expect(next[2][2]).toBe('I');
    expect(next[3][2]).toBe('I');
  });
});

describe('clearLines', () => {
  it('returns same board and 0 if no lines full', () => {
    const board = createBoard(20, 10);
    const { board: result, linesCleared } = clearLines(board);

    expect(linesCleared).toBe(0);
    expect(result).toHaveLength(20);
    for (const row of result) {
      for (const cell of row) {
        expect(cell).toBeNull();
      }
    }
  });

  it('clears a single full line and shifts rows down', () => {
    const board = createBoard(4, 3);
    board[3] = ['T', 'S', 'Z'];

    const { board: result, linesCleared } = clearLines(board);

    expect(linesCleared).toBe(1);
    expect(result).toHaveLength(4);
    expect(result[0]).toEqual([null, null, null]);
    expect(result[1]).toEqual([null, null, null]);
    expect(result[2]).toEqual([null, null, null]);
  });

  it('clears multiple full lines', () => {
    const board = createBoard(4, 3);
    board[2] = ['T', 'S', 'Z'];
    board[3] = ['I', 'O', 'L'];

    const { board: result, linesCleared } = clearLines(board);

    expect(linesCleared).toBe(2);
    for (const row of result) {
      for (const cell of row) {
        expect(cell).toBeNull();
      }
    }
  });

  it('preserves partial lines above cleared lines', () => {
    const board = createBoard(4, 3);
    board[1] = ['T', null, 'Z'];
    board[3] = ['I', 'O', 'L'];

    const { board: result, linesCleared } = clearLines(board);

    expect(linesCleared).toBe(1);
    expect(result[0]).toEqual([null, null, null]);
    expect(result[1]).toEqual([null, null, null]);
    expect(result[2]).toEqual(['T', null, 'Z']);
    expect(result[3]).toEqual([null, null, null]);
  });

  it('returns empty board when all lines full', () => {
    const board = [
      ['I', 'I', 'I'],
      ['O', 'O', 'O'],
      ['T', 'T', 'T'],
      ['S', 'S', 'S'],
    ];

    const { board: result, linesCleared } = clearLines(board);

    expect(linesCleared).toBe(4);
    expect(result).toHaveLength(4);
    for (const row of result) {
      for (const cell of row) {
        expect(cell).toBeNull();
      }
    }
  });
});

describe('getGhostRow — spacebar / hard drop', () => {
  let board;

  beforeEach(() => {
    board = createBoard(20, 10);
  });

  it('drops T piece to bottom row 18 on empty board', () => {
    const ghostRow = getGhostRow(board, 'T', 0, 3, 0);
    expect(ghostRow).toBe(18);
  });

  it('drops I piece to bottom row 18 on empty board (1 cell offset from bottom)', () => {
    const ghostRow = getGhostRow(board, 'I', 0, 0, 0);
    expect(ghostRow).toBe(18);
  });

  it('drops I piece vertical to bottom row 16 (4 rows tall)', () => {
    const ghostRow = getGhostRow(board, 'I', 1, 0, 0);
    expect(ghostRow).toBe(16);
  });

  it('stops above a placed block', () => {
    board[15] = ['T', 'T', 'T', null, null, null, null, null, null, null];
    const ghostRow = getGhostRow(board, 'T', 0, 0, 0);
    expect(ghostRow).toBe(13);
  });

  it('stops just above scattered blocks', () => {
    board[10][4] = 'S';
    const ghostRow = getGhostRow(board, 'T', 0, 3, 0);
    expect(ghostRow).toBe(8);
  });

  it('returns startRow if already at bottom', () => {
    board[17] = Array(10).fill('T');
    const ghostRow = getGhostRow(board, 'T', 0, 3, 18);
    expect(ghostRow).toBe(18);
  });

  it('falls all the way from startRow 10 past empty rows', () => {
    const ghostRow = getGhostRow(board, 'T', 0, 3, 10);
    expect(ghostRow).toBe(18);
  });

  it('works with rotated piece (T rotation 1 — point down)', () => {
    const ghostRow = getGhostRow(board, 'T', 1, 3, 0);
    expect(ghostRow).toBe(17);
  });

  it('works with O piece (4x4 bounding box, 2x2 block)', () => {
    const ghostRow = getGhostRow(board, 'O', 0, 4, 0);
    expect(ghostRow).toBe(17);
  });

  it('hard drop (spacebar) places piece at ghost row exactly', () => {
    const piece = { name: 'T', rotation: 0, col: 3, row: 0 };
    const ghostRow = getGhostRow(board, piece.name, piece.rotation, piece.col, piece.row);
    const result = placePiece(board, piece.name, piece.rotation, ghostRow, piece.col);

    expect(result[18][4]).toBe('T');
    expect(result[19][3]).toBe('T');
    expect(result[19][4]).toBe('T');
    expect(result[19][5]).toBe('T');
  });

  it('hard drop onto existing blocks lands correctly', () => {
    board[10][5] = 'S';
    board[10][6] = 'S';

    const ghostRow = getGhostRow(board, 'T', 0, 3, 0);
    const result = placePiece(board, 'T', 0, ghostRow, 3);

    expect(ghostRow).toBe(8);
    expect(result[8][4]).toBe('T');
    expect(result[9][3]).toBe('T');
    expect(result[9][4]).toBe('T');
  });

  it('hard drop with rotated I piece works', () => {
    const ghostRow = getGhostRow(board, 'I', 1, 4, 0);
    const result = placePiece(board, 'I', 1, ghostRow, 4);

    expect(result[16][6]).toBe('I');
    expect(result[17][6]).toBe('I');
    expect(result[18][6]).toBe('I');
    expect(result[19][6]).toBe('I');
  });

  it('ghost row is same regardless of startRow', () => {
    const ghost1 = getGhostRow(board, 'T', 0, 3, 0);
    const ghost2 = getGhostRow(board, 'T', 0, 3, 5);
    const ghost3 = getGhostRow(board, 'T', 0, 3, 10);

    expect(ghost1).toBe(18);
    expect(ghost2).toBe(18);
    expect(ghost3).toBe(18);
  });
});

describe('getValidCols — random spawn positions', () => {
  let board;

  beforeEach(() => {
    board = createBoard(20, 10);
  });

  it('returns 7 cols for I piece (size 4, cols 0–6 on 10-wide board)', () => {
    expect(getValidCols(board, 'I', 0, 0)).toEqual([0, 1, 2, 3, 4, 5, 6]);
  });

  it('returns 8 cols for T piece (size 3, cols 0–7 on 10-wide board)', () => {
    expect(getValidCols(board, 'T', 0, 0)).toEqual([0, 1, 2, 3, 4, 5, 6, 7]);
  });

  it('returns 8 cols for O piece (size 4 but cells only at col offsets 1-2)', () => {
    expect(getValidCols(board, 'O', 0, 0)).toEqual([0, 1, 2, 3, 4, 5, 6, 7]);
  });

  it('excludes columns blocked by existing blocks at spawn row', () => {
    board[0][3] = 'T';
    board[0][4] = 'T';
    const valid = getValidCols(board, 'T', 0, 0);
    expect(valid.includes(2)).toBe(false);
    expect(valid.includes(3)).toBe(false);
    expect(valid).toEqual([0, 1, 4, 5, 6, 7]);
  });

  it('returns empty array when all columns blocked', () => {
    for (let c = 0; c < 10; c++) {
      board[0][c] = 'X';
    }
    expect(getValidCols(board, 'T', 0, 0)).toEqual([]);
  });

  it('works with rotated I piece (col offset 2, max 7)', () => {
    const valid = getValidCols(board, 'I', 1, 0);
    expect(valid).toEqual([0, 1, 2, 3, 4, 5, 6, 7]);
  });
});
