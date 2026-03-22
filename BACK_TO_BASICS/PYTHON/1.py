import asyncio
from contextlib import asynccontextmanager
from typing import Literal, Union, Dict, List
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from pydantic import BaseModel, Field

# INPUT VALIDATION ---

class MovePayload(BaseModel):
    dy: str = Literal["up", "down"]
    input_id : int

class JoinPayload(BaseModel):
    name: str = Field(..., min_length=1, max_length=20)

class ClientMessage(BaseModel):
    type: Literal["join", "move"]
    payload: Union[MovePayload, JoinPayload]

# LOGICA DI GIOCO (ENGINE) ---

class Player:
    def __init__(self, name: str,side :str):
        self.name = name
        self.side = side

class Game:# Usiamo coordinate normalizzate per non dipendere dai pixel del frontend
 
    def __init__(self):
        self.players: Dict[str,Player] = {}
        self.inputs: Dict[str,List[(str,int)]] = {}
        self.last_input_id : Dict[str, int] = {}

        self.paddle_l_y = 0.5
        self.paddle_r_y = 0.5
        self.ball_x = 0.5
        self.ball_y = 0.5
        self.ball_dx = 0.01
        self.ball_dy = 0.01
        self.paddle_speed = 0.02

    def add_player(self, name: str) -> bool:
        if name in self.players or len(self.players) >= 2:
            return False
            
        # Assegna il lato in base a quanti sono già dentro
        side = "left" if len(self.players) == 0 else "right"
        self.players[name] = Player(name, side)
        return True

    def update_paddles(self):
        # Processa gli input accumulati
        for name, moves in self.inputs.items():
            player = self.players.get(name)
            if not player: continue
            for move_tuple in moves:
                dy,input_id = move_tuple
                if player.side == "left":
                    if dy == "up": self.paddle_l_y = max(0, self.paddle_l_y - self.paddle_speed)
                    if dy == "down": self.paddle_l_y = min(1, self.paddle_l_y + self.paddle_speed)
                else:
                    if dy == "up": self.paddle_r_y = max(0, self.paddle_r_y - self.paddle_speed)
                    if dy == "down": self.paddle_r_y = min(1, self.paddle_r_y + self.paddle_speed)
                self.last_input_id[name] = input_id
        self.inputs.clear()
    
    def update_ball(self):
        # 2. Muovi la pallina
        self.ball_x += self.ball_dx
        self.ball_y += self.ball_dy

        # 3. Rimbalzo muri (Sopra/Sotto)
        if self.ball_y <= 0 or self.ball_y >= 1:
            self.ball_dy *= -1

        # Reset se esce (Goal)
        if self.ball_x < 0 or self.ball_x > 1:
            self.ball_x, self.ball_y = 0.5, 0.5
            self.ball_dx *= -1 # Cambia direzione servizio

    def game_state(self): return({
        "p_left": self.paddle_l_y,
        "p_right": self.paddle_r_y,
        "ball": {"x": self.ball_x, "y": self.ball_y},
        "last_inputs": self.last_input_id 
        })

# GESTIONE SERVER E WEB SOCKET ---

game = Game()
connections: Dict[WebSocket, str] = {}

@asynccontextmanager
async def lifespan(app: FastAPI):
    # Avvio del world loop come task in background
    task = asyncio.create_task(world_loop())
    yield
    task.cancel()
    print("Server shutdown")

app = FastAPI(lifespan=lifespan)

async def world_loop():
    while True:
        game.update_paddles()
        game.update_ball()

        state = {"type": "state", "payload": game.game_state()}
        for ws in list(connections.keys()):
                try:
                    await ws.send_json(state)
                except Exception:
                    # Se l'invio fallisce, lo gestiremo con la disconnessione
                    pass

        await asyncio.sleep(1/60) # 60 FPS    

@app.websocket("/ws")
async def manage_ws(ws: WebSocket):
    await ws.accept()
    try:
        while True:
            # Riceviamo il JSON grezzo
            raw_data = await ws.receive_json()
            msg = ClientMessage(**raw_data)
                
            if msg.type == "join":
                if game.add_player(msg.payload.name):
                    connections[ws] = msg.payload.name
                else:
                    await ws.send_json({"type": "error", "payload": "Lobby full or name taken"})

            elif msg.type == "move":
                name = connections.get(ws)
                if name:
                    if name not in game.inputs:
                        game.inputs[name] = []
                    tupla = (msg.payload.dy,msg.payload.input_id)
                    game.inputs[name].append(tupla)
                    

    except WebSocketDisconnect:
        name = connections.pop(ws, None)
        print(f"Client {name} disconnected")

 









