list = [2,1,3,4] #lista,mutabile,si possono aggiungere o cambiare elementi
list.append(5)
list[0] = 7
print(list)# 7,1,3,4,5

# list.append(x) aggiunge in coda
# list.insert(i,x) inserisce x in posizione i
# l.pop() rimuove l'ultimo
# l.pop(i) rimuove e ritorna l'elemento all'indice i
# l.remove(x) rimuove la prima occorrenza di x  

tupla = (1,2,3,4)# tupla,immutabile,utile per dati costanti
#tupla[0] = 5 ERRORE
print(tupla[2])

# immutabile,niente append,pop,remove

#solo in liste e tuple si accede con un indice a partenza 0,in set e dizionari errore

set = {1,2,2,3}#set,non ordinato,mutabile,nessun duplicato,come i set di C
print(set)# 1,2,3
#print(set[2]) ERRORE
#set.add(x) aggiunge x se non c'e gia'
#set.remove(x) rimuove x se c'e',errore se non c'e'
#set.discard(x) rimuove x se c'e',non ritorna errore se non c'e'

#i dizionari,come le unordered map di c++, sono basati su hash map,cioe la chiave viene convertita da una funzione di hashing in un numero che viene usato come indice interno nella tabella del dizionario per accedere(accesso per chiave),anche se non corrisponde direttamente ad un indice di un array,e python gestisce automaticamente i casi di collisione dell'hash 
#il che permette di accedervi molto velocemente (accesso e inserimento/rimozione per chiave entrambi O(1),O sta per order of,cioe tempo costante,sempre lo stesso tempo indipendentemente dalla dimensione dei dati)
#in un array di C o una lista tradizionale devi conoscere la posizione per accedere ad ogni elemento(accesso per indice,complessita' O(1)) e se vuoi trovare un elemento per valore devi iterare
#su tutto l'array(inserire in fondo O(1),ricerca o inserire o rimuovere altrove tempo medio O(n),cioe lineare,proporzionale al numero di dati);nel dizionario accedi al valore tramite chiave significativa,non per posizione,non devi scorrere tutto
#le normali map di C++,a differenza delle unordered,sono ordinate per chiave e sono invece basate su alberi binari bilanciati,cioe' alberi binari che mantengono l'altezza minima possibile per il numero di nodi,quindi cio' garantisce che accesso,ricerca,inserimento e cancellazione siano tutti logaritmici,cioe O(log n), invece di peggiorare a O(n) se l'albero si "sgonfia" e diventa simile ad una lista
#inoltre il dizionario e' estremamente flessibile,perche la chiave puo essere una stringa,o un numero o una tupla ecc. ,e sono facilmente annidabili(dizionario dentro dizionario)
diz = {"nome": "tobia", "eta": 37, "citta" :"Roma"}#dizionario,mutabile,coppie chiave valore come le unordered map in C++
print(diz["nome"])#tobia
print(diz["citta"])#Roma
print(diz.keys())#nome,eta,citta
print(diz.values())#tobia,37,Roma
for chiave,valore in diz.items():
    print(f"Chiave : {chiave}, Valore : {valore}")
#print(diz[2]) ERRORE

# d.get(key,None) ritorna d[key] se esiste,None se non esiste
# d.pop(key) se d[key] non esiste crasha e solleva keyerror,se esiste ritorna il valore
# d.pop(key,None) se d[key] non esiste non crasha,ritorna none,se esiste ritorna il valore
# d.del(key) rimuove la chiave senza ritornare il valore
# d.keys() ritorna le chiavi
# d.values() ritorna i valori
# d.items() ritorna coppie chiave/valore

#una lista vuota si dichiara con [],una tupla con (),un set con set(),un dizionario con {}