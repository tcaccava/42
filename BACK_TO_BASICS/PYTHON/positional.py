def mostra(a,b,c):
    print("a = ",a)
    print("b = ",b)
    print("c = ",c)

mostra(10,20,c=40)# vanno sempre prima tutti i posizionali,poi le keywords,mai il contrario

#  *args tupla di tutti gli argomenti posizionali
#  **kwargs dizionario di tutti gli argomenti con keywords

def mostra(*args,**kwargs):  
    print("Posizionali",args)
    print("Con nome", kwargs)

mostra(2,4,c=7)