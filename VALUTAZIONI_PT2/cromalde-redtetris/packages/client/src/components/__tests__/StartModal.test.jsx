import { describe, it, expect, beforeEach, vi } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import { useGameStore } from '../../store';
import socket from '../../socket';
import StartModal from '../StartModal';

vi.mock('../../socket', () => ({
  default: { emit: vi.fn() },
}));

describe('StartModal', () => {
  beforeEach(() => {
    useGameStore.getState().resetGame();
    vi.clearAllMocks();
  });

  it('renders the title', () => {
    render(<StartModal />);
    expect(screen.getByText(/RED TETRIS/i)).toBeInTheDocument();
  });

  it('shows waiting message when player is not the host', () => {
    useGameStore.setState({
      players: [{ id: 'host1', name: 'Alice', isHost: true, ready: false, connected: true }],
      playerId: 'not-host',
    });
    render(<StartModal />);
    expect(screen.getByText(/WAITING FOR HOST/i)).toBeInTheDocument();
  });

  it('shows start button when player is host', () => {
    useGameStore.setState({
      players: [{ id: 'p1', name: 'Alice', isHost: true, ready: false, connected: true }],
      playerId: 'p1',
    });
    render(<StartModal />);
    expect(screen.getByText('START')).toBeInTheDocument();
  });

  it('emits startGame when host clicks start', () => {
    useGameStore.setState({
      players: [{ id: 'p1', name: 'Alice', isHost: true, ready: false, connected: true }],
      playerId: 'p1',
    });
    render(<StartModal />);
    fireEvent.click(screen.getByText('START'));
    expect(socket.emit).toHaveBeenCalledWith('startGame');
  });

  it('does not show start button when no players', () => {
    useGameStore.setState({ players: [], playerId: null });
    render(<StartModal />);
    expect(screen.queryByText('START')).not.toBeInTheDocument();
  });

  it('shows disconnected status for offline players', () => {
    useGameStore.setState({
      players: [
        { id: 'h', name: 'Host', isHost: true, ready: false, connected: true },
        { id: 'p2', name: 'Bob', isHost: false, ready: false, connected: false },
      ],
      playerId: 'h',
    });
    render(<StartModal />);
    expect(screen.getByText(/DISCONNECTED/i)).toBeInTheDocument();
  });
});
