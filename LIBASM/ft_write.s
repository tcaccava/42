global ft_write

extern __errno_location

section .text

ft_write:
    mov rax, 1     ; sposto in rax la syscall id della syscall che andro' subito dopo a richiamare,cioe' sys_write
    syscall        ; interrupt hw per andare in kernel mode, la syscall scrivera' il return value in rax
    cmp rax, -4095 ; comparo rax con il piu' piccolo(in complemento a due) dei return value riservati dal kernel agli errori delle syscall
    jae .error     ; se jae restituira' true,vorra' dire che il return value in rax e' (sempre in complemento a due) maggiore di -4095,per cui siamo sicuri che sia un errore
    ret            ; ritorniamo il controllo al chiamante, restituendo in rax un valore positivo in caso di successo della syscall o -1 in caso contrario 


.error :           ; il salto a questa label avviene esclusivamente se il return value della sys_write e' compreso tra -4095 e -1
    neg rax        ; inverto il valore negativo che la syscall ha salvato in rax,cosi' da convertirlo in positivo. In codice c equivarrebbe ad un -errno
    push rax       ; pusho in stack il valore positivizzato dell'errore, perche' nella riga successiva riutilizzero' rax e ho bisogno di salvarne il valore
    call __errno_location wrt ..plt   ; chiamo la funzione della libc che restituisce in rax l'indirizzo di memoria della variabile &errno(che e' un int*), indicando
                                      ; esplicitamente a NASM con wrt(With Respect To) e ..plt(Procedure Linkage Table) di indirizzare la call alla PLT
                                      ; Sara' poi la plt,gestita dal linker dinamico,a risolvere a runtime l'indirizzo effettivo della funzione __errno_location nella libc
                                      ; tutto questo e' necessario perche' nei sistemi moderni con eseguibili PIE le funzioni delle librerie dinamiche come libc.so vengono
                                      ; caricate ad indirizzi di memoria casuali via ASLR e non hanno un offset fisso noto in fase di assemblaggio/compilazione
                                      ; senza wrt ..plt NASM eseguirebbe una chiamata diretta ad un offset a 32 bit relativo che ld non sara' in grado di risolvere,restituendo un
                                      ; relocation error
    pop rdx           ; recupero il valore di ritorno positivizzato dalla stack e lo piazzo in rdx
    mov [rax],  edx   ; essendo errno un int a 32 bit, leggo dal sottoregistro edx di rdx e ne piazzo il valore alla memoria puntata da rax,cioe' quella della variabile errno
    mov rax, -1       ; adesso posso impostare rax a -1
    ret               ; ritorno al chiamante con -1 di return value e errno impostata correttamente


; In C una funzione di sistema in caso di errore ritorna -1 e poi imposta la variabile globale errno del thread
; il comportamento in Assembly e' completamente diverso: la syscall richiamata restituisce in rax un valore di ritornero
; negativo compreso tra -1 e -4095(ad es. -9 Bad File Descriptor o EBADF). Per implementare ft_write in modo conforme allo standard POSIX
; una volta chiamata la syscall dobbiamo innanzitutto verificare che rax sia negativo
; la verifica si potrebbe fare usando cmp rax, 0 e poi richiamando js(Jump if Sign),cioe' una funzione che esegue un jump ad una label se il Sign flag
; e' impostato a 1,ovvero se il registro usato per la comparazione contiene un numero negativo
; oppure si potrebbe ancora piu' elegantemente usare test rax,rax che esegue un AND bitwise tra due registri e aggiorna gli RFLAGS,per cui il SF risulta 1
; se l'AND bitwise degli MSB dei due registri e' 1,cioe' se sono entrambi 1,quindi negativi.Rispetto al cmp richiede meno byte di opcode
; La trappola dei due approcci precedenti D' che il solo controllo del segno con js puo' essere rischioso,perche' alcune syscall come mmap ritornano indirizzi
; di memoria che,se interpretati in complemento a due come interi signed a 64 bit, potrebbero avere l'MSB a 1 risultando negativi pur essendo indirizzi perfettamente
; validi. Per questo motivo il kernel garantisce che gli unici e soli codici di errore reali siano compresi tra -1 e -4095(in hex unsigned da 0xFFFFFFFFFFFFF001 a 
; 0xFFFFFFFFFFFFFFFF).Ecco perche' il modo veramente sicuro,usato anche dalla libc,per vericare il ritorno della syscall e' fare un cmp di rax con -4095, che impostera' 
; gli RFLAGS,e poi richiamando la funzione jae(Jump if Above/ Equal). La valutazione dell'istruzione cmp rax , -4095 seguita da jae oepra infatti trattando i dati in 
; modalita' unsigned in complemento a due. -4095 in unsigned equivale al numero 0xFFFFFFFFFFFFF001,e l'intervallo di codici di errore spazia da lui a -1,cioe' 0xFFFFFFFFFFFFFFFF
; Jae esegue un confronto unsigned,cioe' la condizione di salto e' che rax >= 0xFFFFFFFFFFFFF001. Un qualsiasi valore di ritorno positivo salvato in rax,per es. 1,cioe 0x000...1
; dara' un FALSE,quindi gli unici valori che rispetteranno quella condizione dando TRUE saranno quelli superiori a 0xFFFFFFFFFFFFF001,cioe' in signed proprio quelli che vanno fino a -4095





; quando il main di test chima la funzione ft_write,che a sua volta fa internamente una syscall a sys_write
; il main in quanto caller si limita a preparare soltanto gli argomenti della funzione secondo la Function Calling ABI
; quindi nel caso specifico carichera' l'fd in rdi, il buf in rsi,e il count in rdx
; quando la mia ft_write chiama la syscall di cui fa da wrapper interfacciandosi con il kernel, deve a sua volta
; rispettare l'Syscall Kernel ABI garantendo che quegli argomenti si trovino caricati negli stessi identici registri(questo perche' 
; per le syscall i primi 3 registri di carico degli arg sono identici a quelli delle funzioni userspace)
; per cui non ci sara' bisogno di ricaricare gli argomenti,ma potro' limitarmi a caricare l'id della syscall sys_write
; in rax,e poi a chiamare la syscall
; il problema si porrebbe nel caso la syscall accettasse anche un quarto arg: in quel caso,visto che il registro di carico del
; quarto arg delle syscall e' r10, diverso da quello delle funzioni esterne rcx,e quindi le due ABI(Function Calling ABI vs Kernel Syscall ABi)
; divergono,in quel caso syscall distruggerebbe il contenuto di rcx,che usa per salvare l'instruction pointer di rientro,e sarei quindi 
; obbligato(per esempio con mmap che ha 4+ argomenti), a spostare il 4 arg da rcx a r10(mov r10, rcx) prima di lanciare syscall

; come viene garantito l'alllineamento della stack? In primo luogo il chiamante,il main.c,deve rispettare l'ABI e garantire che 
; al momento di ingresso nella funzione l'RSP punti ad un multiplo di 16. Al momento della call di ft_write, la stessa call del chiamante spinge
; l'indirizzo di rientro sulla stack,per cui rsp risulta sfasata(a sinistra) di 8. Ma se la chiamata ha successo ret togliera' quegli 8 byte dell'indirizzo di rientro
; dalla stack(spostandosi a destra) ripristinando la stessa allo stato di ingresso. Piu' complesso invece e' il caso in cui si vada nel blocco di errore: in quel caso entriamo sfasati di 8 
; a sinistra proprio a causa della call, poi push rax sottrae altri 8 byte a rsp,e ritorniamo allineati a 16 a sinistra. Dopo c'e' la call __errno_location,una funzione esterna che puo' essere 
; eseguita proprio perche' rsp e' allineato a 16 byte. Segue una pop rdx che aggiunge 8 byte a rsp,quindi ritorniamo sfasati di soli 8 a sinistra, e poi la ret finale, che rimuovendo l'indirizzo di 
; rientro del caller, sposta di altri 8 a destra e ritorniamo perfettamente allineati allo stato del chiamante.
; Quindi in sostanza se la call di ft_write ha successo rsp ritorna automaticamente al valore originale grazie alla simmetria tra la stessa call e ret, mentre se ritorna un errore
; l'ABI verra' comunque rispettata perche' la chiamata a errno_location avviene con rsp allineata ad un multiplo di 16 grazie alla push rax, mentre il pop rdx e il ret successivi garantiscono il
; ritorno all'indirizzo originale del main. In sostanza pieno rispetto dell'ABI in tutti i flussi di esecuzione.