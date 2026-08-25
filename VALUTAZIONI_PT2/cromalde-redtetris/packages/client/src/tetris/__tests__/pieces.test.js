import { describe, it, expect } from 'vitest';
import { PIECES, getCells, nextRotation } from '@red-tetris/shared';

describe('PIECES', () => {
  it('defines all 7 standard tetriminos', () => {
    const names = Object.keys(PIECES);
    expect(names.sort()).toEqual(['I', 'J', 'L', 'O', 'S', 'T', 'Z']);
  });

  describe('each piece', () => {
    for (const [name, piece] of Object.entries(PIECES)) {
      it(`${name} has valid properties`, () => {
        expect(['I', 'O', 'T', 'S', 'Z', 'J', 'L']).toContain(piece.name);
        expect([3, 4]).toContain(piece.size);
        expect(piece.cells).toHaveLength(4);
      });

      it(`${name} has exactly 4 cells in each rotation`, () => {
        for (let r = 0; r < 4; r++) {
          expect(piece.cells[r]).toHaveLength(4);
        }
      });

      it(`${name} cells are within bounding box`, () => {
        for (let r = 0; r < 4; r++) {
          for (const [row, col] of piece.cells[r]) {
            expect(row).toBeGreaterThanOrEqual(0);
            expect(row).toBeLessThan(piece.size);
            expect(col).toBeGreaterThanOrEqual(0);
            expect(col).toBeLessThan(piece.size);
          }
        }
      });
    }
  });

  describe('O piece', () => {
    it('has identical cells in all 4 rotations', () => {
      for (let r = 1; r < 4; r++) {
        expect(PIECES.O.cells[r]).toEqual(PIECES.O.cells[0]);
      }
    });
  });
});

describe('getCells', () => {
  it('returns correct cells for I piece rotation 0', () => {
    const cells = getCells('I', 0);
    expect(cells).toEqual([[1, 0], [1, 1], [1, 2], [1, 3]]);
  });

  it('returns correct cells for I piece rotation 1', () => {
    const cells = getCells('I', 1);
    expect(cells).toEqual([[0, 2], [1, 2], [2, 2], [3, 2]]);
  });

  it('wraps rotation index with & 3', () => {
    expect(getCells('I', 4)).toEqual(getCells('I', 0));
    expect(getCells('I', 5)).toEqual(getCells('I', 1));
    expect(getCells('I', -1)).toEqual(getCells('I', 3));
  });

  it('returns empty array for unknown piece name', () => {
    expect(getCells('X', 0)).toEqual([]);
  });
});

describe('nextRotation', () => {
  it('rotates CW: 0 -> 1 -> 2 -> 3 -> 0', () => {
    expect(nextRotation(0, 1)).toBe(1);
    expect(nextRotation(1, 1)).toBe(2);
    expect(nextRotation(2, 1)).toBe(3);
    expect(nextRotation(3, 1)).toBe(0);
  });

  it('rotates CCW: 0 -> 3 -> 2 -> 1 -> 0', () => {
    expect(nextRotation(0, -1)).toBe(3);
    expect(nextRotation(3, -1)).toBe(2);
    expect(nextRotation(2, -1)).toBe(1);
    expect(nextRotation(1, -1)).toBe(0);
  });

  it('handles larger direction values', () => {
    expect(nextRotation(0, 5)).toBe(1);
    expect(nextRotation(0, -5)).toBe(3);
  });
});
