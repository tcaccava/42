; dati inizializzati
; per quanto le etichette possano essere scritte anche senza i : , e' piu' elegante farle seguire da essi
; nasm sapra' che quella e' una label,cioe' basicamente un indirizzo a cui saltare
; le labels e quindi i : si usano per etichettare entry point,funzioni e bersagli per i salti(es. loop_inizio:)
; nasm e' in grado di parsare il sorgente indipendentemente dalle indentazioni,ma di solito si usa la convenzione
; di non usare indentazioni per le label e di usarla(1 tab o 4 spazi) invece per le mnemoniche e le direttive come global e estern.
; msg e msg_len per esempio sono due label, cioe' placeholder testuali che nasm sostituira' con l'indirizzo di memoria fisico in cui inizia quella sequenza di byte 
; db sta per Define Byte,non e' uno mnemonico,ma una direttiva per l'assembler che gli dice di scrivere gli esatti bytes(in questo caso in formato ascii,
; ma potevano essere forniti anche in esadecimale) nel file binario di output,separati dalla virgola,a botte di un singolo byte alla volta
; nasm legge la stringa "hello" e la converte nei bytes ascii corrispondenti
; il 10 dopo la virgola e' il valore ascii della \n, che db sbatte in ram subito dopo la o di hello
; perche' non scrivere direttamente db "hello\n"? perche' nasm leggerebbe \ e n come due bytes ascii distinti,allocando 7 bytes
; esistono anche dw(define word),che scrive a blocchi di 2 byte,dd(dualword) a 4 byte,e dq(quadword) a 8 bytes.
; equ sta per Equals,e' anche lui una direttiva che dice a nasm di calcolare l'offset tra il byte corrente $ e l'inizio della stringa msg a compile time e di definirla come una costante
; in sostanza e' una sorta di #define in C
; $ e' una variabile speciale di nasm che contiene l'indirizzo di memoria corrente a cui l'assemblatore e' arrivato a compilare
; Assembly e' un linguaggio imperativo,letto e eseguito dall'alto in basso e da sx a dx,quindi nasm prima incontra msg,di cui salva l'indirizzo
; poi esegue db e scrive 6 byte,spostando l'indirizzo corrente $ di 6 bytes,ed ecco perche' la sottrazione matematica da proprio la lunghezza della stringa

section .rodata

msg: db "hello", 10 
msg_len: equ $ - msg 


; global e' un metacomando(una relocation entry) per il linker ld che esporta la label _start affinché il linker sappia da dove iniziare
; start non e' una funzione richiamabile con call,ma l'entry point del file elf
; il kernel ci consegna lo stack point RSP immacolato(ci sono solo argc,argv e le env variables) e quindi noi per stampare hello e uscire non abbiamo nemmeno bisogno di toccarlo

global _start 


; in C quando facciamo return 0 dal main viene eseguita l'istruzione ret
; il main e' stato chiamato dalla _start della c runtime crt1.o,che cattura lo 0 e invoca sys_exit 
; in questo esercizio non c'e la glibc,non c'e' crt1,lo start lo scrivo io,quindi nella stack non c'e' alcun return adress salvato a cui tornare con ret
; definendo a mano _start, il kernel mi consegna RSP puntato direttamente sui dati grezzi del processo (argc, argv, envp). Non esiste un indirizzo di ritorno in memoria: se esegui ret,
; la CPU legge argc credendolo un puntatore e va in Segmentation Fault. La sys_exit è l'unico modo per distruggere il processo.
; senza sys_exit la cpu avanzerebbe RIP al byte successivo alla syscall write e finirebbe per fetchare spazzatura che non appartiene al .text == SEGFAULT
; quindi invocare esplicitamente sys_exit e' il solo modo di dire al kernel di killare il processo in modo pulito

; se invece definissi un main, la C Runtime (crt1.o) eseguirebbe una call main. In cima allo stack ([RSP]) c'è il return address e la CRT provvede ad allineare RSP a 16 byte come richiesto dall'ABI.
; quando eseguo ret in main, restituisco il controllo alla CRT passando l'exit code in RAX. Sarà poi la CRT stessa a gestire il teardown del processo (inclusi il flush dei buffer I/O della libc, i distruttori e 
; il rilascio delle risorse) prima di invocare sys_exit. La C Runtime non ha un chiamante all'interno dello spazio utente, non è un programma autonomo ed è fusa direttamente nel tuo binario. 
; la C Runtime è un insieme di file oggetto (crt1.o, crti.o, crtn.o) scritti per fare da "bootstrap". In fase di linking, gcc incolla fisicamente questo codice dentro il tuo file eseguibile finale. Fa parte dello stesso processo della tua applicazione.
; fa parte della C Standard Library del sistema (su Linux solitamente la glibc, o la musl) integrata con la toolchain del compilatore (gcc/binutils)
; in che linguaggio è scritto la C runtime? L'entry point _start è rigorosamente scritto in Assembly. In quel momento la CPU non ha uno stack frame valido per una funzione C; _start legge i parametri grezzi lasciati dal kernel su RSP (argc, argv), allinea lo stack a 16 byte per rispettare l'ABI e prepara i registri.
; una volta sistemati i registri di basso livello, l'Assembly di _start fa una call alla funzione C __libc_start_main. Quest'ultima (scritta in C) si occupa di inizializzare la memoria, il Thread Local Storage (TLS), eseguire i costruttori globali (sezione .init_array), e infine chiamare il tuo main(argc, argv, envp).
; Quindi la C runtime e' scritta in un misto di C e Assembly.

; il "chiamante" concettuale della C runtime è il Kernel Linux
; quando eseguo un binario, la syscall execve legge l'header ELF dell'eseguibile, mappa i segmenti in RAM, imposta lo stack e inserisce il valore del campo e_entry dell'header ELF direttamente nel registro RIP della CPU.
; il kernel passa così da Ring 0 (Kernel Mode) a Ring 3 (User Mode) saltando al primo byte dell'etichetta _start contenuta nel file oggetto crt1.o.
; se il binario usa librerie dinamiche, il kernel salta prima al linker dinamico ld.so, che carica le dipendenze, e poi salta a _start.

section .text
    
_start:
    ; prepariamo i registri per la syscall sys_write(id = 1)
    mov rax, 1    ; metto l'id della syscall in rax
    mov rdi, 1    ;primo arg di write e' l'fd di stdout,cioe' 1
    mov rsi, msg  ;secondo arg di write il pointer all'inizio della stringa
    mov rdx, msg_len ; terzo arg la lunghezza della stringa
    syscall          ; interrupt hardware: passiamo in kernel mode(ring 0)


    ; prepariamo i registri per sys_exit(id = 60)
    mov rax , 60   ;metto in rax l'id di exit
    mov rdi , 0    ; primo e unico argomento di exit, zero,cioe' successo
    syscall        ; il kernel esegue exit,uccide il processo e ne libera la memoria



 ; assembliamo il file con il flag -f elf64: dice a NASM il Formato di output. 
 ; esistono decine di formati binari (macho64 per macOS, win64 per Windows). 
 ; con elf64, NASM genera un file oggetto (.o) che rispetta lo standard ELF a 64-bit,l'unico che il linker e il kernel di Linux sanno leggere.  
 ; -o: sta per output: indica il nome da dare al file generato.