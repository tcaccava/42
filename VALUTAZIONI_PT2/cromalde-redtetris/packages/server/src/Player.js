class Player {
  constructor(id, socketId, name, isHost = false) {
    this.id = id;
    this.socketId = socketId;
    this.name = name;
    this.isHost = isHost;
    this.ready = false;
    this.connected = true;
  }

  markReady() {
    this.ready = !this.ready;
  }

  setHost(value) {
    this.isHost = value;
  }

  disconnect() {
    this.connected = false;
  }

  reconnect(socketId) {
    this.socketId = socketId;
    this.connected = true;
  }
}

export default Player;
