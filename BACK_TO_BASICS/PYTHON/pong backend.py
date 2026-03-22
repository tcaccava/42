import asyncio
from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket, WebSocketDisconnect

MAX_STEP = 10

class Player :
    def __init__(self):
        self.x = 0
        self.y = 0
    def move(self,dx,dy):
        dx = max(-MAX_STEP, min(MAX_STEP,dx))
        dy = max(-MAX_STEP, min(MAX_STEP, dy))
        self.x += dx
        self.y += dy

class Game :
    def __init__(self):
        self.players = {}
        self.inputs = {}
    def add_player(self,name):
        if name not in self.players:
            self.players[name] = Player()
    def move_player(self,name,dx,dy):
        if name in self.players:
            self.players[name].move(dx,dy)
    def game_state(self):
        return {name : (p.x,p.y) for name,p in self.players.items()}
    

@asynccontextmanager
async def lifespan(app : FastAPI):
    asyncio.create_task(world_loop())
    yield
    print("Server shutdown")

app = FastAPI(lifespan=lifespan)
game = Game()
connections = {}
new_clients = set()

@app.websocket("/ws")
async def manage_ws(ws : WebSocket):
    await ws.accept()
    try :    
        while True:
            data = await ws.receive_json()
            msg_type = data.get("type")
            payload = data.get("payload",{})
            if msg_type == "join":
                name = payload.get("name")
                if not name:
                    await ws.send_json({"type": "error", "payload": {"msg": "bad join"}})
                    continue
                connections[ws] = name
                game.add_player(name)
            
            elif msg_type == "move":
                name = connections.get(ws)
                if not name:
                    await ws.send_json({"type": "error", "payload": {"msg": "not joined"}})
                    continue
                dx = payload.get("dx",0)
                dy = payload.get("dy",0)
                if name not in game.inputs:
                    game.inputs[name] = []
                game.inputs[name].append((dx,dy))

    except WebSocketDisconnect:
            print("Client disconnected")
            connections.pop(ws,None)

async def world_loop() :
    while True :
        state_dirty = False
        for name, moves in game.inputs.items():
            for dx,dy in moves:
                game.move_player(name,dx,dy)
            if moves:
                state_dirty = True
        game.inputs.clear()
        for ws in connections:
            if ws not in new_clients:
                new_clients.add(ws)
                state_dirty = True

        if state_dirty:
            state = {"type": "state", "payload": game.game_state()}
            disconnected = []
            for client in list(connections):
                try:
                    await client.send_json(state)
                except WebSocketDisconnect:
                    disconnected.append(client)
            for d in disconnected:
                connections.pop(d,None)
                new_clients.discard(d)
        await asyncio.sleep(1/30) 

