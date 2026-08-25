import { describe, it, expect } from 'vitest';
import { render } from '@testing-library/react';
import { createBoard } from '@red-tetris/shared';
import GameBoard from '../GameBoard';

describe('GameBoard', () => {
  it('renders an empty 10x20 grid', () => {
    const board = createBoard(20, 10);
    const { container } = render(<GameBoard board={board} />);
    const cells = container.querySelectorAll('[class*="aspect-square"]');
    expect(cells).toHaveLength(200);
  });

  it('renders active piece cells with special class', () => {
    const board = createBoard(20, 10);
    const activePiece = { name: 'T', rotation: 0, row: 0, col: 3 };
    const { container } = render(<GameBoard board={board} activePiece={activePiece} />);
    const cells = container.querySelectorAll('.piece-glow-T');
    expect(cells.length).toBeGreaterThan(0);
  });

  it('renders ghost row at the given position', () => {
    const board = createBoard(20, 10);
    const activePiece = { name: 'I', rotation: 0, row: 0, col: 3 };
    const { container } = render(
      <GameBoard board={board} activePiece={activePiece} ghostRow={18} />,
    );
    const ghostCells = container.querySelectorAll('.border-dashed');
    expect(ghostCells.length).toBeGreaterThan(0);
  });

  it('renders penalty cells with penalty class', () => {
    const board = createBoard(20, 10);
    board[19] = Array(10).fill('PENALTY');
    const { container } = render(<GameBoard board={board} />);
    const penaltyCells = container.querySelectorAll('[class*="border-l-2"]');
    expect(penaltyCells.length).toBeGreaterThan(0);
  });

  it('renders placed cells with placed class', () => {
    const board = createBoard(20, 10);
    board[19][5] = 'T';
    const { container } = render(<GameBoard board={board} />);
    const placedCells = container.querySelectorAll('[class*="bg-surface-variant"]');
    expect(placedCells.length).toBe(1);
  });

  it('handles active piece without ghost row', () => {
    const board = createBoard(20, 10);
    const activePiece = { name: 'O', rotation: 0, row: 4, col: 4 };
    const { container } = render(<GameBoard board={board} activePiece={activePiece} />);
    expect(container.querySelectorAll('.piece-glow-O').length).toBeGreaterThan(0);
  });
});
