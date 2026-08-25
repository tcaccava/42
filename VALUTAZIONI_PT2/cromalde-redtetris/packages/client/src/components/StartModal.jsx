import { useGameStore } from '../store';
import socket from '../socket';

function StartModal() {
  const players = useGameStore((s) => s.players);
  const myId = useGameStore((s) => s.playerId);

  const isHost = players.length > 0 && myId
    ? players.find((p) => p.isHost)?.id === myId
    : false;

  function handleStart() {
    if (isHost) {
      socket.emit('startGame');
    }
  }

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black/70">
      <div className="hologram-glass border border-tertiary/40 p-8 text-center max-w-sm w-full mx-4">
        <h1 className="font-headline-lg text-headline-lg text-tertiary mb-2 tracking-wider">
          RED TETRIS
        </h1>
        <p className="font-technical-sm text-technical-sm text-on-surface-variant mb-4">
          READY TO INITIALIZE NEURAL LINK
        </p>

        {players.length > 0 && (
          <div className="mb-6 font-technical-sm text-technical-sm text-on-surface-variant">
            <span className="text-secondary">OPERATORS ONLINE:</span>
            <ul className="mt-2 space-y-1">
              {players.map((p) => (
                <li
                  key={p.id}
                  className={`${p.isHost ? 'text-tertiary' : 'text-on-surface'}`}
                >
                  {p.name} {p.isHost ? '[HOST]' : ''}
                  {!p.connected ? ' (DISCONNECTED)' : ''}
                </li>
              ))}
            </ul>
          </div>
        )}

        {isHost ? (
          <button
            onClick={handleStart}
            className="cursor-pointer bg-tertiary text-surface-container-lowest font-label-caps text-label-caps px-8 py-3 tracking-widest hover:brightness-110 transition-all border border-tertiary"
          >
            START
          </button>
        ) : (
          <p className="font-technical-sm text-technical-sm text-on-surface-variant">
            WAITING FOR HOST TO START
          </p>
        )}
      </div>
    </div>
  );
}

export default StartModal;
