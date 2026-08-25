import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import LandingPage from '../LandingPage';
import { navigate } from '../../router';

vi.mock('../../router', () => ({
  navigate: vi.fn(),
}));

describe('LandingPage', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it('renders the connection form', () => {
    render(<LandingPage />);
    expect(screen.getByText(/CONNECT_TO_GRID/i)).toBeInTheDocument();
  });

  it('renders room and operator ID inputs', () => {
    render(<LandingPage />);
    expect(screen.getByLabelText(/Room_ID/i)).toBeInTheDocument();
    expect(screen.getByLabelText(/Operator_ID/i)).toBeInTheDocument();
  });

  it('does not navigate when fields are empty', () => {
    render(<LandingPage />);
    const button = screen.getByText(/ENTER THE GRID/i);
    const form = button.closest('form');
    fireEvent.submit(form);
    expect(navigate).not.toHaveBeenCalled();
  });

  it('navigates to game room on valid submit', () => {
    render(<LandingPage />);
    fireEvent.change(screen.getByLabelText(/Room_ID/i), { target: { value: 'room1' } });
    fireEvent.change(screen.getByLabelText(/Operator_ID/i), { target: { value: 'alice' } });
    fireEvent.click(screen.getByText(/ENTER THE GRID/i));

    expect(navigate).toHaveBeenCalledWith('/room1/alice');
  });

  it('encodes special characters in room and player names', () => {
    render(<LandingPage />);
    fireEvent.change(screen.getByLabelText(/Room_ID/i), { target: { value: 'my room' } });
    fireEvent.change(screen.getByLabelText(/Operator_ID/i), { target: { value: 'bob@42' } });
    fireEvent.click(screen.getByText(/ENTER THE GRID/i));

    expect(navigate).toHaveBeenCalledWith('/my%20room/bob%4042');
  });
});
