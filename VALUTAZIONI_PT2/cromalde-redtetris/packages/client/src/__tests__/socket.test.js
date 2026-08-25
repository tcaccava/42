import { describe, it, expect, beforeEach, vi } from 'vitest';

const localStorageMock = (() => {
  let store = {};
  return {
    getItem: vi.fn((key) => store[key] ?? null),
    setItem: vi.fn((key, value) => { store[key] = value; }),
    clear: vi.fn(() => { store = {}; }),
  };
})();

vi.mock('socket.io-client', () => ({
  io: vi.fn(() => ({ on: vi.fn(), off: vi.fn(), connect: vi.fn(), disconnect: vi.fn(), emit: vi.fn() })),
}));

// Re-import after mocking
const { getPlayerId } = await import('../socket');

describe('getPlayerId', () => {
  beforeEach(() => {
    localStorageMock.clear();
    Object.defineProperty(window, 'localStorage', { value: localStorageMock });
  });

  it('generates a new ID and persists it', () => {
    const id = getPlayerId();
    expect(id).toBeTruthy();
    expect(typeof id).toBe('string');
    expect(localStorage.setItem).toHaveBeenCalledWith('rt_player_id', id);
  });

  it('returns the same ID on subsequent calls', () => {
    const id1 = getPlayerId();
    const id2 = getPlayerId();
    expect(id1).toBe(id2);
  });

  it('returns existing ID from localStorage', () => {
    localStorageMock.getItem.mockReturnValue('existing-id');
    expect(getPlayerId()).toBe('existing-id');
  });
});
