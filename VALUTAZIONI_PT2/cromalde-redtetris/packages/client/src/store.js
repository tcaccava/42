import { create } from 'zustand';
import { createBoard, getGhostRow } from '@red-tetris/shared';

const INITIAL_STATE = {
  board: createBoard(20, 10),
  activePiece: null,
  nextPiece: null,
  score: 0,
  linesCleared: 0,
  holdPiece: null,
  gameStatus: 'idle',
  winnerId: null,
  players: [],
  playerStates: {},
};

const useGameStore = create((set) => ({
  ...INITIAL_STATE,

  updateFromServer(data) {
    const myId = useGameStore.getState().playerId;
    const myState = data.playerStates?.[myId];

    const myQueueIndex = data.playerStates?.[myId]?.queueIndex;
    set({
      gameStatus: data.status,
      winnerId: data.winnerId ?? null,
      players: data.players ?? [],
      playerStates: data.playerStates ?? {},
      nextPiece: data.nextQueue?.[myQueueIndex] ?? null,
      ...(myState
        ? {
            board: myState.board,
            activePiece: myState.activePiece,
            score: myState.score,
            linesCleared: myState.linesCleared,
          }
        : {}),
    });
  },

  setPlayerId(id) {
    set({ playerId: id });
  },

  resetGame() {
    set({ ...INITIAL_STATE });
  },
}));

function selectGhostRow(state) {
  if (!state.activePiece) return null;
  return getGhostRow(
    state.board,
    state.activePiece.name,
    state.activePiece.rotation,
    state.activePiece.col,
    state.activePiece.row,
  );
}

function selectDanger(state) {
  for (let r = 0; r < state.board.length; r++) {
    if (state.board[r].some((cell) => cell !== null)) {
      return Math.round(((state.board.length - r) / state.board.length) * 100);
    }
  }
  return 0;
}

export { useGameStore, selectGhostRow, selectDanger };
