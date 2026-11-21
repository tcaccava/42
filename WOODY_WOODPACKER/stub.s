_start:

    ; salvo tutti i registri caller saved volatili,compresi rcx e r11 che verranno sporcati dalla syscall
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

    ; scrivo in stdout chiamando write
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg]
    mov rdx, 14
    syscall


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
    lea rax, [rel _start]    ; indirizzo reale di _start (base + stub vaddr)
    mov r11, 0xAAAAAAAAAAAAAAAA ; placeholder int64_t(OEP - stub_vaddr)
    add rax, r11               ; rax = base + oep
    ;scambio il contenuto di rax con quello che e' in cima allo stack,cosi che rax riprende il suo valore originale
    ; e sullo stack ci finisce l'indirizzo OEP
    xchg rax, [rsp]

    ; ret fa un pop rsp dell'indirizzo in cima allo stack e ci salta dopo averlo messo in rip
    ; il programma vero parte,lo stack e' pulito e i registri intatti
    ret

msg: db "....WOODY....", 10; perche' sto definendo la label msg alla fine del sorgente e non all'inizio come 
; tradizionalmente si fa in assembly? perche' questo stub non verra compilato con nasm -f elf64,ottenendo cosi un eseguibile
; in cui le sezioni dati e codice eseguibile sono fisicamente separate nell'elf finale,ma con nasm -f bin,ottenendo un flat binary
; senza sezioni e metadati,un unico flusso di bytes scritti dall'alto in basso nello stesso ordine in cui nasm incontra il sorgente
; quando nel packer impostero' come nuovo OEP il vaddr dello stub,il kernel iniziera' a eseguire direttamente lo stub iniettato dal primo bytes
; se trovasse li' la label msg il kernel,non sapendo che deve interpretarla come caratteri ASCII di una stringa,proverebbe a passarli al frontend
; del decoder della cpu come mnemonici assembly,cioe' istruzioni,mandando tutto in SIGSEGV. Mettendo la label dopo ret invece,l'esecuzione partira' 
;dal byte 0 dello stub e proseguira' fino a ret,per poi saltare all'OEP originale. L'instruction pointer non puntera' mai ai bytes della label,che verranno 
; letti unicamente come dati tramite lo mnemonico lea