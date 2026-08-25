import { describe, it, expect } from 'vitest';
import { render } from '@testing-library/react';
import { createBoard } from '@red-tetris/shared';
import MiniGameBoard from '../MiniGameBoard';

describe('MiniGameBoard component', () => {
  it('renders empty spectrum grid with correct dimensions', () => {
    const board = createBoard(20, 10);
    const { container } = render(<MiniGameBoard board={board} />);
    const cells = container.querySelectorAll('[class*="aspect-square"]');
    expect(cells).toHaveLength(200);
  });

  it('applies grayscale when isDead is true', () => {
    const board = createBoard(20, 10);
    const { container } = render(<MiniGameBoard board={board} isDead={true} />);
    const grid = container.firstChild.firstChild;
    expect(grid.className).toContain('grayscale');
  });

  it('renders completion bar segments', () => {
    const board = createBoard(20, 10);
    const { container } = render(<MiniGameBoard board={board} />);
    const segments = container.querySelectorAll('[class*="rounded-sm"]');
    expect(segments).toHaveLength(10);
  });

  it('fills some completion segments when board has blocks at the bottom', () => {
    const board = createBoard(20, 10);
    board[19][5] = 'T';
    const { container } = render(<MiniGameBoard board={board} />);
    // At least one completion segment should have the filled color
    const segments = container.querySelectorAll('.bg-primary-container');
    expect(segments.length).toBeGreaterThan(0);
  });
});
