import { describe, it, expect } from 'vitest';
import { createBoard } from '@red-tetris/shared';
import { calcSpectrum } from '../MiniGameBoard';

describe('calcSpectrum', () => {
  it('returns all zero heights for an empty board', () => {
    const board = createBoard(20, 10);
    const { spectrum, maxHeight } = calcSpectrum(board);
    expect(spectrum).toHaveLength(10);
    expect(spectrum.every((h) => h === 0)).toBe(true);
    expect(maxHeight).toBe(0);
  });

  it('returns height 1 for a block on the bottom row (row 19)', () => {
    const board = createBoard(20, 10);
    board[19][5] = 'T';
    const { spectrum, maxHeight } = calcSpectrum(board);
    expect(spectrum[5]).toBe(1);
    expect(maxHeight).toBe(1);
  });

  it('returns height 2 for a block on row 18', () => {
    const board = createBoard(20, 10);
    board[18][3] = 'S';
    const { spectrum } = calcSpectrum(board);
    expect(spectrum[3]).toBe(2);
  });

  it('returns height 10 for a block on row 10', () => {
    const board = createBoard(20, 10);
    board[10][7] = 'Z';
    const { spectrum } = calcSpectrum(board);
    expect(spectrum[7]).toBe(10);
  });

  it('returns height 20 for a block on the top row (row 0)', () => {
    const board = createBoard(20, 10);
    board[0][2] = 'I';
    const { spectrum } = calcSpectrum(board);
    expect(spectrum[2]).toBe(20);
  });

  it('picks the highest block when multiple blocks exist in the same column', () => {
    const board = createBoard(20, 10);
    board[19][4] = 'T';
    board[10][4] = 'S';
    board[5][4] = 'Z';
    const { spectrum } = calcSpectrum(board);
    // Highest block is at row 5 → height = 20 - 5 = 15
    expect(spectrum[4]).toBe(15);
  });

  it('counts PENALTY cells toward column height', () => {
    const board = createBoard(20, 10);
    board[15][0] = 'PENALTY';
    const { spectrum } = calcSpectrum(board);
    expect(spectrum[0]).toBe(5);
  });

  it('handles mixed heights across columns', () => {
    const board = createBoard(20, 10);
    // col 0: block at row 19 → height 1
    board[19][0] = 'T';
    // col 1: block at row 0 → height 20
    board[0][1] = 'I';
    // col 2: block at row 10 → height 10
    board[10][2] = 'S';
    // col 3: empty → height 0

    const { spectrum, maxHeight } = calcSpectrum(board);
    expect(spectrum[0]).toBe(1);
    expect(spectrum[1]).toBe(20);
    expect(spectrum[2]).toBe(10);
    expect(spectrum[3]).toBe(0);
    expect(maxHeight).toBe(20);
  });

  it('ignores columns with no blocks (remain 0)', () => {
    const board = createBoard(20, 10);
    board[19][9] = 'L';
    const { spectrum } = calcSpectrum(board);
    expect(spectrum[0]).toBe(0);
    expect(spectrum[8]).toBe(0);
    expect(spectrum[9]).toBe(1);
  });

  it('works with a non-20-row board', () => {
    const board = createBoard(10, 5);
    board[9][2] = 'T';
    const { spectrum, maxHeight } = calcSpectrum(board);
    expect(spectrum[2]).toBe(1);
    expect(maxHeight).toBe(1);
  });
});
