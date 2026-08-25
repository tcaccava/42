import { describe, it, expect } from 'vitest';
import Player from '../Player.js';

describe('Player', () => {
  it('constructs with id, socketId, name, isHost', () => {
    const p = new Player('p1', 'socket1', 'Alice', true);
    expect(p.id).toBe('p1');
    expect(p.socketId).toBe('socket1');
    expect(p.name).toBe('Alice');
    expect(p.isHost).toBe(true);
    expect(p.ready).toBe(false);
    expect(p.connected).toBe(true);
  });

  it('defaults isHost to false and ready to false', () => {
    const p = new Player('p2', 'socket2', 'Bob');
    expect(p.isHost).toBe(false);
    expect(p.ready).toBe(false);
  });

  it('markReady toggles ready state', () => {
    const p = new Player('p1', 's1', 'Alice');
    expect(p.ready).toBe(false);
    p.markReady();
    expect(p.ready).toBe(true);
    p.markReady();
    expect(p.ready).toBe(false);
  });

  it('setHost updates isHost', () => {
    const p = new Player('p1', 's1', 'Alice');
    p.setHost(true);
    expect(p.isHost).toBe(true);
    p.setHost(false);
    expect(p.isHost).toBe(false);
  });

  it('disconnect sets connected to false', () => {
    const p = new Player('p1', 's1', 'Alice');
    p.disconnect();
    expect(p.connected).toBe(false);
  });

  it('reconnect updates socketId and sets connected to true', () => {
    const p = new Player('p1', 's1', 'Alice');
    p.disconnect();
    p.reconnect('socket2');
    expect(p.connected).toBe(true);
    expect(p.socketId).toBe('socket2');
  });
});
