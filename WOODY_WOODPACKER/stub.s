; salvo tutti i registri callee saved
push rax  ; salvo rax per prima,cosi restera' in fondo allo stack
push rdi
push rsi 
push rdx
push rcx
push r8
push r9
push r10
push r11
pushfq    ; salva sullo stack lo stato dei registri RFLAGS

; loop XOR, da fare,per adesso mi limito al dirottamento


; ripristino i registri allo stato originale
popfq
pop r11
pop r10
pop r9
pop r8
pop rcx
pop rdx
pop rsi
pop rdi
; per adesso non faccio pop rax,cosi' in cima allo stack rimane ancora il rax originale


; salto all' OEP(ninja jump)
; carico il placeholder a 64 bit per l'oep,sara' poi il packer C a cercare e sostituire questi bytes
mov rax, 0XAAAAAAAAAAAAAAAA

;scambio il contenuto di rax con quello che e' in cima allo stack,cosi che rax riprende il suo valore originale
; e sullo stack ci finisce l'indirizzo OEP
xchg rax, [rsp]

; ret fa un pop rsp dell'indirizzo in cima allo stack e ci salta dopo averlo messo in riprende
; il programma vero parte,lo stack e' pulito e i registri intatti
ret