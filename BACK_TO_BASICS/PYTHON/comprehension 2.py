#1) Hai una lista di tuple che rappresentano dei prodotti: (nome, prezzo, categoria).
prodotti = [("Laptop", 1200, "Elettronica"), ("Mela", 2, "Cibo"), ("TV", 800, "Elettronica")]
#Obiettivo: Crea una lista contenente solo i nomi dei prodotti che appartengono alla categoria "Elettronica".
elettronica = [a for a,b,c in prodotti if c == "Elettronica"]
print(elettronica)



#2) Hai una lista di stringhe che rappresentano dei log di server:
logs = ["INFO:Login:Utente1", "ERROR:Database:Connessione_Persa", "INFO:Logout:Utente1"]
#Obiettivo: Crea un dizionario dove la chiave è il secondo elemento (il componente, es: "Login") e il valore è il primo elemento (il livello, es: "INFO").
dizionario = {b : a for a,b,c in [string.split(':') for string in logs]}#oppure
dizionario = {b : a for s in logs for a, b, *resto in [s.split(':')]}
print(dizionario)



#3) Hai una lista di dizionari che rappresentano dei player:
players_data = [{"id": 0, "name": "Gino"}, {"id": 1, "name": "Pino"}]
#Obiettivo: Crea una nuova lista di dizionari dove a ogni player viene aggiunta la coppia "score": 0 senza modificare i dizionari originali.
new_list = [{**diz , "score" : 0} for diz in players_data]
print(new_list)


#4) Hai una frase: frase = "Il codice Python è potente"
#Obiettivo: Crea un set che contenga tutte le lettere uniche della frase, ma solo se sono minuscole e diverse dallo spazio.
str = "Il codice Python è potente"
new_set = {letter for letter in str if letter.islower() and letter != ' '}
print(new_set)


#5)Hai una funzione che calcola il prezzo finale:
def calcola_prezzo(base, tasse, sconto): return (base + tasse) - sconto
#E hai un dizionario con i dati: 
dati_prezzo = {"base": 100, "tasse": 22, "sconto": 10}
#Obiettivo: 1. Chiama la funzione usando lo scompattamento nominativo (**) del dizionario.
#2. Crea un generatore che raddoppi i valori del dizionario dati_prezzo (solo i numeri).

calcola_prezzo(**dati_prezzo)
gen = (2 * val for val in dati_prezzo.values())
for valore in gen:
    print(valore)

# Come "applichi" il generatore?
# Un generatore è "pigro": non fa nulla finché non glielo chiedi. Puoi applicarlo in tre modi:
# Ciclo for: for x in gen: print(x)
# Conversione: lista_risultati = list(gen)
# Consumatore: totale = sum(gen)



