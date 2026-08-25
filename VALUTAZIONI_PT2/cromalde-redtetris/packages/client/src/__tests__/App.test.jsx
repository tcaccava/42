import { describe, it, expect, vi } from 'vitest';
import { render, screen } from '@testing-library/react';
import App from '../App';

vi.mock('../router', () => ({
  useRoute: vi.fn(() => ({ route: 'landing' })),
  navigate: vi.fn(),
}));

describe('App', () => {
  it('renders header with title', () => {
    render(<App />);
    expect(screen.getByText(/RED_TETRIS/)).toBeInTheDocument();
  });

  it('renders landing page content', () => {
    render(<App />);
    expect(screen.getByText(/CONNECT_TO_GRID/)).toBeInTheDocument();
  });

  it('renders footer with system status link', () => {
    render(<App />);
    const links = screen.getAllByRole('link');
    expect(links.length).toBeGreaterThanOrEqual(3);
  });
});
