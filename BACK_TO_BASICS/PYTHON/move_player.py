def move_player(a,b,c,d):
    e = a + c
    f = b + d
    return e,f

a,b = move_player(0,0,1,2)
print("Nuova posizione: ",a, b)
c, d = move_player(a,b,0,-1)
print("Nuova posizione: ", c,d)

