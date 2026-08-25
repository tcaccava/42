import { describe, it, expect, beforeEach } from 'vitest';
import { useGameStore, selectGhostRow, selectDanger } from '../../store';

function freshState() {
  useGameStore.getState().resetGame();
}

function withPlayerId(id) {
  useGameStore.setState({ playerId: id });
}

describe('updateFromServer', () => {
  beforeEach(freshState);

  it('hydrates board, activePiece, nextPiece, score, linesCleared from playerStates', () => {
    withPlayerId('p1');

    useGameStore.getState().updateFromServer({
      status: 'playing',
      players: [
        { id: 'p1', name: 'Alice', isHost: true, ready: false, connected: true },
      ],
      nextQueue: [{ name: 'O', rotation: 0 }],
      playerStates: {
        p1: {
          board: Array.from({ length: 20 }, () => Array(10).fill(null)),
          activePiece: { name: 'T', rotation: 0, row: 5, col: 3 },
          score: 500,
          linesCleared: 3,
          status: 'playing',
          queueIndex: 0,
        },
      },
    });

    const state = useGameStore.getState();
    expect(state.activePiece).toEqual({ name: 'T', rotation: 0, row: 5, col: 3 });
    expect(state.score).toBe(500);
    expect(state.linesCleared).toBe(3);
    expect(state.gameStatus).toBe('playing');
    expect(state.players).toHaveLength(1);
    expect(state.nextPiece).toEqual({ name: 'O', rotation: 0 });
  });

  it('sets gameStatus to over', () => {
    withPlayerId('p1');
    useGameStore.getState().updateFromServer({
      status: 'over',
      players: [],
      firstNextPiece: null,
      playerStates: {
        p1: {
          board: Array.from({ length: 20 }, () => Array(10).fill(null)),
          activePiece: null,
          score: 100,
          linesCleared: 0,
          status: 'over',
        },
      },
    });
    expect(useGameStore.getState().gameStatus).toBe('over');
  });

  it('does not hydrate when playerId is missing from playerStates', () => {
    useGameStore.getState().updateFromServer({
      status: 'playing',
      players: [],
      firstNextPiece: null,
      playerStates: {},
    });
    const state = useGameStore.getState();
    expect(state.activePiece).toBeNull();
    expect(state.score).toBe(0);
  });

  it('defaults players and playerStates to empty when missing', () => {
    useGameStore.getState().updateFromServer({
      status: 'waiting',
    });
    const state = useGameStore.getState();
    expect(state.players).toEqual([]);
    expect(state.playerStates).toEqual({});
  });
});

describe('resetGame', () => {
  beforeEach(freshState);

  it('resets to initial state', () => {
    withPlayerId('p1');
    useGameStore.getState().updateFromServer({
      status: 'playing',
      players: [{ id: 'p1', name: 'A', isHost: true, ready: false, connected: true }],
      firstNextPiece: { name: 'O', rotation: 0 },
      playerStates: {
        p1: {
          board: Array.from({ length: 20 }, () => Array(10).fill('X')),
          activePiece: { name: 'T', rotation: 0, row: 0, col: 3 },
          score: 999,
          linesCleared: 50,
          status: 'playing',
        },
      },
    });

    useGameStore.getState().resetGame();
    const state = useGameStore.getState();

    expect(state.gameStatus).toBe('idle');
    expect(state.activePiece).toBeNull();
    expect(state.nextPiece).toBeNull();
    expect(state.score).toBe(0);
    expect(state.linesCleared).toBe(0);
    expect(state.players).toEqual([]);
    expect(state.board).toHaveLength(20);
  });
});

describe('selectGhostRow', () => {
  beforeEach(freshState);

  it('returns null when no active piece', () => {
    expect(selectGhostRow(useGameStore.getState())).toBeNull();
  });

  it('returns bottom row for empty board', () => {
    useGameStore.setState({
      activePiece: { name: 'I', rotation: 0, row: 0, col: 3 },
    });
    expect(selectGhostRow(useGameStore.getState())).toBe(18);
  });
});

describe('selectDanger', () => {
  beforeEach(freshState);

  it('returns 0 for empty board', () => {
    expect(selectDanger(useGameStore.getState())).toBe(0);
  });

  it('returns >0 when cells exist in top rows', () => {
    const board = Array.from({ length: 20 }, () => Array(10).fill(null));
    board[2][5] = 'T';
    useGameStore.setState({ board });
    expect(selectDanger(useGameStore.getState())).toBeGreaterThan(0);
  });
});
