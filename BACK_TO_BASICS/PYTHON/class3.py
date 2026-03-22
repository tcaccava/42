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
    def create_player(self,name):
        self.players[name] = Player()
    def move_player(self,name,dx,dy):
        return self.players[name].move(dx,dy)
    def get_state(self):
        return {nome: (pos.x,pos.y) for nome,pos in self.players.items()}
    
game = Game()

game.create_player("Gino")
game.create_player("Pino")

game.move_player("Gino", 1,2)
game.move_player("Pino", 2,4)

print(game.get_state())#printa lo stato del gioco con i player e la loro posizione