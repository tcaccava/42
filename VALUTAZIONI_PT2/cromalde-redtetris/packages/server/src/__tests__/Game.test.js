import { describe, it, expect, beforeEach } from 'vitest';
import { addPenaltyLines, createBoard } from '@red-tetris/shared';
import Game from '../Game.js';
import Player from '../Player.js';

describe('Game', () => {
  let game;
  let player1;
  let player2;

  beforeEach(() => {
    game = new Game('room1');
    player1 = new Player('p1', 'socket1', 'Alice', true);
    player2 = new Player('p2', 'socket2', 'Bob');
  });

  function setupTwoPlayers() {
    game.addPlayer(player1);
    game.addPlayer(player2);
  }

  describe('constructor', () => {
    it('creates a game with waiting status', () => {
      expect(game.roomId).toBe('room1');
      expect(game.status).toBe('waiting');
      expect(game.players).toEqual([]);
      expect(game.playerStates.size).toBe(0);
      expect(game.winnerId).toBeNull();
    });
  });

  describe('addPlayer / removePlayer', () => {
    it('adds a player to the game', () => {
      game.addPlayer(player1);
      expect(game.players).toHaveLength(1);
      expect(game.players[0].id).toBe('p1');
    });

    it('allows adding player even when game is playing', () => {
      game.addPlayer(player1);
      game.start();
      game.addPlayer(player2);
      expect(game.players).toHaveLength(2);
      expect(game.players[1].id).toBe('p2');
    });

    it('allows adding player when status is over', () => {
      game.addPlayer(player1);
      game.start();
      // Force game over
      const ps = game.playerStates.get('p1');
      ps.activePiece = null;
      ps.status = 'over';
      game.tick();
      expect(game.status).toBe('over');
      // Now add a new player
      game.addPlayer(player2);
      expect(game.players).toHaveLength(2);
      expect(game.players[1].id).toBe('p2');
    });

    it('removes a player and their state', () => {
      setupTwoPlayers();
      game.start();
      game.removePlayer('p1');
      expect(game.players).toHaveLength(1);
      expect(game.players[0].id).toBe('p2');
      expect(game.playerStates.has('p1')).toBe(false);
    });

    it('promotes next player to host when host leaves', () => {
      game.addPlayer(player1);
      game.addPlayer(player2);
      game.removePlayer('p1');
      expect(game.players[0].isHost).toBe(true);
    });

    it('sets status to over when last player removed', () => {
      game.addPlayer(player1);
      game.removePlayer('p1');
      expect(game.status).toBe('over');
    });
  });

  describe('start', () => {
    it('creates player states for connected players', () => {
      setupTwoPlayers();
      game.start();

      expect(game.status).toBe('playing');
      expect(game.playerStates.size).toBe(2);
      expect(game.playerStates.get('p1').status).toBe('playing');
      expect(game.playerStates.get('p2').status).toBe('playing');
    });

    it('skips disconnected players', () => {
      game.addPlayer(player1);
      player2.disconnect();
      game.addPlayer(player2);
      game.start();

      expect(game.playerStates.size).toBe(1);
      expect(game.playerStates.has('p1')).toBe(true);
      expect(game.playerStates.has('p2')).toBe(false);
    });

    it('spawns active piece for each player and fills queue', () => {
      setupTwoPlayers();
      game.start();

      expect(game.playerStates.get('p1').activePiece).not.toBeNull();
      expect(game.playerStates.get('p2').activePiece).not.toBeNull();
      // One bag of 7 pieces added, no shift — both players read from index 0
      expect(game.nextQueue.length).toBeGreaterThanOrEqual(7);
    });

    it('resets boards to empty', () => {
      setupTwoPlayers();
      game.start();

      for (const [_, ps] of game.playerStates) {
        for (const row of ps.board) {
          for (const cell of row) {
            expect(cell).toBeNull();
          }
        }
      }
    });

    it('resets winnerId', () => {
      setupTwoPlayers();
      game.winnerId = 'p1';
      game.start();
      expect(game.winnerId).toBeNull();
    });
  });

  describe('tick', () => {
    beforeEach(() => {
      setupTwoPlayers();
      game.start();
    });

    it('moves active piece down for all players', () => {
      const rows1 = game.playerStates.get('p1').activePiece.row;
      const rows2 = game.playerStates.get('p2').activePiece.row;
      game.tick();
      expect(game.playerStates.get('p1').activePiece.row).toBe(rows1 + 1);
      expect(game.playerStates.get('p2').activePiece.row).toBe(rows2 + 1);
    });

    it('locks piece at bottom and spawns next', () => {
      const ps1 = game.playerStates.get('p1');
      // Tick until piece reaches bottom (row stops increasing)
      while (game.status === 'playing' && ps1.activePiece) {
        const prev = ps1.activePiece.row;
        game.tick();
        if (!ps1.activePiece) break;
        if (ps1.activePiece.row === prev) break;
      }
      // Lock delay: first collision sets lockTimer=1, second tick locks
      game.tick();

      const hasPlaced = ps1.board.some((r) => r.some((c) => c !== null));
      expect(hasPlaced).toBe(true);
    });

    it('does not lock on first collision tick — sets lockTimer=1', () => {
      const ps1 = game.playerStates.get('p1');
      while (game.status === 'playing' && ps1.activePiece) {
        const prev = ps1.activePiece.row;
        game.tick();
        if (!ps1.activePiece) break;
        if (ps1.activePiece.row === prev) break;
      }
      expect(ps1.lockTimer).toBe(1);
      expect(ps1.activePiece).not.toBeNull();
    });

    it('resets lockTimer on successful movement', () => {
      const ps1 = game.playerStates.get('p1');
      while (game.status === 'playing' && ps1.activePiece) {
        const prev = ps1.activePiece.row;
        game.tick();
        if (!ps1.activePiece) break;
        if (ps1.activePiece.row === prev) break;
      }
      expect(ps1.lockTimer).toBe(1);

      game.handleInput('p1', 'moveLeft');
      expect(ps1.lockTimer).toBe(0);
    });

    it('skips dead players', () => {
      const ps1 = game.playerStates.get('p1');
      ps1.status = 'over';
      const ps2 = game.playerStates.get('p2');
      const rowBefore = ps2.activePiece.row;
      game.tick();
      expect(ps2.activePiece.row).toBe(rowBefore + 1);
    });

    it('is no-op when status is not playing', () => {
      game.status = 'over';
      game.tick();
      // no crash, pieces unmoved
    });

    it('ends game with winnerId when one player dies and one remains alive', () => {
      const ps1 = game.playerStates.get('p1');
      ps1.status = 'over';
      ps1.activePiece = null;
      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBe('p2');
    });

    it('ignores disconnected players for winner calculation', () => {
      const ps1 = game.playerStates.get('p1');
      const p2 = game.players.find((p) => p.id === 'p2');
      p2.disconnect();
      ps1.status = 'over';
      ps1.activePiece = null;
      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBeNull();
    });

    it('ends game when one player disconnects and other is alive', () => {
      const p2 = game.players.find((p) => p.id === 'p2');
      p2.disconnect();
      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBe('p1');
    });

    it('ends game with null winnerId when all players die', () => {
      for (const pid of ['p1', 'p2']) {
        const ps = game.playerStates.get(pid);
        ps.status = 'over';
        ps.activePiece = null;
      }
      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBeNull();
    });
  });

  describe('handleInput', () => {
    let pid;

    beforeEach(() => {
      setupTwoPlayers();
      game.start();
      pid = 'p1';
    });

    it('moveLeft moves piece left when possible', () => {
      const ps = game.playerStates.get(pid);
      ps.activePiece.col = 5;
      ps.activePiece.rotation = 0;
      game.handleInput(pid, 'moveLeft');
      expect(ps.activePiece.col).toBe(4);
    });

    it('moveLeft is blocked by left wall', () => {
      const ps = game.playerStates.get(pid);
      ps.activePiece.name = 'I';
      ps.activePiece.rotation = 0;
      ps.activePiece.col = 0;
      game.handleInput(pid, 'moveLeft');
      expect(ps.activePiece.col).toBe(0);
    });

    it('moveRight moves piece right when possible', () => {
      const ps = game.playerStates.get(pid);
      ps.activePiece.col = 3;
      ps.activePiece.rotation = 0;
      game.handleInput(pid, 'moveRight');
      expect(ps.activePiece.col).toBe(4);
    });

    it('rotateCW rotates the piece', () => {
      const ps = game.playerStates.get(pid);
      ps.activePiece.name = 'T';
      ps.activePiece.rotation = 0;
      game.handleInput(pid, 'rotateCW');
      expect(ps.activePiece.rotation).toBe(1);
    });

    it('softDrop moves piece down', () => {
      const ps = game.playerStates.get(pid);
      ps.activePiece.row = 0;
      game.handleInput(pid, 'softDrop');
      expect(ps.activePiece.row).toBe(1);
    });

    it('hardDrop locks piece at bottom', () => {
      game.handleInput(pid, 'hardDrop');
      const ps = game.playerStates.get(pid);
      expect(ps.activePiece).not.toBeNull();
      const hasPlaced = ps.board.some((r) => r.some((c) => c !== null));
      expect(hasPlaced).toBe(true);
    });

    it('is no-op for dead player', () => {
      const ps = game.playerStates.get(pid);
      ps.status = 'over';
      game.handleInput(pid, 'moveLeft');
      // no crash
    });

    it('affects only the targeted player', () => {
      const ps1 = game.playerStates.get('p1');
      const ps2 = game.playerStates.get('p2');
      const colBefore = ps2.activePiece.col;
      ps1.activePiece.col = 5;
      game.handleInput('p1', 'moveLeft');
      expect(ps1.activePiece.col).toBe(4);
      expect(ps2.activePiece.col).toBe(colBefore);
    });
  });

  describe('penalty lines', () => {
    it('adds n-1 penalty rows when player clears n >= 2 lines', () => {
      setupTwoPlayers();
      game.start();
      const ps1 = game.playerStates.get('p1');
      const ps2 = game.playerStates.get('p2');

      // Fill bottom 2 rows of p1's board to trigger 2-line clear on lock
      ps1.board[18] = Array(10).fill('T');
      ps1.board[19] = Array(10).fill('S');

      // Place active piece to trigger lock + clear
      ps1.activePiece.row = 17;
      game.handleInput('p1', 'hardDrop');

      // p1 cleared 2 lines → 1 penalty row applied to p2
      const penaltyCount = ps2.board.filter(
        (r) => r.every((c) => c === 'PENALTY'),
      ).length;
      expect(penaltyCount).toBe(1);
    });

    it('adds no penalty for clearing 1 line', () => {
      setupTwoPlayers();
      game.start();
      const ps1 = game.playerStates.get('p1');
      const ps2 = game.playerStates.get('p2');

      // Fill only bottom row — 1-line clear → no penalty
      ps1.board[19] = Array(10).fill('T');
      ps1.activePiece.row = 18;
      game.handleInput('p1', 'hardDrop');

      const penaltyCount = ps2.board.filter(
        (r) => r.every((c) => c === 'PENALTY'),
      ).length;
      expect(penaltyCount).toBe(0);
    });

    it('kills opponent when penalty reaches spawn zone', () => {
      setupTwoPlayers();
      game.start();
      const ps2 = game.playerStates.get('p2');

      // Fill ps2's board with penalty rows except top
      for (let r = 4; r < 20; r++) {
        ps2.board[r] = Array(10).fill('PENALTY');
      }

      const ps1 = game.playerStates.get('p1');
      ps1.board[18] = Array(10).fill('T');
      ps1.board[19] = Array(10).fill('S');
      ps1.activePiece.row = 17;
      game.handleInput('p1', 'hardDrop');

      expect(ps2.status).toBe('over');
    });

    it('does not add penalty to the clearing player', () => {
      setupTwoPlayers();
      game.start();
      const ps1 = game.playerStates.get('p1');

      ps1.board[18] = Array(10).fill('T');
      ps1.board[19] = Array(10).fill('S');
      ps1.activePiece.row = 17;
      game.handleInput('p1', 'hardDrop');

      const selfPenalty = ps1.board.filter(
        (r) => r.every((c) => c === 'PENALTY'),
      ).length;
      expect(selfPenalty).toBe(0);
    });
  });

  describe('reconnectPlayer', () => {
    it('finds player and updates socket', () => {
      setupTwoPlayers();
      game.start();

      const state = game.reconnectPlayer('p1', 'newSocket');
      expect(state).not.toBeNull();
      expect(state.score).toBe(0);
      expect(game.players.find((p) => p.id === 'p1').socketId).toBe('newSocket');
    });

    it('returns null for unknown player', () => {
      expect(game.reconnectPlayer('nobody', 's')).toBeNull();
    });

    it('creates fresh state when reconnecting to an active game after disconnecting before start', () => {
      game.addPlayer(player1);
      game.addPlayer(player2);
      player2.disconnect();
      game.start();

      // player2 was disconnected, so they have no playerState
      expect(game.playerStates.has('p2')).toBe(false);

      const state = game.reconnectPlayer('p2', 'newSocket');
      expect(state).not.toBeNull();
      expect(state.status).toBe('playing');
      expect(state.board).toHaveLength(20);
      expect(state.activePiece).not.toBeNull();
    });
  });

  describe('toJSON', () => {
    it('returns per-player states without socketIds', () => {
      setupTwoPlayers();
      game.start();

      const state = game.toJSON();
      expect(state.roomId).toBe('room1');
      expect(state.players).toHaveLength(2);
      expect(state.players[0].socketId).toBeUndefined();
      expect(state.status).toBe('playing');
      expect(state.nextQueue).toHaveLength(7);
      expect(state.winnerId).toBeNull();

      expect(state.playerStates.p1).toBeDefined();
      expect(state.playerStates.p1.board).toHaveLength(20);
      expect(state.playerStates.p1.activePiece).not.toBeNull();
      expect(state.playerStates.p1.score).toBe(0);
      expect(state.playerStates.p1.status).toBe('playing');
      expect(state.playerStates.p1.queueIndex).toBe(1);
      expect(state.playerStates.p2.queueIndex).toBe(1);
    });

    it('playerStates is empty when game is waiting', () => {
      const state = game.toJSON();
      expect(state.status).toBe('waiting');
      expect(state.playerStates).toEqual({});
    });
  });

  describe('end-to-end gameplay', () => {
    it('handles game over when all players die', () => {
      setupTwoPlayers();
      game.start();

      // Kill both players by filling their spawn zones
      for (const pid of ['p1', 'p2']) {
        const ps = game.playerStates.get(pid);
        for (let r = 0; r < 4; r++) {
          for (let c = 0; c < 10; c++) {
            ps.board[r][c] = 'X';
          }
        }
        ps.activePiece = null;
        ps.status = 'over';
      }

      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBeNull();
    });

    it('declares winner when one player remains alive', () => {
      setupTwoPlayers();
      game.start();
      // Kill p1
      const ps1 = game.playerStates.get('p1');
      ps1.activePiece = null;
      ps1.status = 'over';
      game.tick();
      expect(game.status).toBe('over');
      expect(game.winnerId).toBe('p2');
    });

    it('players advance their queue independently without skipping pieces for others', () => {
      setupTwoPlayers();
      game.start();

      const p1Queue = game.playerStates.get('p1').queueIndex;
      const p2Queue = game.playerStates.get('p2').queueIndex;
      // Both started at 0 and both read index 0 → both at 1
      expect(p1Queue).toBe(1);
      expect(p2Queue).toBe(1);

      // p1 hard-drops 3 times fast → spawns 3 more pieces → queueIndex = 4
      for (let i = 0; i < 3; i++) {
        const ps1 = game.playerStates.get('p1');
        if (ps1.activePiece) {
          ps1.activePiece.row = 18;
          game.handleInput('p1', 'hardDrop');
        }
      }
      expect(game.playerStates.get('p1').queueIndex).toBe(4);

      // p2 hasn't consumed any more pieces → queueIndex still 1
      expect(game.playerStates.get('p2').queueIndex).toBe(1);

      // p2 can still access nextQueue[1] (next piece for p2)
      const p2Next = game.nextQueue[game.playerStates.get('p2').queueIndex];
      expect(p2Next).toBeDefined();
    });

    it('concurrent games do not interfere', () => {
      const game1 = new Game('room1');
      const game2 = new Game('room2');
      game1.addPlayer(new Player('p1', 's1', 'A', true));
      game2.addPlayer(new Player('p2', 's2', 'B', true));
      game1.start();
      game2.start();

      game1.handleInput('p1', 'hardDrop');
      expect(game1.playerStates.get('p1').score).toBeGreaterThanOrEqual(0);
      expect(game2.playerStates.get('p2').score).toBe(0);
    });
  });
});

describe('addPenaltyLines (pure function)', () => {
  it('adds 1 penalty row at bottom of empty board', () => {
    const board = createBoard(20, 10);
    const { board: result, gameOver } = addPenaltyLines(board, 1);
    expect(gameOver).toBe(false);
    expect(result[19]).toEqual(
      Array(10).fill('PENALTY'),
    );
  });

  it('fills rows from bottom up', () => {
    const board = createBoard(20, 10);
    const { board: r1 } = addPenaltyLines(board, 1);
    expect(r1[19]).toEqual(Array(10).fill('PENALTY'));
    expect(r1[18].every((c) => c === null)).toBe(true);

    const { board: r2 } = addPenaltyLines(r1, 1);
    expect(r2[18]).toEqual(Array(10).fill('PENALTY'));
    expect(r2[19]).toEqual(Array(10).fill('PENALTY'));
  });

  it('returns gameOver when spawn zone would be hit', () => {
    const board = createBoard(20, 10);
    // Fill all rows below the spawn zone (rows 4-19) with penalty
    let resultBoard = board;
    let gameOver = false;
    for (let i = 0; i < 16; i++) {
      const result = addPenaltyLines(resultBoard, 1);
      resultBoard = result.board;
      gameOver = result.gameOver;
      if (gameOver) break;
    }
    expect(gameOver).toBe(false);
    expect(resultBoard[4]).toEqual(Array(10).fill('PENALTY'));

    // One more hits row 3 (within row < 4 spawn zone) → game over
    const final = addPenaltyLines(resultBoard, 1);
    expect(final.gameOver).toBe(true);
  });

  it('overwrites existing piece cells with penalty', () => {
    const board = createBoard(20, 10);
    board[19] = Array(10).fill('T');
    const { board: result } = addPenaltyLines(board, 1);
    expect(result[19]).toEqual(Array(10).fill('PENALTY'));
  });
});
