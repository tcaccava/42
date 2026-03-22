def mostra(a,b):
    print("a =",a)
    print("b =",b)

mostra(b=10,a=20)# a = 20,b = 10

def printa(name, surname=""):#il "" di surname ci dice che l'argomento con keyword e' opzionale,cioe' se non c'e usa al suo posto la stringa vuota
    print(f"{name} {surname}")

printa("Tobia",surname="Caccavale")
printa("Tobia")
# printa(surname="Caccavale") non va perche,a differenza di surname,name non e' opzionale,ma necessario

def hello(name: str, age: int = 18):
    print(f"{name} ha {age} anni")

hello("Tobia")        #  usa il default 18
hello("Tobia", 25)    #  passa un posizionale
hello(name="Tobia")   #  usa il default
hello(name="Tobia", age=30)  #  keyword

#stesso esempio di prima ma con un intero predefinito opzionale

#in condizioni normali la scelta tra posizionale e keyword e' a runtime,cioe' ,una volta definita
#la funzione,e' quando la chiami passandole degli argomenti che scegli tra posizionale e keyword,avendo solo 
#la cura di passare i posizionali prima dei keyword. Una alternativa e' usare l'asterisco come argomento,che sostanzialmente
#dice alla funzione che,se tutti gli argomenti precedenti possono essere posizionali o keyword,i successivi devono essere solo keyword

def hello(name: str, *, age: int):
    print(f"{name} ha {age} anni")

hello("Tobia", age=20)  #  corretto
hello("Tobia", 20)       #  errore, age deve essere keyword

def f(a, b=2, *, c, d=4):
    print(a, b, c, d)

# Possibili chiamate corrette:
f(1, c=10)          # b usa default 2, d default 4
f(1, 3, c=10)       # b = 3
f(1, c=10, d=5)     # override d

# Errate:
f(a=1, b=3, c=10)     #  posizionale dopo keyword non va
f(1, 2, 3, 4)       #  c e d devono essere keyword-only
