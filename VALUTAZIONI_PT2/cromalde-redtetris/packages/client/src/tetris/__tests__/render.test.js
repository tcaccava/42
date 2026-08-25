import { describe, it, expect } from 'vitest';
import { cellClass } from '../render';

describe('cellClass', () => {
  it('returns empty cell class', () => {
    const cls = cellClass('empty');
    expect(cls).toBe('bg-white/[0.03] border border-white/[0.05]');
  });

  it('returns ghost cell class', () => {
    const cls = cellClass('ghost');
    expect(cls).toBe('bg-transparent border border-dashed border-primary/50');
  });

  it('returns placed cell class', () => {
    const cls = cellClass('placed');
    expect(cls).toBe('bg-surface-variant border border-surface-variant');
  });

  describe('active', () => {
    for (const name of ['I', 'O', 'T', 'S', 'Z', 'J', 'L']) {
      it(`returns a non-empty string for ${name}`, () => {
        const cls = cellClass('active', name);
        expect(cls.length).toBeGreaterThan(0);
      });

      it(`${name} includes its glow class`, () => {
        const cls = cellClass('active', name);
        expect(cls).toContain(`piece-glow-${name}`);
      });

      it(`${name} includes color and border classes`, () => {
        const cls = cellClass('active', name);
        expect(cls).toContain('bg-');
        expect(cls).toContain('border');
      });
    }

    it('returns empty string for unknown piece name', () => {
      expect(cellClass('active', 'X')).toBe('');
    });

    it('returns empty string when pieceName is missing', () => {
      expect(cellClass('active')).toBe('');
    });
  });

  it('returns empty string for unknown type', () => {
    expect(cellClass('invalid')).toBe('');
  });
});
