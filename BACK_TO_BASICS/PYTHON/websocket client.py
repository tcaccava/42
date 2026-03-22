import websocket
import json

url = "ws://127.0.0.1:8000/ws"

ws = websocket.create_connection(url)

while True :
    name = input("My name is: ")
    move_data = input("Write moves separated by comma,like this 1,1 2,2 : ")
    for coppie in move_data.split():
        dx,dy = coppie.split(",")
        dx = int(dx)
        dy = int(dy)
        moves = {"dx": dx, "dy": dy}
        diz = {"player": name, "moves": moves}
        ws.send(json.dumps(diz))
        response = ws.recv()
        print(response)
    cont = input("Do you want to continue? s/n:")
    if(cont != "s"):
        break
            




