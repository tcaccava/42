import asyncio
import json
import time
from contextlib import asynccontextmanager
from typing import Literal, Union, Dict, List , Set
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from pydantic import BaseModel, Field

# --- VALIDAZIONE INPUT

class MovePayload(BaseModel):
    direction : Literal["UP","DOWN","LEFT","RIGHT"]
    player_id : int # 0-3 per identificare lo slot

class ClientMessage(BaseModel):
    type: Literal["move"]
    payload : MovePayload

# --- LOGICA DI GIOCO

class Ball:
    def __init__(self):
        # Posizione centrale normalizzata
        self.x = 0.5
        self.y = 0.5
        # Velocità normalizzata (frazione di spazio al secondo / tick)
        self.vx = 0.008
        self.vy = 0.004
        self.radius = 0.013 # Come concordato nel JSON

    def update(self):
        # Spostamento lineare puro
        # La logica di rimbalzo/goal viene gestita dal GameState
        # perché dipende dalla posizione dei 4 player
        self.x += self.vx
        self.y += self.vy

    def reset(self):
        """Riporta la palla al centro dopo un punto"""
        self.x = 0.5
        self.y = 0.5
        # Qui potresti aggiungere un pizzico di casualità alla direzione
        self.vx *= -1 
        self.vy *= -1

    def serialize(self):
        """Conforme al metodo serialize() di GameState.js"""
        return {
            "x": round(self.x, 4),
            "y": round(self.y, 4),
            "vx": round(self.vx, 4),
            "vy": round(self.vy, 4),
            "radius": self.radius
        }
    
class Player:
    def __init__(self, id, side, name="Anonymous"):
        self.id = id
        self.side = side # 'left', 'right', 'top', 'bottom'
        self.name = name
        self.score = 0
        self.connected = True
        self.votedToAbandon = False
        
        # Dimensioni standard (normalizzate)
        # Per i lati left/right: width è lo spessore, height è la lunghezza
        # Per i lati top/bottom: width è la lunghezza, height è lo spessore
        if side in ['left', 'right']: 
            self.width = 0.015
            self.height = 0.13
            self.x = 0.02 if side == 'left' else 0.965
            self.y = 0.435 # Centrato verticalmente (0.5 - height/2)
        else: # top o bottom
            self.width = 0.13
            self.height = 0.015
            self.x = 0.435 # Centrato orizzontalmente (0.5 - width/2)
            self.y = 0.02 if side == 'top' else 0.965

    def move(self, direction, speed=0.02):
        """
        Aggiorna la posizione in base al lato.
        'UP'/'DOWN' per i verticali, 'LEFT'/'RIGHT' per gli orizzontali.
       
        """
        if self.side in ['left', 'right']:
            if direction == 'UP':
                self.y = max(0, self.y - speed)
            elif direction == 'DOWN':
                self.y = min(1 - self.height, self.y + speed)
        
        elif self.side in ['top', 'bottom']:
            if direction == 'LEFT':
                self.x = max(0, self.x - speed)
            elif direction == 'RIGHT':
                self.x = min(1 - self.width, self.x + speed)

    def serialize(self):
        """Conforme alla struttura richiesta in GameState.js"""
        return {
            "id": self.id,
            "side": self.side,
            "x": round(self.x, 4),
            "y": round(self.y, 4),
            "width": self.width,
            "height": self.height,
            "score": self.score,
            "connected": self.connected,
            "name": self.name,
            "votedToAbandon": self.votedToAbandon
        }
    
class PongGameState:
    def __init__(self):
        self.status = "waiting" # Stati: 'waiting', 'countdown', 'playing', 'finished'
        self.tick = 0
        self.ball = Ball()
        # Creiamo i 4 slot fissi come richiesto dal frontend
        self.players = [
            Player(0, "left", "Player 1"),
            Player(1, "right", "Player 2"),
            Player(2, "top", "Player 3"),
            Player(3, "bottom", "Player 4")
        ]
        self.spectators = []
        self.connections: Set[WebSocket] = set()

    def update(self):
        if self.status == "playing":
            self.tick += 1
            self.ball.update()
            # Qui in futuro aggiungeremo: check_collisions()

    def check_collisions(self):
        ball = self.ball
        
        for p in self.players:
            if not p.connected:
                continue
                
            # 1. Calcoliamo i 4 margini (muri) del paddle corrente
            p_left = p.x
            p_right = p.x + p.width
            p_top = p.y
            p_bottom = p.y + p.height
            
            # 2. Verifica se la palla è nel rettangolo (AABB Collision)
            # Controlliamo se i margini della palla "pestano" lo spazio del paddle
            if (ball.x + ball.radius > p_left and   # Il fianco dx palla ha superato il margine sx paddle
                ball.x - ball.radius < p_right and  # Il fianco sx palla ha superato il margine dx paddle
                ball.y + ball.radius > p_top and    # Il fondo palla ha superato la cima del paddle
                ball.y - ball.radius < p_bottom):   # La cima palla ha superato il fondo del paddle
                
                # --- SE SIAMO QUI, C'È STATO UN IMPATTO ---

                # 3. Rimbalzo e Sblocco differenziato per lato
                if p.side == 'left':
                    ball.vx = abs(ball.vx) * 1.1      # Forza vx a essere positiva (va verso destra)
                    ball.x = p_right + ball.radius    # Teletrasporto fuori dal margine destro
                    
                elif p.side == 'right':
                    ball.vx = -abs(ball.vx) * 1.1     # Forza vx a essere negativa (va verso sinistra)
                    ball.x = p_left - ball.radius     # Teletrasporto fuori dal margine sinistro
                    
                elif p.side == 'top':
                    ball.vy = abs(ball.vy) * 1.1      # Forza vy a essere positiva (va verso il basso)
                    ball.y = p_bottom + ball.radius   # Teletrasporto fuori dal margine inferiore
                    
                elif p.side == 'bottom':
                    ball.vy = -abs(ball.vy) * 1.1     # Forza vy a essere negativa (va verso l'alto)
                    ball.y = p_top - ball.radius      # Teletrasporto fuori dal margine superiore

                # 4. Aggiungiamo un po' di fisica angolata (opzionale ma consigliato)
                # Questo evita che la palla faccia sempre le stesse traiettorie
                self.apply_angular_bounce(p, ball)

    def apply_angular_bounce(self, player, ball):
        # 1. Troviamo il centro del paddle e la posizione relativa dell'impatto
        if player.side in ['left', 'right']:
            # Paddle verticali: l'impatto dipende dalla coordinata Y
            paddle_center = player.y + (player.height / 2)
            # Calcoliamo la distanza tra il centro palla e centro paddle
            # Risultato: tra -0.5 (punta in alto) e 0.5 (punta in basso)
            relative_intersect_y = (ball.y - paddle_center) / player.height
            
            # 2. Modifichiamo la velocità verticale (vy)
            # Un moltiplicatore (es. 0.01) decide quanto è estremo l'angolo
            ball.vy += relative_intersect_y * 0.01

        else:
            # Paddle orizzontali (top/bottom): l'impatto dipende dalla X
            paddle_center = player.x + (player.width / 2)
            # Risultato: tra -0.5 (punta a sinistra) e 0.5 (punta a destra)
            relative_intersect_x = (ball.x - paddle_center) / player.width
            
            # 2. Modifichiamo la velocità orizzontale (vx)
            ball.vx += relative_intersect_x * 0.01

    def check_goal(self):
        ball = self.ball
        margin = self.ball.radius
        
        # Verifichiamo se la palla ha superato i limiti 0 o 1
        # con un piccolo margine di tolleranza
        if ball.x < -margin or ball.x > 1 + margin or ball.y < -margin or ball.y > 1 + margin:
            
            # Determiniamo chi ha perso il punto in base a dove è uscita
            if ball.x < 0:
                loser_side = 'left' # Il lato sinistro ha subito gol
            elif ball.x > 1:
                loser_side = 'right'
            elif ball.y < 0:
                loser_side = 'top'
            else:
                loser_side = 'bottom'

            # Assegnazione punti: nel Pong a 4, di solito chi subisce gol 
            # non prende punti, mentre gli altri sì.
            for p in self.players:
                if p.side != loser_side:
                    p.score += 1
            
            # Reset della palla (la riporta a 0.5, 0.5)
            ball.reset()
            
            # Opzionale: puoi mettere lo stato in 'countdown' per dare un attimo di pausa
            # self.status = 'countdown'
    
    def update_physics(self):
        # Muove la palla
        self.ball.update()
        
        # Controlla se tocca un paddle (include sblocco e angular bounce)
        self.check_collisions()
        
        # Controlla se è uscita (punti e reset)
        self.check_goal()
        
        # Muove i paddle (se hanno una velocità data dai tasti premuti)
        for player in self.players:
            player.move()  


    def get_payload(self):
        """Genera il pacchetto pronto per json.dumps()"""
        return {
            "type": "gameState", # Il 'type' che abbiamo concordato
            "payload": {
                "status": self.status,
                "tick": self.tick,
                "ball": self.ball.serialize(),
                "players": [p.serialize() for p in self.players],
                "spectators": self.spectators,
                "winner": None,
                "activePlayerCount": sum(1 for p in self.players if p.connected),
                "aiEnabled": False,
                "aiPlayerSlot": None,
                "lobby": {"playersReady": []}
            }
        }
# --- SERVER CORE   
game = PongGameState()

@asynccontextmanager
async def lifespan(app: FastAPI):
    task = asyncio.create_task(world_loop())
    yield
    task.cancel()

app = FastAPI(lifespan=lifespan)

async def world_loop():
    loop_time = 1 / 60
    while True:
        start = time.time()
        if game.status == "playing":
            game.update_physics()
        payload_json = json.dumps(game.get_payload())
        if game.connections:
            await asyncio.gather(*[ws.send_text(payload_json) for ws in list(game.connections)])
        
        wait = loop_time - (time.time() - start)
        await asyncio.sleep(max(0, wait))

@app.websocket("/ws")
async def websocket_endpoint(ws: WebSocket):
    await ws.accept()
    game.connections.add(ws)
    # Simuliamo assegnazione automatica allo slot libero per test
    current_player_idx = next((i for i, p in enumerate(game.players) if not p.connected), None)
    if current_player_idx is not None:
        game.players[current_player_idx].connected = True
    
    try:
        while True:
            data = await ws.receive_json()
            msg = ClientMessage(**data)
            if msg.type == "move":
                if current_player_idx is not None:
                    game.players[current_player_idx].move(msg.payload.direction)
    except (WebSocketDisconnect, Exception):
        if current_player_idx is not None: game.players[current_player_idx].connected = False
        game.connections.remove(ws)