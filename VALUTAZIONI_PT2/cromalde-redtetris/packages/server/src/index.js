import express from 'express';
import { createServer } from 'node:http';
import { Server } from 'socket.io';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import Game from './Game.js';
import Player from './Player.js';

const __dirname = dirname(fileURLToPath(import.meta.url));

const app = express();
const server = createServer(app);
const io = new Server(server, { path: '/ws' });

const PORT = process.env.PORT ?? 3000;
const DROP_INTERVAL = 1000;

/** @type {Map<string, { game: Game, interval: ReturnType<typeof setInterval> | null }>} */
const rooms = new Map();
/** @type {Map<string, { roomId: string, playerId: string }>} */
const socketMap = new Map();

app.use(express.static(join(__dirname, '..', 'public')));

app.get('/{*path}', (_req, res) => {
  res.sendFile(join(__dirname, '..', 'public', 'index.html'));
});

/* ---- helpers ---- */

function broadcastGameState(roomId) {
  const room = rooms.get(roomId);
  if (!room) return;
  io.to(roomId).emit('gameState', room.game.toJSON());
}

function startGameLoop(roomId) {
  const room = rooms.get(roomId);
  if (!room) return;
  if (room.interval) clearInterval(room.interval);
  room.interval = setInterval(() => {
    const r = rooms.get(roomId);
    if (!r || r.game.status !== 'playing') {
      if (r) {
        clearInterval(r.interval);
        r.interval = null;
      }
      return;
    }
    r.game.tick();
    broadcastGameState(roomId);
  }, DROP_INTERVAL);
}

function cleanupRoom(roomId) {
  const room = rooms.get(roomId);
  if (!room) return;
  if (room.interval) clearInterval(room.interval);
  rooms.delete(roomId);
}

/* ---- Socket.io events ---- */

io.on('connection', (socket) => {
  socket.on('joinRoom', ({ roomId, playerName, playerId } = {}) => {
    if (!roomId || !playerName) {
      socket.emit('error', 'roomId and playerName are required');
      return;
    }

    let room = rooms.get(roomId);
    if (!room) {
      room = { game: new Game(roomId), interval: null };
      rooms.set(roomId, room);
    }

    const { game } = room;

    // Reconnection: player sent a known clientId
    const existingPlayer = game.players.find((p) => p.id === playerId);
    if (existingPlayer) {
      // Name changed — user signaled a new identity, treat as new player
      if (existingPlayer.name !== playerName) {
        game.removePlayer(playerId);
      } else {
        if (game.status === 'over') game.status = 'waiting';
        game.reconnectPlayer(playerId, socket.id);
        socket.join(roomId);
        socketMap.set(socket.id, { roomId, playerId });
        socket.emit('joined', { playerId, isHost: existingPlayer.isHost, roomId });
        broadcastGameState(roomId);
        return;
      }
    }

    // Reset to lobby for new players after game over
    if (game.status === 'over') {
      game.status = 'waiting';
    }

    const isHost = game.players.length === 0;
    const player = new Player(playerId, socket.id, playerName, isHost);
    game.addPlayer(player);

    socket.join(roomId);
    socketMap.set(socket.id, { roomId, playerId });

    socket.emit('joined', { playerId, isHost, roomId });
    broadcastGameState(roomId);
  });

  socket.on('startGame', () => {
    const entry = socketMap.get(socket.id);
    if (!entry) return;
    const room = rooms.get(entry.roomId);
    if (!room) return;
    const player = room.game.players.find((p) => p.id === entry.playerId);
    if (!player || !player.isHost) return;
    if (room.game.status === 'playing') return;

    room.game.start();
    startGameLoop(entry.roomId);
    broadcastGameState(entry.roomId);
  });

  socket.on('input', ({ action } = {}) => {
    const entry = socketMap.get(socket.id);
    if (!entry) return;
    const room = rooms.get(entry.roomId);
    if (!room) return;
    room.game.handleInput(entry.playerId, action);
    broadcastGameState(entry.roomId);
  });

  socket.on('disconnect', () => {
    const entry = socketMap.get(socket.id);
    if (!entry) return;
    const room = rooms.get(entry.roomId);
    if (!room) return;

    const player = room.game.players.find((p) => p.id === entry.playerId);
    if (player) {
      const wasHost = player.isHost;
      player.disconnect();
      if (wasHost) {
        player.setHost(false);
        const nextHost = room.game.players.find(
          (p) => p.connected && p.id !== entry.playerId,
        );
        if (nextHost) nextHost.setHost(true);
      }
    }
    socketMap.delete(socket.id);

    const hasConnected = room.game.players.some((p) => p.connected);
    if (!hasConnected) {
      cleanupRoom(entry.roomId);
    } else {
      broadcastGameState(entry.roomId);
    }
  });
});

server.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
