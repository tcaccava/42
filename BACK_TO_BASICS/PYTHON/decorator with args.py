def decorator(f):
    def func(*args, **kwargs):
        print("Prima")
        result = f(*args,**kwargs)
        print("Dopo")
        return result
    return func

@decorator
def hello(name,surname):
    print(f"Hello {name} {surname}")

hello("Tobia", surname="caccavale")
hello("Gino",surname="Pasticcino")
hello(name="Peeno",surname="Tucci")

#usando nella func interna al decoratore(cd wrapper) args e kwargs come argomenti
#rendo il decoratore universale,cioe' gli permetto di funzionare su qualsiasi funzione
#indipendentemente dal numero di argomenti che ha,ma questo non cambia i vincoli della funzione
#hello a runtime,che per esempio in questo caso deve avere al massimo due argomenti

#puoi personalizzare un decoratore passandogli un argomento
def decorator_with_args(prefix):  # <--- questo è il nuovo argomento
    def actual_decorator(f):       # <--- questo è il "decoratore vero"
        def wrapper(*args, **kwargs):
            print(f"{prefix} Prima")
            result = f(*args, **kwargs)
            print(f"{prefix} Dopo")
            return result
        return wrapper
    return actual_decorator

@decorator_with_args(">>>")   # <--- passiamo un argomento al decoratore
def hello(name):
    print(f"Hello {name}")

hello("Tobia")
#quindi python,appena legge @decorator_with_args(">>>"),la esegue subito,e gli ritorna actual decorator
# quindi esegue actual decorator(hello) e ritorna wrapper,quindi assegnazione finale hello = wrapper,questo prima ancora
# che hello("Tobia") venga chiamato;quando questo succede viene eseguito wrapper,che a sua volta richiama la hello originale

#seguiamo il flusso:
#1)viene eseguito il decoratore esterno che ritorna actual decorator,quindi actualdecorator = decoratorwithargs(">>>")
#2)decorazione vera,viene eseguito actual decorator su hello,e ritorna wrapper,quindi hello = actualdecorator(hello),in questo momento hello == wrapper
#3)runtime hello("Tobia"),che in realta esegue wrapper("Tobia"),che internamente fa f("Tobia"),dove f e' hello originale
#4)i primi due punti si possono riassumere cosi hello = decoratorwithargs(prefix)(hello)



#altro esempio di decoratore con argomento

def repeat(n):
    def decorator(f):
        def wrapper(*args,**kwargs):
            for _ in range(n):
                result = f(*args,**kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def hello(name):
    print(f"Hello {name}")

hello("Tobia")


#ultimo esempio di decoratore con argomento

def log(label):
    def dec(f):
        def wrapper(*args,**kwargs):
            print(f"[{label}] START")
            f(*args,**kwargs)
            print(f"[{label}] END")
        return wrapper
    return dec

@log("WS")
def ws():
    print("gestione websocket")

ws()

