def announcer(f): 
    def func():
        print("Prima")
        f()
        print("Dopo")
    return func

@announcer
def hello():
    print("Hello")

#equivale a questo:

# def hello():
#     print("Hello")

# hello = announcer(hello)

# cioe definisco la funzione hello,quindi chiamo announcer con argomento(f) la funzione hello
# announcer ritorna func ,quindi adesso hello punta a func,non piu alla hello originale,quindi
# se chiamo hello() sto in realta chiamando func,che al suo interno esegue la hello originale
# announcer e' il decoratore,hello la funzione decorata