import { describe, it, expect } from 'vitest';
import { render } from '@testing-library/react';
import PiecePreview from '../PiecePreview';

describe('PiecePreview', () => {
  for (const name of ['I', 'O', 'T', 'S', 'Z', 'J', 'L']) {
    it(`renders a preview for piece ${name}`, () => {
      const { container } = render(<PiecePreview name={name} />);
      expect(container.querySelectorAll('[class*="w-6"]').length).toBeGreaterThan(0);
    });
  }

  it('returns null for unknown piece name', () => {
    const { container } = render(<PiecePreview name={'X'} />);
    expect(container.innerHTML).toBe('');
  });

  it('applies sm size class when size is sm', () => {
    const { container } = render(<PiecePreview name="T" size="sm" />);
    expect(container.querySelectorAll('[class*="w-5"]').length).toBeGreaterThan(0);
  });

  it('accepts custom rotation', () => {
    const { container } = render(<PiecePreview name="I" rotation={1} />);
    expect(container.querySelectorAll('[class*="w-6"]').length).toBeGreaterThan(0);
  });
});
