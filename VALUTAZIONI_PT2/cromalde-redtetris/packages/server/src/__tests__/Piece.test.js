import { describe, it, expect } from 'vitest';
import Piece from '../Piece.js';

describe('Piece', () => {
  it('constructs with name, rotation, row, col', () => {
    const p = new Piece('T', 0, 5, 3);
    expect(p.name).toBe('T');
    expect(p.rotation).toBe(0);
    expect(p.row).toBe(5);
    expect(p.col).toBe(3);
  });

  it('defaults rotation to 0, row to 0, col to 3', () => {
    const p = new Piece('O');
    expect(p.rotation).toBe(0);
    expect(p.row).toBe(0);
    expect(p.col).toBe(3);
  });

  it('getCells returns absolute positions', () => {
    const p = new Piece('T', 0, 0, 3);
    const cells = p.getCells();
    expect(cells).toEqual([
      [0, 4],
      [1, 3],
      [1, 4],
      [1, 5],
    ]);
  });

  it('getCells respects row/col offset', () => {
    const p = new Piece('I', 0, 5, 2);
    const cells = p.getCells();
    expect(cells).toEqual([
      [6, 2],
      [6, 3],
      [6, 4],
      [6, 5],
    ]);
  });

  it('rotateCW cycles rotation 0->1->2->3->0', () => {
    const p = new Piece('T', 0);
    p.rotateCW();
    expect(p.rotation).toBe(1);
    p.rotateCW();
    expect(p.rotation).toBe(2);
    p.rotateCW();
    expect(p.rotation).toBe(3);
    p.rotateCW();
    expect(p.rotation).toBe(0);
  });

  it('clone returns an independent copy', () => {
    const p = new Piece('S', 1, 3, 5);
    const c = p.clone();
    expect(c).not.toBe(p);
    expect(c.name).toBe('S');
    expect(c.rotation).toBe(1);
    expect(c.row).toBe(3);
    expect(c.col).toBe(5);

    c.rotateCW();
    expect(p.rotation).toBe(1);
    expect(c.rotation).toBe(2);
  });
});
