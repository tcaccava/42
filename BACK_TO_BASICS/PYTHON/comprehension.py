#COMPREHENSION

# List Comprehension [...]
# Scopo: Crea una nuova lista applicando un'espressione a ogni elemento di un iterabile.
# Sintassi: [espressione for elemento in iterabile if condizione]
# Caratteristica: Viene calcolata immediatamente (eager evaluation) e memorizzata per intero.
numeri = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
quadrati_pari = [x**2 for x in numeri if x % 2 == 0]#list comprehension

# Dict Comprehension {k: v}
# Scopo: Crea un nuovo dizionario definendo coppie chiave-valore.
# Sintassi: {chiave: valore for elemento in iterabile if condizione}
# Caratteristica: Utile per mappare oggetti o trasformare liste in tabelle di ricerca rapida.
parole = ["mela", "banana", "kiwi"]
dizionario = {p : len(p) for p in parole}#dict comprehension

# Set Comprehension {...}
# Scopo: Crea un nuovo set (insieme di elementi unici).
# Sintassi: {espressione for elemento in iterabile if condizione}
# Caratteristica: Rimuove automaticamente i duplicati e non garantisce l'ordine degli elementi.
testo = "abracadabra"
lettere = {char.upper() for char in testo}#set comprehension

# Generator Expression (...)
# Scopo: Crea un generatore che produce elementi uno alla volta su richiesta.
# Sintassi: (espressione for elemento in iterabile if condizione)
# Caratteristica: A differenza della list comprehension,non occupa memoria per l'intera sequenza (lazy evaluation). È ideale per calcoli su grandi masse di dati (es. passata direttamente a funzioni come sum() o any()).
prezzi = [10.0, 20.0, 50.0, 35.0, 45.0, 90.0]
totale_scontato = sum(0.9 * p for p in prezzi)#generator expression
# Una nota importante: I generatori sono "usa e getta"
# A differenza di una lista o di una tupla, una volta che hai iterato su un generatore, esso si svuota.

gen = (x for x in range(3))
for a in gen: print(a) # Stampa 0, 1, 2
for b in gen: print(b) # Non stampa nulla! Il generatore è esaurito.

# Le funzioni più usate con le generator expression:
# sum() — somma valori: sum(1 for p in players if p.connected)
# any() — True se almeno uno soddisfa: any(p.name == name for p in players)
# all() — True se tutti soddisfano: all(p.votedToAbandon for p in players)
# max() / min() — massimo/minimo: max(p.score for p in players)
# list() — materializza la generator expression in lista: list(p.name for p in players)
# next() — primo elemento che soddisfa: next((p for p in players if not p.connected), None)
# enumerate() tecnicamente non si usa con generator expression ma con for loop direttamente — ti dà indice e valore: for i, p in enumerate(players).

# La generator expression è più efficiente in memoria perché genera un elemento alla volta senza mai costruire la lista completa in memoria. Se hai un milione di elementi e vuoi solo fare sum() o any(), la generator non alloca mai tutto in memoria — processa un elemento, lo usa, lo scarta.
# La list comprehension invece costruisce tutta la lista in memoria prima di passarla alla funzione successiva.
# Però se hai bisogno di usare il risultato più di una volta, la list comprehension è più efficiente — la generator si esaurisce dopo la prima iterazione e non puoi riusarla, quindi dovresti ricrearla ogni volta.
# Quindi la regola pratica è: se consumi il risultato una volta sola usa la generator, se ti serve più volte usa la list comprehension.


#Nelle comprehension (List, Dict, Set), l'ordine dei for e degli if segue esattamente lo stesso ordine che useresti scrivendo dei cicli annidati classici.
#La regola è: Leggi da sinistra a destra, come se stessi indentando verso l'interno.

#UNPACKING

# 1.Unpacking Posizionale: L'operatore * (Splat)
# Si usa con tutti gli iterabili: list, tuple, set, dict (solo chiavi), string, generator.

# A. Con Liste e Tuple (Ordine Garantito)
# L'assegnazione segue l'indice esatto.

numeri = [10, 20, 30]
a, b, c = numeri  # a=10, b=20, c=30

#B. Con i Set (Ordine Arbitrario)
#Poiché i set non sono ordinati, l'unpacking estrarrà gli elementi nell'ordine in cui Python li sta ciclando internamente.

s = {1, 2, 3}
a, b, c = s  # Funziona, ma non sai se a sarà 1, 2 o 3.

# C. Con i Dizionari (Estrae le Chiavi)
# L'operatore * applicato a un dizionario estrae solo le sue chiavi.

d = {"x": 1, "y": 2}
a, b = d  # a="x", b="y"
print(*d) # Stampa: x y

# D. Extended Unpacking (Catturare il resto)
# Utile per dividere un iterabile in "testa" e "resto".

primo, *centro, ultimo = [1, 2, 3, 4, 5]
primo=1, centro=[2, 3, 4], ultimo=5


# 2. Unpacking Nominativo: L'operatore ** (Double Splat)
# Si usa esclusivamente con i Dizionari. Serve a estrarre le coppie chiave-valore.

# A. Nelle chiamate a funzione 
# Trasforma le chiavi del dizionario in argomenti della funzione.

def saluta(nome, messaggio):
    print(f"{messaggio}, {nome}")

dati = {"nome": "Marco", "messaggio": "Ciao"}
saluta(**dati) # Equivale a saluta(nome="Marco", messaggio="Ciao")

# B. Nel Merging (Unione)
# Crea un nuovo dizionario unendo i contenuti di altri.

dict_a = {"id": 1}
dict_b = {"punti": 100}
unito = {**dict_a, **dict_b, "stato": "attivo"}
# # {'id': 1, 'punti': 100, 'stato': 'attivo'}


# 3. Casi Particolari e "Tricks"
# Unpacking delle Stringhe
# Le stringhe sono iterabili di caratteri.

a, b, *resto = "Python"
a='P', b='y', resto=['t', 'h', 'o', 'n']

# Unpacking dei Generatori
# Quando usi next((...)), stai estraendo un singolo valore da un generatore. Se volessi estrarli tutti:

gen = (i**2 for i in range(3))
a, b, c = gen # a=0, b=1, c=4

# Unpacking e Comprehension possono lavorare assieme,per esempio puoi usare ** dentro una comprehension se stai manipolando dizionari complessi.
#Esempio: Hai una lista di dizionari e vuoi aggiungere a ognuno un campo "visto":

lista_punti = [{"x": 1}, {"x": 2}]
aggiornati = [{**punto, "visto": True} for punto in lista_punti]#dictionary merging + list comprehension

# Unpacking nelle Comprehension: {**punto, "visto": True}
# Questa è una tecnica chiamata Dictionary Merging. Non è una magia, ma un modo per "copiare e aggiungere" in un colpo solo.
# Ecco cosa succede riga per riga quando Python vede {**punto, "visto": True}:
# Crea un nuovo dizionario vuoto: { }.
# Scompatta punto: Prende tutte le coppie chiave-valore del dizionario punto e le "scarica" dentro il nuovo dizionario. Se punto era {"x": 10, "y": 20}, il nuovo dizionario diventa {"x": 10, "y": 20}.
# Aggiunge il resto: Python continua a leggere dopo la virgola e trova "visto": True. Lo aggiunge al nuovo dizionario.
# Risultato finale: {"x": 10, "y": 20, "visto": True}.

# Cosa succede in caso di duplicati?
# Se punto avesse già avuto una chiave "visto": False, l'ultima istruzione scritta ("visto": True) avrebbe sovrascritto il valore precedente. L'ordine conta: l'ultimo che arriva vince.

