from fastapi import FastAPI
from pydantic import BaseModel

#il frontend e' codice remoto seguito localmente

class Player:
    def __init__(self):
        self.x = 0
        self.y = 0
    def move(self,dx,dy):
        self.x += dx
        self.y += dy
        return self.x,self.y
    
class Game:
    def __init__(self):
        self.players = {}
    def add_player(self,name):
        self.players[name] = Player()
    def move_player(self,name,dx,dy):
        if name not in self.players:
            self.add_player(name)
        return self.players[name].move(dx,dy)
    def get_state(self):
        return {name : (pos.x,pos.y) for name,pos in self.players.items()}

class Move(BaseModel):
    name : str
    dx : int
    dy : int

app = FastAPI()
game = Game()

@app.post("/move")
async def move(move : Move):
    game.move_player(move.name,move.dx,move.dy)
    return game.get_state()
    
    