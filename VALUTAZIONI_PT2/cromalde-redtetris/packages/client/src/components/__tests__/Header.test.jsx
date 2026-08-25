import { describe, it, expect } from 'vitest';
import { render, screen } from '@testing-library/react';
import Header from '../Header';

describe('Header', () => {
  it('renders the title', () => {
    render(<Header />);
    expect(screen.getByText(/RED_TETRIS/)).toBeInTheDocument();
  });

  it('renders the UTC timestamp', () => {
    render(<Header />);
    const utcElements = screen.getAllByText(/UTC/);
    expect(utcElements.length).toBeGreaterThanOrEqual(1);
  });
});
