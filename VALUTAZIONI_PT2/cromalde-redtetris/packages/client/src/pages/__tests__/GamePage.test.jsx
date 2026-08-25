import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen } from '@testing-library/react';
import GamePage from '../GamePage';
import socket from '../../socket';

vi.mock('../../socket', () => {
  const s = { on: vi.fn(), off: vi.fn(), emit: vi.fn(), connect: vi.fn(), disconnect: vi.fn() };
  return { default: s, getPlayerId: () => 'test-id' };
});

let storeState = {
  gameStatus: 'idle',
  board: Array.from({ length: 20 }, () => Array(10).fill(null)),
  activePiece: null,
  nextPiece: null,
  score: 0,
  linesCleared: 0,
  holdPiece: null,
  players: [],
  playerStates: {},
  winnerId: null,
  playerId: 'test-id',
  resetGame: vi.fn(),
};

function useGameStore(selector) {
  return selector ? selector(storeState) : storeState;
}
useGameStore.getState = () => storeState;

vi.mock('../../store', () => ({
  useGameStore,
  selectGhostRow: () => null,
  selectDanger: () => 0,
}));

describe('GamePage', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it('shows connecting message before joining', () => {
    render(<GamePage roomName="room1" playerName="alice" />);
    expect(screen.getByText(/CONNECTING TO GRID/i)).toBeInTheDocument();
  });

  it('calls socket.connect on mount', () => {
    render(<GamePage roomName="room1" playerName="alice" />);
    expect(socket.connect).toHaveBeenCalled();
  });
});
