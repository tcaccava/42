import websockets
import asyncio
import json

URL = "ws://127.0.0.1:8000/ws"

async def listen(ws):
    try:
        while True:
            msg = await ws.recv()
            data = json.loads(msg)
            if data["type"] == "state":
                print("\nState:", data["payload"])
    except Exception as e:
        print("\nListener stopped:", e)

async def send_input(ws):
    while True:
        moves_str = await asyncio.to_thread(input, "Write moves (es: 1,1 2,2): ")
        for coppia in moves_str.split():
            dx_str,dy_str = coppia.split(",")
            dx = int(dx_str)
            dy = int(dy_str)
            msg = {"type": "move", "payload": {"dx":dx,"dy":dy}}
            try:
                await ws.send(json.dumps(msg))
            except Exception as e:
                print("\nSend input error:", e)

async def main():
    try:
        ws = await websockets.connect(URL)
    except Exception as e:
        print("Cannot connect:", e)
        return
    try:
        name = input("Player name: ")
        join_msg = {"type": "join", "payload": {"name":name}}
        await ws.send(json.dumps(join_msg))
        print("Joined as", name)

        await asyncio.gather(listen(ws),send_input(ws))

    finally:
        await ws.close()

asyncio.run(main())

