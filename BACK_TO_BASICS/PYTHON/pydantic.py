# **Pydantic — Riassunto**

# Pydantic è una libreria Python per la validazione dei dati basata sui type hint. L'uso principale è definire modelli che descrivono la struttura attesa di un dato in entrata (tipicamente JSON) e validarla automaticamente.

# ---

# **BaseModel**

# Tutto parte da qui. Definisci una classe che eredita da `BaseModel` e dichiari i campi con i loro tipi:

# ```python
# class MovePayload(BaseModel):
#     direction: str
#     inputId: int
# ```

# Quando fai `MovePayload(**data)`, Pydantic valida automaticamente i tipi e lancia `ValidationError` se qualcosa non va.

# ---

# **Tipi base**

# ```python
# nome: str          # stringa
# eta: int           # intero
# altezza: float     # decimale
# attivo: bool       # True/False
# ```

# ---

# **Literal — valori esatti**

# Accetta solo i valori specificati, rifiuta tutto il resto:

# ```python
# from typing import Literal
# direzione: Literal["UP", "DOWN"]  # accetta solo queste due stringhe
# status: Literal["playing", "waiting", "finished"]
# ```

# ---

# **Union — più tipi accettati**

# ```python
# from typing import Union
# valore: Union[str, int]           # accetta stringa o intero
# direzione: Union[Literal["UP", "DOWN"], None]  # accetta UP, DOWN, o None
# ```

# ---

# **Optional — shortcut per Union con None**

# `Optional[X]` è esattamente uguale a `Union[X, None]`. Il campo deve essere presente ma può valere None:

# ```python
# from typing import Optional
# direzione: Optional[str]   # equivale a Union[str, None]
# ```

# ---

# **Valori di default**

# Con `= valore` il campo diventa opzionale — se non arriva nel JSON usa il default:

# ```python
# nome: str = "Anonymous"     # se non arriva, usa "Anonymous"
# direzione: Optional[str] = None  # se non arriva, usa None
# vsAI: bool = False          # se non arriva, usa False
# ```

# Senza default il campo è **obbligatorio** — se manca Pydantic lancia errore.

# ---

# **Field — validazione avanzata**

# Per aggiungere vincoli oltre al tipo:

# ```python
# from pydantic import Field
# nome: str = Field(..., min_length=1, max_length=20)  # ... significa obbligatorio
# punteggio: int = Field(default=0, ge=0, le=100)      # ge=greater equal, le=less equal
# ```

# ---

# **Tipi annidati**

# Puoi annidare modelli dentro modelli:

# ```python
# class Ball(BaseModel):
#     x: float
#     y: float

# class GameState(BaseModel):
#     ball: Ball
#     tick: int
# ```

# ---

# **List e Dict**

# ```python
# from typing import List, Dict
# giocatori: List[str]           # lista di stringhe
# punteggi: Dict[str, int]       # dizionario stringa → intero
# ```

# ---

# **Come si usa in pratica**

# ```python
# data = {"direction": "UP", "inputId": 5}
# msg = MovePayload(**data)   # valida e crea l'oggetto
# print(msg.direction)        # "UP"
# print(msg.inputId)          # 5

# # Se il dato è sbagliato:
# data = {"direction": "SIDEWAYS", "inputId": 5}
# msg = MovePayload(**data)   # lancia ValidationError
# ```

# ---

# **Con FastAPI**

# FastAPI usa Pydantic automaticamente — basta dichiarare il tipo del parametro e la validazione avviene senza scrivere codice aggiuntivo:

# ```python
# @app.post("/move")
# async def move(payload: MovePayload):
#     # se arriva qui, payload è già validato
#     print(payload.direction)
# ```