from fastapi import FastAPI, WebSocket, WebSocketDisconnect

class Player:
    def __init__(self):
        self.x = 0
        self.y = 0

    def move(self, dx, dy):
        self.x += dx
        self.y += dy

class Game:
    def __init__(self):
        self.players = {}

    def add_player(self, name):
        if name not in self.players:
            self.players[name] = Player()

    def move_player(self, name, dx, dy):
        self.players[name].move(dx, dy)

    def get_state(self):
        return {name: (p.x, p.y) for name, p in self.players.items()}

app = FastAPI()
game = Game()

@app.websocket("/ws")
async def manage_websocket(ws: WebSocket):
    await ws.accept()
    try:
        while True:
            data = await ws.receive_json()

            player = data.get("player")
            move_info = data.get("moves")

            if not player or not move_info:
                await ws.send_json({"error": "bad request"})
                continue

            game.add_player(player)

            dx = move_info.get("dx", 0)
            dy = move_info.get("dy", 0)

            game.move_player(player, dx, dy)
            await ws.send_json(game.get_state())

    except WebSocketDisconnect:
        print("Client disconnected")
