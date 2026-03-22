import asyncio
from contextlib import asynccontextmanager
from typing import Literal, Union, Dict, List
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from pydantic import BaseModel, Field

# --- 1. SCHEMI DI VALIDAZIONE (IL CONTRATTO) ---

class MovePayload(BaseModel):
    # Validiamo che dx e dy siano interi e non superino il MAX_STEP
    dx: int = Field(..., ge=-10, le=10)
    dy: int = Field(..., ge=-10, le=10)

class JoinPayload(BaseModel):
    # Validiamo che il nome non sia vuoto e non sia troppo lungo
    name: str = Field(..., min_length=1, max_length=20)

class ClientMessage(BaseModel):
    # Questo è il contenitore universale. 
    # Literal costringe 'type' a essere SOLO una di quelle due stringhe.
    type: Literal["join", "move"]
    payload: Union[MovePayload, JoinPayload]

# --- 2. LOGICA DI GIOCO (ENGINE) ---

class Player:
    def __init__(self, name: str):
        self.name = name
        self.x = 0
        self.y = 0

    def move(self, dx: int, dy: int):
        self.x += dx
        self.y += dy

class Game:
    def __init__(self):
        self.players: Dict[str, Player] = {}
        self.inputs: Dict[str, List[tuple]] = {}

    def add_player(self, name: str):
        if name not in self.players:
            self.players[name] = Player(name)

    def move_player(self, name: str, dx: int, dy: int):
        if name in self.players:
            self.players[name].move(dx, dy)

    def game_state(self):
        return {name: {"x": p.x, "y": p.y} for name, p in self.players.items()}

# --- 3. GESTIONE SERVER E WEB SOCKET ---

game = Game()
connections: Dict[WebSocket, str] = {}
new_clients = set()

@asynccontextmanager
async def lifespan(app: FastAPI):
    # Avvio del world loop come task in background
    task = asyncio.create_task(world_loop())
    yield
    task.cancel()
    print("Server shutdown")

app = FastAPI(lifespan=lifespan)

@app.websocket("/ws")
async def manage_ws(ws: WebSocket):
    await ws.accept()
    try:
        while True:
            # Riceviamo il JSON grezzo
            raw_data = await ws.receive_json()
            
            try:
                # TRASFORMAZIONE E VALIDAZIONE
                # Se raw_data non rispetta ClientMessage, scatta l'eccezione
                msg = ClientMessage(**raw_data)
                
                if msg.type == "join":
                    # Sappiamo per certo che msg.payload è un JoinPayload
                    name = msg.payload.name
                    connections[ws] = name
                    game.add_player(name)
                    print(f"Player {name} joined")

                elif msg.type == "move":
                    name = connections.get(ws)
                    if name:
                        # Sappiamo per certo che msg.payload è un MovePayload
                        if name not in game.inputs:
                            game.inputs[name] = []
                        game.inputs[name].append((msg.payload.dx, msg.payload.dy))

            except Exception as ve:
                # Se i dati sono sporchi, mandiamo un errore al client
                await ws.send_json({"type": "error", "message": "Invalid payload format"})
                print(f"Validation error: {ve}")

    except WebSocketDisconnect:
        name = connections.pop(ws, None)
        new_clients.discard(ws)
        print(f"Client {name} disconnected")

# --- 4. IL CICLO DI AGGIORNAMENTO (WORLD LOOP) ---

async def world_loop():
    while True:
        state_dirty = False
        
        # 1. Processiamo gli input accumulati
        for name, moves in game.inputs.items():
            if moves:
                for dx, dy in moves:
                    game.move_player(name, dx, dy)
                state_dirty = True
        game.inputs.clear()

        # 2. Controlliamo se ci sono nuovi client che hanno bisogno dello stato iniziale
        # (Uso una copia di connections per evitare errori di mutazione durante il loop)
        current_connections = list(connections.keys())
        for ws in current_connections:
            if ws not in new_clients:
                new_clients.add(ws)
                state_dirty = True

        # 3. Se qualcosa è cambiato, facciamo broadcast a TUTTI
        if state_dirty:
            state = {"type": "state", "payload": game.game_state()}
            for ws in current_connections:
                try:
                    await ws.send_json(state)
                except Exception:
                    # Se l'invio fallisce, lo gestiremo con la disconnessione
                    pass

        await asyncio.sleep(1/30) # 30 FPS