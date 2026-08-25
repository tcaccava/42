import { io } from 'socket.io-client';

const socket = io(window.location.origin, {
  path: '/ws',
  autoConnect: false,
});

const STORAGE_KEY = 'rt_player_id';

/**
 * Returns a stable player ID persisted across sessions.
 * @returns {string}
 */
function getPlayerId() {
  let id = localStorage.getItem(STORAGE_KEY);
  if (!id) {
    id = crypto.randomUUID();
    localStorage.setItem(STORAGE_KEY, id);
  }
  return id;
}

export default socket;
export { getPlayerId };
