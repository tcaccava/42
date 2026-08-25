import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act } from '@testing-library/react';
import { useRoute, navigate } from '../router';

describe('navigate', () => {
  beforeEach(() => {
    window.history.pushState(null, '', '/');
  });

  it('pushes a new history entry and dispatches popstate', () => {
    const spy = vi.fn();
    window.addEventListener('popstate', spy);

    navigate('/room/player');

    expect(window.location.pathname).toBe('/room/player');
    expect(spy).toHaveBeenCalled();
    window.removeEventListener('popstate', spy);
  });
});

describe('useRoute', () => {
  beforeEach(() => {
    window.history.pushState(null, '', '/');
  });

  it('returns landing for root path', () => {
    const { result } = renderHook(() => useRoute());
    expect(result.current).toEqual({ route: 'landing' });
  });

  it('returns game route for /room/player path', () => {
    navigate('/myroom/alice');
    const { result } = renderHook(() => useRoute());
    expect(result.current).toEqual({
      route: 'game',
      roomName: 'myroom',
      playerName: 'alice',
    });
  });

  it('updates on popstate', () => {
    const { result } = renderHook(() => useRoute());
    expect(result.current).toEqual({ route: 'landing' });

    act(() => {
      navigate('/game/bob');
    });

    expect(result.current).toEqual({
      route: 'game',
      roomName: 'game',
      playerName: 'bob',
    });
  });

  it('handles multi-segment player name', () => {
    navigate('/room/player/42');
    const { result } = renderHook(() => useRoute());
    expect(result.current).toEqual({
      route: 'game',
      roomName: 'room',
      playerName: 'player/42',
    });
  });
});
