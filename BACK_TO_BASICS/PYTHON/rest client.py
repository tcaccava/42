# import requests

# URL = "http://127.0.0.1:8000/move"

# data = {"name": "Tobia", "dx": 1, "dy": 2}

# response = requests.post(URL,json = data)

# print("Status code:", response.status_code)
# print("Risposta JSON:", response.json())

#############################################################
# import requests

# URL = "http://127.0.0.1:8000/move"

# while True:
#     nome = input("Nome del player: ")
#     dx = int(input("Delta X: "))
#     dy = int(input("Delta Y: "))

#     payload = {"name": nome, "dx": dx, "dy": dy}
#     r = requests.post(URL, json=payload)

#     if r.status_code == 200:
#         print("Stato del gioco:", r.json())
#     else:
#         print("Errore:", r.status_code, r.text)

#     cont = input("Vuoi fare un'altra mossa? (s/n) ")
#     if cont.lower() != "s":
#         break
################################################################3
import requests

URL = "http://127.0.0.1:8000/move"

name = input("Player name is: ")

while True:
    move_list = []
    input_moves = input("Write the move list in couples separated by comma, for example 1,1 2,-1 3,3 : ")
    for coppie in input_moves.split():
        coppia_dx,coppia_dy = coppie.split(",")
        diz = {"name": name, "dx": int(coppia_dx),"dy":int(coppia_dy)}
        r = requests.post(URL, json=diz)
        if r.status_code == 200:
            print("Game state :", r.json())
        else:
            print("Error:", r.status_code, r.text)
    cont = input("Do you want to continue? s/n:")
    if(cont != "s"):
        break



