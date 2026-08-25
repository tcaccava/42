import { describe, it, expect } from 'vitest';
import { render } from '@testing-library/react';
import Scanlines from '../Scanlines';

describe('Scanlines', () => {
  it('renders scanlines overlay', () => {
    const { container } = render(<Scanlines />);
    const div = container.firstChild;
    expect(div).toBeInTheDocument();
    expect(div.className).toContain('scanlines');
  });
});
