import {
  PIECES,
  getCells,
  createBoard,
  checkCollision,
  placePiece,
  clearLines,
  addPenaltyLines,
  getValidCols,
} from '@red-tetris/shared';
import Piece from './Piece.js';

const PIECE_NAMES = /** @type {import('@red-tetris/shared').PieceName[]} */ (
  Object.keys(PIECES)
);

class Game {
  constructor(roomId) {
    this.roomId = roomId;
    /** @type {import('./Player.js').default[]} */
    this.players = [];
    /** @type {Map<string, { board: any[][], activePiece: InstanceType<typeof Piece>|null, score: number, linesCleared: number, status: string, queueIndex: number }>} */
    this.playerStates = new Map();
    /** @type {Array<{name: string, rotation: number}>} */
    this.nextQueue = [];
    this.status = 'waiting'; // waiting | playing | over
    this.winnerId = null;
  }

  /* ---- Player management ---- */

  addPlayer(player) {
    this.players.push(player);
  }

  removePlayer(playerId) {
    this.playerStates.delete(playerId);
    const idx = this.players.findIndex((p) => p.id === playerId);
    if (idx === -1) return;
    const removed = this.players[idx];
    this.players.splice(idx, 1);
    if (removed.isHost && this.players.length > 0) {
      this.players[0].setHost(true);
      this.players[0].ready = false;
    }
    if (this.players.length === 0) {
      this.status = 'over';
    }
  }

  reconnectPlayer(playerId, newSocketId) {
    const player = this.players.find((p) => p.id === playerId);
    if (!player) return null;
    player.reconnect(newSocketId);
    let ps = this.playerStates.get(playerId);
    if (!ps && this.status === 'playing') {
      const maxIdx = Math.max(
        ...Array.from(this.playerStates.values()).map((s) => s.queueIndex),
        0,
      );
      ps = {
        board: createBoard(20, 10),
        activePiece: null,
        score: 0,
        linesCleared: 0,
        status: 'playing',
        queueIndex: maxIdx,
        lockTimer: 0,
      };
      this.playerStates.set(playerId, ps);
      this.#spawnFromQueue(playerId);
    }
    return ps ?? null;
  }

  /* ---- Game lifecycle ---- */

  start() {
    this.status = 'playing';
    this.winnerId = null;
    this.nextQueue = [];
    this.playerStates.clear();
    for (const p of this.players) {
      if (!p.connected) continue;
      this.playerStates.set(p.id, {
        board: createBoard(20, 10),
        activePiece: null,
        score: 0,
        linesCleared: 0,
        status: 'playing',
        queueIndex: 0,
        lockTimer: 0,
      });
    }
    this.#fillBag();
    for (const [pid] of this.playerStates) {
      this.#spawnFromQueue(pid);
    }
  }

  tick() {
    if (this.status !== 'playing') return;

    for (const [pid, ps] of this.playerStates) {
      if (ps.status !== 'playing' || !ps.activePiece) continue;

      const candidate = ps.activePiece.clone();
      candidate.row += 1;

      if (!checkCollision(ps.board, candidate.name, candidate.rotation, candidate.row, candidate.col)) {
        ps.activePiece.row += 1;
        ps.lockTimer = 0;
      } else if (ps.lockTimer === 0) {
        ps.lockTimer = 1;
      } else {
        this.#lock(pid);
      }
    }

    // Check if game should end
    const alive = Array.from(this.playerStates.entries()).filter(
      ([pid, ps]) => {
        if (ps.status !== 'playing') return false;
        const p = this.players.find((p) => p.id === pid);
        return p && p.connected;
      },
    );
    if (alive.length <= 1) {
      this.status = 'over';
      this.winnerId = alive.length === 1 ? alive[0][0] : null;
    }
  }

  handleInput(playerId, action) {
    const ps = this.playerStates.get(playerId);
    if (!ps || ps.status !== 'playing' || !ps.activePiece) return;

    const piece = ps.activePiece;

    switch (action) {
      case 'moveLeft': {
        if (!checkCollision(ps.board, piece.name, piece.rotation, piece.row, piece.col - 1)) {
          piece.col -= 1;
          ps.lockTimer = 0;
        }
        break;
      }
      case 'moveRight': {
        if (!checkCollision(ps.board, piece.name, piece.rotation, piece.row, piece.col + 1)) {
          piece.col += 1;
          ps.lockTimer = 0;
        }
        break;
      }
      case 'rotateCW': {
        const candidate = piece.clone();
        candidate.rotateCW();
        if (!checkCollision(ps.board, candidate.name, candidate.rotation, candidate.row, candidate.col)) {
          piece.rotateCW();
          ps.lockTimer = 0;
        }
        break;
      }
      case 'softDrop': {
        if (!checkCollision(ps.board, piece.name, piece.rotation, piece.row + 1, piece.col)) {
          piece.row += 1;
          ps.lockTimer = 0;
        }
        break;
      }
      case 'hardDrop': {
        let ghostRow = piece.row;
        while (!checkCollision(ps.board, piece.name, piece.rotation, ghostRow + 1, piece.col)) {
          ghostRow++;
        }
        piece.row = ghostRow;
        this.#lock(playerId);
        break;
      }
    }
  }

  /* ---- Private helpers ---- */

  #fillBag() {
    const bag = [...PIECE_NAMES];
    for (let i = bag.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [bag[i], bag[j]] = [bag[j], bag[i]];
    }
    this.nextQueue.push(
      ...bag.map((name) => ({ name, rotation: Math.floor(Math.random() * 4) })),
    );
  }

  #spawnFromQueue(playerId) {
    const ps = this.playerStates.get(playerId);
    if (!ps) return false;

    while (ps.queueIndex >= this.nextQueue.length) {
      this.#fillBag();
    }

    const next = this.nextQueue[ps.queueIndex];
    if (!next) return false;
    ps.queueIndex++;

    const validCols = getValidCols(ps.board, next.name, next.rotation, 0);
    if (validCols.length === 0) {
      ps.activePiece = null;
      ps.status = 'over';
      return false;
    }

    const col = validCols[Math.floor(Math.random() * validCols.length)];
    ps.activePiece = new Piece(next.name, next.rotation, 0, col);
    return true;
  }

  #lock(playerId) {
    const ps = this.playerStates.get(playerId);
    if (!ps || !ps.activePiece) return;

    const { name, rotation, row, col } = ps.activePiece;
    const placed = placePiece(ps.board, name, rotation, row, col);
    const { board: cleared, linesCleared } = clearLines(placed);

    ps.board = cleared;
    if (linesCleared > 0) {
      ps.score += linesCleared * 100;
      ps.linesCleared += linesCleared;
    }

    // Penalty: n-1 indestructible lines to all other players
    const penaltyCount = linesCleared > 1 ? linesCleared - 1 : 0;
    if (penaltyCount > 0) {
      for (const [otherId, otherPs] of this.playerStates) {
        if (otherId === playerId || otherPs.status !== 'playing') continue;
        const result = addPenaltyLines(otherPs.board, penaltyCount);
        otherPs.board = result.board;
        if (result.gameOver) {
          otherPs.activePiece = null;
          otherPs.status = 'over';
        }
      }
    }

    this.#spawnFromQueue(playerId);
  }

  /* ---- Serialization ---- */

  toJSON() {
    const playerStates = {};
    for (const [id, ps] of this.playerStates) {
      playerStates[id] = {
        board: ps.board,
        activePiece: ps.activePiece
          ? {
              name: ps.activePiece.name,
              rotation: ps.activePiece.rotation,
              row: ps.activePiece.row,
              col: ps.activePiece.col,
            }
          : null,
        score: ps.score,
        linesCleared: ps.linesCleared,
        status: ps.status,
        queueIndex: ps.queueIndex,
      };
    }

    return {
      roomId: this.roomId,
      status: this.status,
      winnerId: this.winnerId,
      players: this.players.map((p) => ({
        id: p.id,
        name: p.name,
        isHost: p.isHost,
        ready: p.ready,
        connected: p.connected,
      })),
      nextQueue: this.nextQueue,
      playerStates,
    };
  }
}

export default Game;
