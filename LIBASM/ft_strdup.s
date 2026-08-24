global ft_strdup

extern malloc
extern ft_strlen

segment .text

ft_strdup :
    ; all'ingresso della funzione RSP NON e' allineato a 16, perche' la call del chiamante ha fatto push rdi e quindi RSP e' a -8
    push rdi     ; salvo il puntatore all'inizio della stringa nella stack, RSP - 8,quindi va a -16 ritornando allineato e consentendo la call a ft_strlen
                 ; rdi inoltre e' un registro caller-saved,quindi volatile,che le successive chiamate a ft_strlen e malloc potrebbero sovrascrivere 
    call ft_strlen wrt ..plt ; chiamo ft_strlen sullo stesso argomento che il caller ha piazzato in rdi,cioe' il pointer alla stringa
    inc rax                   ; incremento il risultato di strlen di 1 per il \0 della stringa che andro' a malloccare
    mov rdi, rax              ; sposto la lunghezza della stringa duplicata in rdi, che usero' come argomento di malloc
    call malloc wrt ..plt     ; chiamo malloc,che avra' come argomento la lunghezza della stringa da creare
                              ; in caso di fallimento, malloc imposta già internamente errno a ENOMEM, quindi non serve intercettare errno con __errno_location
    cmp rax , 0               ; verifico se rax e' 0, in tal caso malloc ha ritornato NULL e l'allocazione e' fallita
                              ; una alternativa al cmp e' test rax, rax, che fa l'and di rax con se stesso, che da 0 se rax e' 0/NULL, impostando il ZF a 1
                              ; mentre cmp richiede 7byte di opcode perche' esegue una sottrazione tra dst e src, test occupa solo 3 bytes di opcode
    jz .error                 ; in caso di fail dell'allocazione con malloc ritorno un puntatore NULL
    mov r8 , rax              ; salvo in r8 il puntatore all'inizio della stringa duplicata restituito da malloc,che ritornero' alla fine 
    mov r9, [rsp]             ; sposto in r9 il puntatore sorgente
                              ; perche' ho bisogno di dereferenziare lo stack pointer? all'ingresso della funzione con push rdi ho pushato l'indirizzo a 64bit contenuto in rdi
                              ; non SU rsp,ma alla locazione puntata da RSP, infatti push rdi equivale a sub rsp 8 e poi mov [rsp], rdi, cioe' sposta il puntatore rsp contenuto nel registro rsp
                              ; a sinistra di 8 e poi scrive il valore di rdi nella ram all'indirizzo contenuto in rsp. Facendo [RSP] va a leggere quegli 8 bytes memorizzati all'indirizzo di rsp
    xor rcx, rcx              ; azzero rcx, mi servira' come indice per spostare il puntatore
    jmp .loop                 ; entro nel loop di copia

.error :
    pop rdi                   ; se malloc fallisce con il ret devo poter ritornare al chiamante,cioe' ft_strdup,ma per farlo ho bisogno che l'indirizzo di memoria del ritorno siano salvati nello stack
                              ; in modo che l'istruzione ret, eseguendo implicitamente un pop rip,possa poi spostare quell'indirizzo di rientro del chiamante
                              ; quindi pop rdi serve a rimuovere dalla cima della stack l'indirizzo della stringa,che ho pushato in stack all'inizio di ft_strdup
                              ; rimosso quello,nella pila ci sara' subito sotto l'indirizzo di ritorno a ft_strdup,che la call di malloc ha pop in rip al momento della call
    xor rax, rax              ; azzero rax per ritornare NULL
    ret                       ; ritorno NULL

.loop :               
    mov r11b, byte [r9 + rcx] ; sposto il byte corrente di src in un registro di appoggio
    mov byte [r8 + rcx] , r11b     ; sposto dal registro di appoggio a quello di destinazione
    inc rcx                   ; incremento l'accumulatore
    cmp  r11b, 0              ; solo dopo la copia comparo il bytes scritto in dst con il null terminator
                              ; perche' eseguo il cmp dopo inc? perche' inc modifica i RFLAGS che anche cmp modifica,quindi eseguirlo dopo cmp significherebbe corromperli
                              ; anche qui si poteva usare test r11b, r11b
    jnz .loop                 ; se lo ZF e' diverso da 0,il carattere non e' il null terminator e rientro nel loop 
    pop rdi                   ; tutto il restante blocco di codice viene eseguito solo se il compare da' 0,quindi ZF 1,e allora siamo a fine stringa
    mov rax, r8               ; sposto l'indirizzo di partenza della stringa dup su rax
    ret                       ; ritorno il pointer a dup

; un' alternativa piu' semplice ma meno istruttiva sarebbe sostituire l'intero loop con una call a ft_strcpy
; ricostruiamo il percorso di rsp e l'allineamento della stack dall'inizio alla fine della funzione
; il main,cioe' il chiamante, garantisce che prima di invocare ft_strdup,lo stack sia allocato e allineato,cioe RSP % 16 == 0
; la call di ft_strdup fa internamente un push sulla stack dell'indirizzo di ritorno,quindi andiamo a -8
; il push rdi per salvare il puntatore della stringa da duplicare fa un altro -8,quindi ritorniamo allineati a 16
; solo perche' siamo allineati a 16 possiamo chiamare malloc senza evocare segfault
; call malloc fa implicitamente un altro push sulla stack dell'indirizzo di ritorno,un altro -8,quindi -24
; malloc internamente fara' la sua ret,quindi un pop rip,quindi un +8 e ritorniamo a -16,quindi allineati 
; ritorniamo a ft_strdup con rsp allineato rsp % 16 == 0. Il pop rdi,eseguito o in caso di fallimento di malloc o a fine funzione,
; fa un +8,quindi andiamo a -8 e ritorniamo disallineati, ma il ret finale,che ritorni il puntatore NULL o quella alla stringa duplicata,
; fa un altro pop rip,quindi +8 dell'indirizzo di ritorno che rimuove dalla stack E che piazza in rip,e torniamo a 0. In sostanza in tutto il flusso di 
; esecuzione sono pienamente rispettati i due vincoli ABI,e cioe' che prima di ogni call esterna rsp % 16 == 0,cioe' 16 byte Alignment Rule,
; e che quando la funzione ritorna al caller,il main,eseguendo ret,sia ripristinata la Stack Frame Simmetry (Cleanup),cioe' RSP sia riportato esattamente
; al valore originario che aveva prima della call della funzione.