class Player:
    def __init__(self):#in python il costruttore deve essere preceduto e seguito da un doppio underscore
        self.x = 0
        self.y = 0
    def move(self,dx,dy):
        self.x += dx
        self.y += dy
        return self.x,self.y

p = Player()
x,y = p.move(1,2)
print(x,y)
x,y = p.move(0,-1)
print(x,y)