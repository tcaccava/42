import { useState } from 'react';
import bgImage from '../assets/Neural_Tetris_bg.png';
import { navigate } from '../router';

function TetrisShapes() {
  return (
    <div className="relative w-64 h-64 md:w-96 md:h-96">
      <div className="absolute top-1/4 left-1/4 w-12 h-36 bg-primary-container neon-glow-primary border-2 border-primary-container/50 opacity-80" />
      <div className="absolute top-34 left-36 w-12 h-12 bg-primary-container neon-glow-primary border-2 border-primary-container/50 opacity-80" />
      <div className="absolute bottom-1/4 right-1/4 w-24 h-24 bg-secondary-container neon-glow-secondary border-2 border-secondary-container/50 opacity-80 mix-blend-screen" />
      <div className="absolute inset-0 border border-outline/20 bg-[linear-gradient(rgba(255,255,255,0.05)_1px,transparent_1px),linear-gradient(90deg,rgba(255,255,255,0.05)_1px,transparent_1px)] bg-size-[24px_24px] pointer-events-none" />
    </div>
  );
}

function JoinForm() {
  const [roomId, setRoomId] = useState('');
  const [operatorId, setOperatorId] = useState('');

  function handleSubmit(e) {
    e.preventDefault();
    if (!roomId.trim() || !operatorId.trim()) return;
    navigate(`/${encodeURIComponent(roomId.trim())}/${encodeURIComponent(operatorId.trim())}`);
  }

  return (
    <div className="glass-panel p-8 relative overflow-hidden">
      <div className="absolute top-2 right-2 font-technical-sm text-technical-sm text-outline-variant">
        S/N: 8492-NT42
      </div>

      <h2 className="font-headline-lg-mobile md:font-headline-lg text-headline-lg-mobile md:text-headline-lg text-primary mb-6">
        CONNECT_TO_GRID
      </h2>

      <form className="flex flex-col gap-6" onSubmit={handleSubmit}>
        <div className="flex flex-col gap-2">
          <label
            className="font-technical-sm text-technical-sm text-on-surface-variant uppercase tracking-widest"
            htmlFor="roomId"
          >
            Room_ID
          </label>
          <input
            className="bg-transparent border-0 border-b-2 border-outline text-on-surface font-technical-sm focus:ring-0 focus:border-secondary focus:shadow-[0_2px_10px_rgba(0,238,252,0.2)] transition-all px-2 py-3 outline-none"
            id="roomId"
            name="roomId"
            placeholder="ENTER_ROOM_ID"
            required
            type="text"
            value={roomId}
            onChange={(e) => setRoomId(e.target.value)}
          />
        </div>

        <div className="flex flex-col gap-2">
          <label
            className="font-technical-sm text-technical-sm text-on-surface-variant uppercase tracking-widest"
            htmlFor="operatorId"
          >
            Operator_ID
          </label>
          <input
            className="bg-transparent border-0 border-b-2 border-outline text-on-surface font-technical-sm focus:ring-0 focus:border-secondary focus:shadow-[0_2px_10px_rgba(0,238,252,0.2)] transition-all px-2 py-3 outline-none"
            id="operatorId"
            name="operatorId"
            placeholder="ENTER_USERNAME"
            required
            type="text"
            value={operatorId}
            onChange={(e) => setOperatorId(e.target.value)}
          />
        </div>

        <button
          className="mt-4 bg-primary-container text-on-primary-container font-label-caps text-label-caps uppercase py-4 px-6 border-2 border-transparent hover:border-primary transition-all neon-glow-button relative overflow-hidden group"
          type="submit"
        >
          <span className="relative z-10 flex items-center justify-center gap-2">
            <span className="material-symbols-outlined text-lg">login</span>
            ENTER THE GRID
          </span>
          <div className="absolute inset-0 bg-white/10 w-full h-full scan-effect" />
        </button>
      </form>
    </div>
  );
}

function LandingPage() {
  return (
    <main className="grow pt-20 pb-20 relative z-10 flex items-center justify-center">
      <div
        className="absolute inset-0 z-0 opacity-40 mix-blend-screen"
        style={{
          backgroundImage: `url(${bgImage})`,
          backgroundSize: 'cover',
          backgroundPosition: 'center',
        }}
        role="img"
        aria-label="A dark, rainy industrial cityscape at night in a dystopian cyberpunk style"
      />

      <div className="container mx-auto px-margin-mobile md:px-margin-desktop z-10 relative">
        <div className="grid grid-cols-1 md:grid-cols-12 gap-gutter items-center">
          <div className="col-span-1 md:col-span-7 flex justify-center items-center relative">
            <TetrisShapes />
          </div>
          <div className="col-span-1 md:col-span-5">
            <JoinForm />
          </div>
        </div>
      </div>
    </main>
  );
}

export default LandingPage;
