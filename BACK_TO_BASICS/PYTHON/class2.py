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
        self.players = {}#dizionario vuoto
    def add_player(self,nome):
        self.players[nome] = Player()
    def move_player(self,nome,dx,dy):
        return self.players[nome].move(dx,dy)