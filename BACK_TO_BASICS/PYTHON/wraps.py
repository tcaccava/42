import inspect
from functools import wraps
def decorator(f):
    def wrapper(*args,**kwargs):
        return f(*args,**kwargs)
    return wrapper

@decorator
def sum(a,b):
    """Somma due numeri"""
    return a + b

print(sum.__name__)
print(sum.__doc__)
print(inspect.signature(sum))

#senza wraps stampa rispettivamente wrapper,None,(*args,**kwargs)
#usando il decoratore python ha perso nome,docstring e firma originali della funzione,che sono diventati quelli del suo wrapper
#perche ricordiamo che quel decoratore equivale a fare sum = decorator(sum),e decorator(sum) ritorna wrapper,quindi sum == wrapper

def decorator(f):
    @wraps(f)
    def wrapper(*args,**kwargs):
        return f(*args,**kwargs)
    return wrapper

print(sum.__name__)#sum
print(sum.__doc__)#"Somma di due numeri"
print(inspect.signature(sum))#(a,b)

#identita' ripristinata,wraps e' un decoratore per il wrapper che copia i parametri di f prima che siano cambiati