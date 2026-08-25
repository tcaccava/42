import { describe, it, expect } from 'vitest';
import { render, screen } from '@testing-library/react';
import Footer from '../Footer';

describe('Footer', () => {
  it('renders the component signature', () => {
    render(<Footer />);
    expect(screen.getByText(/MADE_BY_CRSLAR/i)).toBeInTheDocument();
  });

  it('renders three navigation links', () => {
    render(<Footer />);
    const links = screen.getAllByRole('link');
    expect(links).toHaveLength(3);
    expect(links[0]).toHaveTextContent('SYSTEM_STATUS');
    expect(links[1]).toHaveTextContent('RESOURCES');
    expect(links[2]).toHaveTextContent('TERMINAL_ACCESS');
  });
});
