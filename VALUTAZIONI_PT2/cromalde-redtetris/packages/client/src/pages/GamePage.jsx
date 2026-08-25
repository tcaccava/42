import { useState, useEffect, useCallback, useRef } from 'react';
import { useGameStore, selectGhostRow, selectDanger } from '../store';
import GameBoard from '../components/GameBoard';
import PiecePreview from '../components/PiecePreview';
import MiniGameBoard from '../components/MiniGameBoard';
import StartModal from '../components/StartModal';
import socket, { getPlayerId } from '../socket';

const KEY_ACTION_MAP = {
  ArrowLeft: 'moveLeft',
  ArrowRight: 'moveRight',
  ArrowUp: 'rotateCW',
  ArrowDown: 'softDrop',
  ' ': 'hardDrop',
};

/**
 * In-game session page with server-driven Tetris.
 * @param {Object} props
 * @param {string} props.roomName
 * @param {string} props.playerName
 */
function GamePage({ roomName, playerName }) {
  const gameStatus = useGameStore((s) => s.gameStatus);
  const board = useGameStore((s) => s.board);
  const activePiece = useGameStore((s) => s.activePiece);
  const nextPiece = useGameStore((s) => s.nextPiece);
  const score = useGameStore((s) => s.score);
  const linesCleared = useGameStore((s) => s.linesCleared);
  const holdPiece = useGameStore((s) => s.holdPiece);
  const players = useGameStore((s) => s.players);
  const playerStates = useGameStore((s) => s.playerStates);
  const winnerId = useGameStore((s) => s.winnerId);
  const myId = useGameStore((s) => s.playerId);

  const [joined, setJoined] = useState(false);
  const [error, setError] = useState(/** @type {string|null} */ (null));
  const errorTimer = useRef(null);

  /* ---- Socket lifecycle ---- */

  useEffect(() => {
    socket.connect();

    function onConnect() {
      const playerId = getPlayerId();
      useGameStore.getState().setPlayerId(playerId);
      socket.emit('joinRoom', { roomId: roomName, playerName, playerId });
    }

    function onJoined() {
      setJoined(true);
    }

    function onGameState(data) {
      setJoined(true);
      useGameStore.getState().updateFromServer(data);
    }

    function onError(msg) {
      setError(String(msg));
      if (errorTimer.current) clearTimeout(errorTimer.current);
      errorTimer.current = setTimeout(() => setError(null), 5000);
    }

    if (socket.connected) {
      onConnect();
    }
    socket.on('connect', onConnect);
    socket.on('joined', onJoined);
    socket.on('gameState', onGameState);
    socket.on('error', onError);

    return () => {
      socket.off('connect', onConnect);
      socket.off('joined', onJoined);
      socket.off('gameState', onGameState);
      socket.off('error', onError);
      socket.disconnect();
      useGameStore.getState().resetGame();
      if (errorTimer.current) clearTimeout(errorTimer.current);
    };
  }, [roomName, playerName]);

  /* ---- Keyboard controls ---- */

  const handleKey = useCallback((e) => {
    const st = useGameStore.getState();
    if (st.gameStatus !== 'playing') return;
    const myPs = st.playerStates[st.playerId];
    if (myPs?.status === 'over') return;
    const action = KEY_ACTION_MAP[e.key];
    if (action) {
      e.preventDefault();
      socket.emit('input', { action });
      return;
    }
    if (e.key === 'c' || e.key === 'C') {
      e.preventDefault();
    }
  }, []);

  useEffect(() => {
    window.addEventListener('keydown', handleKey);
    return () => window.removeEventListener('keydown', handleKey);
  }, [handleKey]);

  /* ---- Ghost + danger auto-refresh ---- */

  const ghostRowLive = activePiece
    ? selectGhostRow({ board, activePiece })
    : null;
  const dangerLive = selectDanger({ board });

  /* ---- Play again handler ---- */

  function handlePlayAgain() {
    const me = players.find((p) => p.id === myId);
    if (me && me.isHost) {
      socket.emit('startGame');
    }
  }

  /* ---- My status ---- */

  const myPlayerState = myId ? playerStates[myId] : null;
  const amDead = myPlayerState?.status === 'over';
  const isHost = players.find((p) => p.id === myId)?.isHost ?? false;

  const status = gameStatus === 'idle' || gameStatus === 'waiting' ? 'waiting' : gameStatus;

  if (status === 'waiting' && !joined) {
    return (
      <main className="grow flex items-center justify-center">
        <div className="hologram-glass border border-tertiary/40 p-8 text-center max-w-sm w-full mx-4">
          <p className="font-technical-sm text-technical-sm text-on-surface-variant animate-pulse">
            CONNECTING TO GRID...
          </p>
        </div>
      </main>
    );
  }

  if (status === 'waiting') {
    return <StartModal />;
  }

  return (
    <main className="grow mt-24 mb-20 px-margin-mobile md:px-margin-desktop w-full max-w-container-max mx-auto grid grid-cols-4 md:grid-cols-12 gap-gutter relative">
      {/* Error toast */}
      {error && (
        <div className="fixed top-4 right-4 z-50 hologram-glass border border-error/60 p-4 max-w-sm animate-in fade-in">
          <p className="font-technical-sm text-technical-sm text-error">{error}</p>
        </div>
      )}

      {/* Background atmospheric glow */}
      <div className="absolute inset-0 z-0 pointer-events-none opacity-20 bg-[radial-gradient(ellipse_at_center,rgba(0,238,252,0.15)_0%,rgba(19,19,20,0)_70%)]" />

      {/* ---- Left Sidebar ---- */}
      <aside className="col-span-4 md:col-span-3 flex flex-col gap-unit z-10">
        {/* Score Module */}
        <div className="hologram-glass border border-outline/30 p-4 relative neon-glow-secondary">
          <div className="absolute top-1 right-2 font-technical-sm text-technical-sm text-secondary/50">
            MOD_01
          </div>
          <h2 className="font-label-caps text-label-caps text-secondary mb-2 tracking-widest">
            SYSTEM_SCORE
          </h2>
          <div className="font-headline-lg text-headline-lg text-secondary-fixed">
            {score.toLocaleString()}
          </div>
          <div className="mt-4 border-t border-outline/20 pt-2 flex justify-between font-technical-sm text-technical-sm text-on-surface-variant">
            <span>LINES_CLEARED</span>
            <span className="text-on-surface">{linesCleared}</span>
          </div>
          <div className="flex justify-between font-technical-sm text-technical-sm text-on-surface-variant mt-1">
            <span>APM_RATE</span>
            <span className="text-on-surface">
              {linesCleared > 0 ? '--' : '0.0'}
            </span>
          </div>
        </div>

        {/* Next Piece Module */}
        <div className="hologram-glass border border-outline/30 p-4 relative">
          <div className="absolute top-1 right-2 font-technical-sm text-technical-sm text-outline/50">
            MOD_02
          </div>
          <h2 className="font-label-caps text-label-caps text-on-surface-variant mb-4 tracking-widest">
            NEXT_PROTO
          </h2>
          <div className="flex justify-center">
            {nextPiece ? (
              <PiecePreview name={nextPiece.name} rotation={nextPiece.rotation} />
            ) : (
              <div className="text-on-surface-variant text-technical-sm">EMPTY</div>
            )}
          </div>
        </div>

        {holdPiece && (
          <div className="hologram-glass border border-outline/30 p-4 relative">
            <div className="absolute top-1 right-2 font-technical-sm text-technical-sm text-outline/50">
              MOD_03
            </div>
            <h2 className="font-label-caps text-label-caps text-on-surface-variant mb-4 tracking-widest">
              BUFFER
            </h2>
            <div className="flex justify-center">
              <PiecePreview name={holdPiece.name} rotation={holdPiece.rotation} />
            </div>
          </div>
        )}
      </aside>

      {/* ---- Center: Game Board ---- */}
      <section className="col-span-4 md:col-span-6 flex justify-center items-start z-10">
        <div className="relative inline-block border-2 border-outline-variant bg-surface-container-lowest p-2 shadow-[0_0_30px_rgba(255,95,0,0.1)]">
          <div className="absolute inset-0 pointer-events-none scanline z-20" />
          <div className="global-scanner" />
          <GameBoard
            board={board}
            activePiece={amDead ? null : activePiece}
            ghostRow={amDead ? null : ghostRowLive}
          />
          {(amDead || gameStatus === 'over') && (
            <div className="absolute inset-0 z-30 flex flex-col items-center justify-center bg-black/60 gap-4">
              {winnerId ? (
                <span className="font-headline-lg text-headline-lg text-tertiary border-2 border-tertiary px-6 py-3 animate-pulse">
                  {winnerId === myId
                    ? 'YOU WIN'
                    : `${players.find((p) => p.id === winnerId)?.name ?? 'OPERATOR'} WINS`
                  }
                </span>
              ) : (
                <span className="font-headline-lg text-headline-lg text-error border-2 border-error px-6 py-3 animate-pulse">
                  GRID_FAILURE
                </span>
              )}
              {gameStatus === 'over' ? (
                isHost ? (
                  <button
                    onClick={handlePlayAgain}
                    className="cursor-pointer bg-surface-container-lowest text-on-surface font-label-caps text-label-caps px-6 py-2 tracking-widest border border-outline hover:border-secondary/50 transition-all"
                  >
                    PLAY AGAIN
                  </button>
                ) : (
                  <p className="font-technical-sm text-technical-sm text-on-surface-variant">
                    WAITING FOR HOST TO RESTART
                  </p>
                )
              ) : (
                <p className="font-technical-sm text-technical-sm text-on-surface-variant">
                  NEURAL LINK SEVERED — WAITING FOR TEAM
                </p>
              )}
            </div>
          )}
        </div>
      </section>

      {/* ---- Right Sidebar: other players' ghost fields ---- */}
      <aside className="col-span-4 md:col-span-3 flex flex-col gap-4 z-10">
        <h2 className="font-label-caps text-label-caps text-primary border-b-2 border-outline-variant pb-2 tracking-widest flex items-center gap-2">
          <span className="w-2 h-2 rounded-full bg-secondary animate-pulse" />
          ACTIVE_NEURAL_LINKS
        </h2>

        <div className="flex flex-col gap-3 overflow-y-auto max-h-150 pr-2 scrollbar-hide">
          {players
            .filter((p) => p.id !== myId)
            .map((p) => {
              const ps = playerStates[p.id];
              return (
                <div key={p.id} className="hologram-glass p-2 border border-outline/20">
                  <div className="font-technical-sm text-technical-sm text-on-surface truncate mb-1">
                    {p.name}
                    {!p.connected ? ' (DISCONNECTED)' : ''}
                  </div>
                  <div className="font-label-caps text-label-caps text-secondary/70 mb-2">
                    {ps ? ps.score.toLocaleString() : '—'}
                  </div>
                  {ps && p.connected ? (
                    <MiniGameBoard
                      board={ps.board}
                      isDead={ps.status === 'over'}
                    />
                  ) : (
                    <div className="w-24 h-48 bg-surface-container-lowest border border-outline/20 flex items-center justify-center">
                      <span className="font-technical-sm text-technical-sm text-on-surface-variant">
                        OFFLINE
                      </span>
                    </div>
                  )}
                </div>
              );
            })}
        </div>
      </aside>
    </main>
  );
}

export default GamePage;
