global has_dup
global has_forbid_char
global invalid_length
global is_invalid_base
global is_char_in_base 
global return_base_value
global ft_atoi_base

extern ft_strlen


section .rodata              ; NASM non interpreta gli escape di C come \n o \t,quindi mentre la parte stampabile della stringa puo' essere definita
                             ; con la direttiva db define byte,i caratteri speciali per i white space vanno indicati con i loro valori ascii.

forbidden: db " +-", 9, 10, 11, 12, 13, 0  ; salvo nella memoria read-only la label della stringa che contiene i caratteri proibiti nella base
                             ; nei sistemi x86-64 moderni con PIE (Position Independent Executable) e ASLR, l'indirizzo assoluto della sezione .rodata non è noto a 
                             ; tempo di compilazione ed è variabile a runtime.
                             
                             ; un' alternativa e' l' uso degli apici inversi (backtick, ` `), con cui NASM abilita automaticamente l'escape dei caratteri esattamente 
                             ; come fa il compilatore C. Questo è molto più leggibile: forbidden db ` +-\t\n\v\f\r`, 0

whitespace : db " ", 9, 10, 11, 12, 13, 0   ; mi servira' per saltare i whitespace

section .text

; ==============================================================================
; HAS_DUP: cerco duplicati
; ==============================================================================
has_dup :
    xor eax, eax            ; pulisco il registro di ritorno
    xor r8, r8              ; contatore che mi serve per incrementare j a partire da i + 1 per scorrere tutta la sottostringa nel ciclo interno
.loop :                     ; loop esterno
    cmp byte [rdi], 0       ; verifico se il carattere corrente e' \0
    jz .end                 ; in tal caso esco dal loop e ritorno 0
    mov r8 , 1              ; j parte sempre da 1 rispetto al nuovo rdi[i + 1]
    jmp .int_loop           ; salto al loop interno 

.int_loop :
    mov dl ,byte[rdi]       ; carico str[i] in dl
    cmp byte[rdi + r8], 0   ; sono alla fine della stringa?
    jz .end_int_loop        ; si, ho raggiunto la fine della stringa rimanente
    cmp [rdi + r8], dl      ; comparo str[j] con str[i + j]
    jz .if_dup              ; ho trovato un duplicato, ritorno 1
    inc r8                  ; non ho trovato il duplicato, incremento j
    jmp .int_loop           ; ritorno nel loop interno

.if_dup :                   ; duplicato trovato
    mov eax, 1              ; ritorno 1
    ret

.end_int_loop :             ; ciclo interno terminato,str[i] NON e' nella stringa restante
    inc rdi                 ; passo a str[i++]
    jmp .loop               ; ritorno nel ciclo esterno, e r8 verra' resettato a 1

.end :                      ; sono arrivato alla fine di str nel ciclo esterno e non ho trovato alcun duplicato
    xor eax, eax            ; ritorno 0
    ret


; ==============================================================================
; HAS_FORBID_CHAR: Controllo caratteri vietati
; ==============================================================================
has_forbid_char :
    lea rsi, [rel forbidden]  ; rel calcola l'indirizzo della stringa readonly forbidden in modo PIE compliant
                              ; nei sistemi x86-64 moderni con PIE (Position Independent Executable) e ASLR, l'indirizzo assoluto della sezione .rodata 
                              ; non è noto a tempo di compilazione ed è variabile a runtime. Non posso fare mov rsi, forbidden perché l'indirizzo non è un valore 
                              ; immediato costante a 64 bit. Con [rel forbidden], la CPU calcola l'indirizzo al volo basandosi sul registro puntatore alle istruzioni:
                              ; Indirizzo Effettivo = RIP + Displacement . Questo consente di caricare il puntatore in un'unica istruzione senza richiedere rilocazioni assolute dal linker. 
                              ; lea rsi, [rel forbidden] non accede alla RAM, ma prende la formula dentro le quadre, ne calcola l'indirizzo finale (l'offset) e salva il puntatore risultante in rsi
    jmp .loop                 ; salto nel loop esterno
 
.loop :                     
    cmp byte [rdi], 0         ; confronto tra s[i] e \0
    jz .end                   ; sono a fine stringa,esco e ritorno 0
    xor eax, eax              ; uso rax come contatore per scorrere forbidden, resettandolo per ogni nuovo carattere della stringa
    jmp .int_loop             ; salto nel loop interno

.int_loop :
    cmp byte [rsi + rax], 0   ; comparo la s[i] con ogni carattere di forbidden
    jz .exit_int_loop         ; sono a fine forbidden,esco dal ciclo interno
    mov r8b, byte[rsi + rax]  ; leggo il carattere corrente di forbidden e lo sposto in r8b
    cmp [rdi], r8b            ; comparo s[i] con forbidden
    jz .forbidden_found       ; trovato un carattere vietato,ritorno 1
    inc rax                   ; vado al carattere successivo di forbidden
    jmp .int_loop             ; ricomincio il loop interno

.exit_int_loop:
    inc rdi                   ; ciclo interno finito,s[i] non e' forbidden,avanzo al carattere successivo della stringa
    jmp .loop

.forbidden_found :            ; carattere vietato trovato,ritorno 1
    mov rax, 1
    ret

.end :                        ; stringa validata,non ci sono caratteri vietati,ritorno 0
    xor eax, eax
    ret


; ==============================================================================
; INVALID_LENGTH: Ritorna 1 se len < 2, altrimenti 0
; ==============================================================================
invalid_length :
    sub rsp, 8                ; nel prologo sono gia a -8 per la call del chiamante,per chiamare ft_strlen dopo devo ripristinare l'allineamento a 16
    call ft_strlen            ; chiamo ft_strlen
    add rsp, 8                ; faccio + 8 e vado a -8,cosi' che alla fine il ret ripristini la stack simmetry col chiamante
    cmp rax, 2                ; comparo la lunghezza ottenuta con 2
    jae .valid                ; se la lunghezza e' >= 2 la stringa e' valida
    mov eax, 1                ; in caso contrario la sua lunghezza sara' 1 o 0
    ret                       ; ritorno 1,stringa invalida

.valid :                      ; stringa valida,ritorno 0
    xor eax, eax
    ret

; ==============================================================================
; IS_INVALID_BASE: raggruppa tutti i validity check in un' unica funzione
; ==============================================================================
is_invalid_base :             
    push rbx                  ; salvo rbx in stack ,e pushando eseguo un push che porta rsp a -8,che sommato al -8 fatto dal chiamante 
                              ; nel chiamare is_invalid_base,riporta rsp % 16 == 0,consentendomi di fare una call successiva in maniera 
                              ; ABI compliant
    mov rbx, rdi              ; rdi e' un registo volatile,che una call puo' sovrascrivere come vuole,quindi lo sposto in rbx,non volatile,perche'
                              ; mi servira' come argomento di 3 call
    call invalid_length       ; verifico che la stringa sia piu' lunga di un carattere
    test rax, rax             ; and di rax con se stesso che aggiorna il ZF
    jnz .invalid              ; se l'AND e' NON zero,allora significa che rax era 1,quindi lunghezza invalida,esco e ritorno 1
    mov rdi, rbx              ; sposto il puntatore a base in rdi,mi servira' come argomento della successiva call
    call has_forbid_char      ; verifico caratteri proibiti
    test rax, rax             ; testo rax
    jnz .invalid              ; se non zero,ci sono caratteri proibiti,ritorno 1 ed esco
    mov rdi,rbx               ; la call precedente potrebbe aver sovrascritto rdi,quindi ripusho il puntatore a base in rdi
    call has_dup              ; cerco duplicati
    test rax, rax             ; di nuovo test del return value
    jnz .invalid              ; se 1, ci sono duplicati,ritorno 1 ed esco
    xor eax, eax              ; la stringa e' valida
    pop rbx                   ; ripristino la stack simmetry e lo stato del registro rbx, quindi ritorno 0
    ret

.invalid :
    mov eax, 1                ; ritorno 1
    pop rbx                   ; ripristino stack simmetry
    ret                       


; ==============================================================================
; IS_CHAR_IN_BASE:  verifica che il carattere sia contenuto nella base data
; ==============================================================================

is_char_in_base : 

.loop :
    cmp byte [rsi], 0    ; verifico di non essere a fine stringa
    jz .not_in_base      ; se sono arrivato al \0 allora il char non e' nella base,ritorno 0
    cmp dil, byte [rsi]  ; comparo ogni singolo carattere della base con il carattere da validare
    jz .in_base          ; se e' nella base esco e ritorno 1
    inc rsi              ; incremento il pointer della base
    jmp .loop            ; continuo il loop

.in_base :               ; e' nella base,ritorno 1
    mov eax, 1
    ret

.not_in_base :           ; non e' nella base, ritorno 0
    xor eax,eax
    ret

; ==============================================================================
; RETURN_BASE_VALUE: ritorna il valore di un carattere nella base
; ==============================================================================

return_base_value :
    mov rcx, rdi         ; salvo il puntatore all'inizio della stringa in un registro di appoggio
                         ; fallthrough nel loop
.loop :
    cmp byte [rdi], 0    ; verifico se sono alla fine della base`
    jz .not_in_base      ; in tal caso il char non e' in base,quindi ritorno -1
    cmp sil, byte [rdi]  ; confronto un byte di base con il carattere da valutare
    jz .in_base          ; il carattere e' nella base,ritorno il suo valore`
    inc rdi              ; incremento base`
    jmp .loop            ; ricomincio il loop`

.in_base :               ; il carattere e' nella base`
    sub rdi, rcx         ; sottraggo il puntatore avanzato di base con il puntatore al suo inizio che ho salvato prima`
    mov rax, rdi         ; ritorno il risultato dell'aritmetica dei puntatori in rax
    ret

.not_in_base :           ; char non in base, ritorno -1 per indicare errore
    mov rax, -1
    ret


; ==============================================================================
; FT_ATOI_BASE: converte una stringa in un numero di una certa base
; ==============================================================================

ft_atoi_base :
    push rbx             ; pusho in stack i registri callee-saved prima di utilizzarli
    push r12
    push r13
    push r14
    push r15
    mov rbx, rdi         ; str == rbx
    mov r12, rsi         ; base == r12
    mov rdi, r12         ; carico base in rdi per la call successiva
    call ft_strlen       ; calcolo la base_len
    mov r15, rax         ; pusho baselen in r15
    mov rdi, r12         ; sposto base in rdi per la call successiva
    call is_invalid_base ; verifico che la base sia valida
    test eax, eax        ; & bitwise del return value con se stesso
    jnz .invalid_base    ; base invalida,esco e ritorno 0
    
    mov r13, 1           ; sign == r13
    xor r14, r14         ; res == r14          
    mov rcx, 0x100003E00 ; carico la bitmask a 32bit dei whitespace una sola volta fuori dal loop
                         ; la posizione di ogni bit acceso nella bitmask riflette il valore ascii dei whitespace 
                         ; che voglio saltare, quindi saranno accesi i bit 32,9,10,11,12,13
    jmp .whitespace_loop ; procedo a saltare i whitespace

.whitespace_loop :
    movzx rax, byte [rbx] ; carico il carattere corrente della stringa in rax con zero extension a 64bit
    cmp al , 32           ; lo confronto subito con 32, se siamo sopra non e' un whitespace e allora salto al sign check
    ja .sign_check        ; se above, salto al sign check
    bt rcx, rax           ; bit test estrae il bit di rcx all'indice rax e imposta il carry flag di conseguenza a seconda che sia 1 o 0
    jnc .sign_check       ; se il carryflag e' 0, allora NON e' un whitespace,quindi passo al sign check
    inc rbx               ; in caso contrario salto il whitespace, str++
    jmp .whitespace_loop  ; ricomincio il ciclo

.sign_check :
    cmp byte [rbx], 43    ; verifico che il carattere corrente sia +
    jz .is_plus
    cmp byte [rbx], 45    ; -
    jz .is_minus
    jmp .digits_build     ; che sia - o + ,passo alla costruzione del numero

.is_minus :               ; se - inverto il flag sign
    neg r13
.is_plus :                ; se + avanzo al carattere successivo e salto a digits_build
    inc rbx               

.digits_build:
    mov rdi, r12           ; carico base in rdi
    mov sil, byte [rbx]    ; carico il carattere della stringa in rsi
    call return_base_value ; calcolo il valore di quel carattere nella base,oppure -1 se non trovato
    cmp rax , 0            ; confront del return value con zero
    jl .done               ; la call ha restituito -1,abbiamo finito
    imul r14, r15          ; moltiplico res * base_len
    add r14, rax           ; aggiungo digit al risultato
    inc rbx                ; avanzo al carattere successivo
    jmp .digits_build      ; ritorno nel loop 

.done :
    imul r14 , r13       ; se arrivo qui significa che il carattere non fa parte della base,mi limito a ritornare il risultato
    mov rax, r14         ; sposto il risultato nel return register
    jmp .exit

.invalid_base :          ; base invalida,ritorno 0
    xor eax, eax
    jmp .exit

.exit :                  ; label di uscita in cui,in tutti i casi,ritorno il valore salvato in rax e ricompongo i registri callee-saved
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret    