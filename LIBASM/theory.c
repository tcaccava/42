/*
---DEFINIZIONE DI ASSEMBLY--------------------------------------------------------------------------
L'Assembly è una rappresentazione simbolica intellegibile dall'uomo del codice macchina ,cioe' di quella lunga sequenza 
di bytes binari ,detti opcode, che rappresentano istruzioni che la cpu e' in grado di eseguire direttamente.
L'assemblatore NASM e' responsabile della traduzione del codice Assembly in machine code. Altri assemblatori sono GAS (GNU Assembler,
che usa sintassi AT&T di default), MASM (Microsoft Macro Assembler) o FASM.
Il NASM (Netwide Assembler),l'assemblatore per la sintassi Intel, e' il programma che prende il testo Assembly (es. mov rax, 1) e lo traduce 
negli opcode binari (es. B8 01 00 00 00). Questo binario è proprio il codice macchina ISA (Instruction Set Architecture).

---SORGENTE ASSEMBLY--------------------------------------------------------------------------------
Il sorgente Assembly ha formato .s, dove s sta per Source: e' l'estensione storica per i file assembly scritti a mano, in contrapposizione 
ai .S (maiuscolo) che passano prima dal preprocessore C. Il file source contiene diverse sezioni:
- .text: contiene il codice eseguibile (read-only, executable),cioe' e' la sezione fisica dove risiedono i mnemonici.
   Il text,e il binario da esso compilato, vengono caricati in ram con permessi restrittivi,RX(Read, Execute), niente scrittura.
- .data: contiene variabili globali inizializzate (es. int x = 5).
- .bss (Block Started by Symbol): contiene variabili globali non inizializzate o azzerate (es. int y;).
- .rodata (Read-Only Data): contiene costanti, come le stringhe letterali (es. "Hello\n"), RO(READ only).
- direttiva global <simbolo>: esporta l'etichetta affinché sia visibile dal linker C . Per es. global ft_strlen dice 
  all'assemblatore di scrivere questo indirizzo in una tabella di esportazione(Symbol Table) nel file .o.Quindi global serve in sostanza a cambiare lo scope di una label
  nella symbol table del file oggetto rendendola visibile all'esterno del file oggetto per il linker ld. Di default invece qualsiasi label in Assembly e' locale,cioe' privata al file in cui e' definita.
  Quindi per esempio se ho un unico sorgente .s con dentro un main e una funzione ft_strlen,non avro' bisogno di dichiarare la funzione come global perche' NASM risolve la chiamata call ft_strlen del main
  internamente a compile_time. In questo contesto solo global main sara' obbligatoria,affinche' la c runtime possa trovare il main. Allo stesso modo se non uso gcc e la C runtime ma creo un Assembly puro linkato
  con ld, definiro' la global _start,ma non per la C runtime,che senza gcc non esiste nel mio programma,ma per il linker ld che la usera' per scrivere l'entry point nell'header del file ELF,in modo che il kernel
  sappia da quale indirizzo far partire la cpu al momento della esecuzione. Anche qui ft_strlen non dovra' essere global se viene chiamata solo da _start nello stesso file sorgente.
  Quando il compilatore C compila il main.c, trova una chiamata a ft_strlen ma non ha il codice della funzione. La direttiva extern serve per far dire al file che quel codice arrivera' da "fuori"(extern). 
  Alla fine, il Linker (ld) prende tutti i file .o, collega le etichette esportate dal .s con la chiamata pendente nel .c e genera l'eseguibile.
- direttiva extern <simbolo>: dichiarazione di un simbolo fornito esternamente, come malloc o il puntatore di sistema a errno. 
  Extern in sostanza lascia uno spazio vuoto(Relocation Entry) segnalando il nome della funzione esterna da cercare. Il Linker incrociera' le due direttive e
  riempira' quel buco ricalcolando gli offset.
Quindi in sostanza global e extern sono due direttive gemelle che servono agli antipodi della relazione di importazione-esportazione: global e' usato da chi esporta,cioe' da chi definisce il codice da esportare,
per metterlo a disposizione degli altri file oggetto,mentre extern si usa nel file che importa un simbolo definito in un altro file oggetto.

ESEMPIO : Se scrivo un sorgente strlen.s e un altro main.s separati,dovro nel primo dichiarare strlen come global e nel secondo importare strlen come extern perche' la possa usare.
in fase di build nasm produce un oggetto ft_strlen.o dove ft_strlen è contrassegnata come GLOBAL, e un oggetto main.o dove la call ft_strlen fa riferimento a un simbolo EXTERN. Compilando con gcc main.o strlen.o il linker 
prende i due file .o, legge il simbolo ft_strlen esportato da strlen.o e lo inserisce nel punto in cui main.o aveva lasciato la chiamata call in sospeso.

---COMPILAZIONE, C E ASSEMBLY----------------------------------------------------------------------- 
Il flusso completo di un programma C è questo:
1. Preprocessore C (cpp): legge i file .c e risolve tutte le direttive che iniziano per # (#include, #define). 
   I file .S (maiuscola) passano per questa fase (puoi usare #define in Assembly), i file .s (minuscola) no.
2. Compilatore (cc1): legge il C e genera codice Assembly intermedio. Perché passa per l'Assembly? Perché il compilatore 
   lavora su astrazioni: generare codice Assembly permette al compilatore di ottimizzare il flusso logico 
   del programma prima di tradurlo in crudo binario.
3. Assemblatore (as o nasm): prende l'Assembly e genera i file oggetto (.o). I file .o contengono codice macchina puro, 
   ma mancano gli indirizzi di memoria di qualsiasi simbolo(funzioni o variabili globali) che non sia definito in quel file sorgente(es. printf).
   L'assembler non crea il binario riga per riga in un singolo passaggio,ma,essendo un Multi-pass Assembler, dapprima (Pass 1) legge tutto il sorgente dall'alto in basso senza tradurre 
   alcuna istruzione,ma solo costruendo una Symbol Table in cui annota la posizione di ogni etichetta e la sua distanza(offset) dall'inizio della sessione(per es.
   .loop +2 byte di offset). Soltanto nel Pass 2 successivo traduce i mnemonici in opcode binari,usando la table costruita prima per saltare agli indirizzi interni esatti.
4. Linker (ld): fonde assieme C e Assembly; prende tutti i file oggetto .o e le librerie esterne, risolve gli indirizzi vuoti e impacchetta tutto nel file 
   eseguibile finale (ELF su Linux). Il file a.out (Assembler OUTput) e' in formato ELF(Executable and Linkable Format).
A differenza di C, dove il compilatore gestisce l'allocazione delle variabili locali e lo stack frame, in Assembly si ha il controllo diretto sui registri 
della CPU e sugli indirizzi di memoria. 

Chi gestisce l'avvio del processo?
In cado di un programma Freestanding ,cioe' che definisce l'etichetta global _start, il linker (ld) per default cerca il simbolo _start per impostare l'header ELF dell'eseguibile (l'Entry Point Address). 
Quando il kernel esegue l'ELF, trasferisce il controllo direttamente a quell'indirizzo. Siccome non c'è la libc, il simbolo main per la CPU e per il kernel semplicemente non esiste e non serve.
Se invece si tratta di un programma ospitato da C,cioe' che definisce un etichetta global main, quando compilo o linko tramite gcc, il compilatore include silenziosamente il codice di avvio della libreria standard C (crt1.o). 
Quel file contiene già la sua etichetta _start, che si occupa di allineare lo stack, preparare argc, argv, envp e l'ambiente di runtime. Alla fine di questo setup, la CRT esegue call main. In questo contesto non c'e' bisogno di definire la label _start,ma 
devo definire la global main per permettere al _start della CRT di trovare ed eseguire la funzione.

Il comando gcc non è un singolo eseguibile monolito, ma un wrapper frontend(un "orchestratore" della toolchain) che analizza l'estensione dei file di input e decide quali binari sottostanti chiamare:
Se gli passo un file .c, invoca in sequenza:
1. Il compilatore C vero e proprio (cc1) --> genera Assembly (.s)
2. L'assembler (gas, GNU assembler) --> genera codice macchina (.o)
3. Il linker (ld o collect2) --> genera l'eseguibile ELF finale.
Gcc invoca Gas,cioe' l'assembler di sistema standard per sistemi Unix-like,ma non e' in grado di invocare NASM. GAS infatti si aspetta codice scritto con direttive GNU e che rispetti la verbosa sintassi AT&T(per esempio .global invece di global, .text invece di 
section .text,registri preceduti dal prefisso %,costanti con $,ordine degli operandi invertito,cioe' src ,dst).
Se gli passo direttamente un file .o, gcc capisce che il codice macchina è già stato generato, salta i primi due passaggi e invoca unicamente il linker ld).

Se provassi a linkare manualmente un main.o scritto in C o che usa la libc chiamando direttamente ld: ld main.o -o program,
ld fallirebbe immediatamente. Questo accade perché non saprebbe dove trovare i file di avvio della C Runtime (crt1.o, crti.o, crtn.o) e le librerie di sistema (libc.so), restituendo un errore di undefined reference to _start.
Gcc trasforma internamente quella chiamata isolando la complessità del sistema operativo e invocando ld con questa lista reale di parametri (si puo' vedere aggiungendo il flag -v, cioè verbose):
ld /usr/lib/x86_64-linux-gnu/crt1.o \
   /usr/lib/x86_64-linux-gnu/crti.o \
   main.o \
   -lc \
   /usr/lib/x86_64-linux-gnu/crtn.o \
   -o program

In sintesi gcc non compila gli opcode. Riconosce che main.o è già un ELF Relocatable contiguo di codice macchina e lo passa direttamente a ld, aggiungendo in automatico le librerie di runtime necessarie per far girare il main.
Mi risparmia in sostanza lo sforzo di dover scrivere a mano l'entrypoint _start,perche' fa il link con il C runtime(crt1.o) che a sua volta fornisce l'entrypoint _start,inizializza il processo e chiama il main.
Quindi ha senso scrivere _start (Freestanding / Pure Assembly) per:
- capire lo stack iniziale del Kernel: quando il kernel avvia l'ELF, non c'è nessuna funzione chiamante. Lo stack contiene direttamente argc su [rsp], i puntatori argv a seguire e le variabili d'ambiente envp.
- zero dipendenze / size optimization: nessuna dipendenza da LibC o librerie esterne. Ottengo un binario minuscolo. È l'approccio necessario per bootloader, sviluppare un sistema operativo, embedded bare-metal o scrittura di shellcode.
- controllo assoluto: non c'è codice "nascosto" eseguito prima o dopo le mie istruzioni.

Ha senso usare invece la C Runtime / gcc (main) per:
- allineamento dello stack: l'ABI impone che RSP sia allineato a 16 byte prima di eseguire qualsiasi call che usi istruzioni SIMD/vectorial (es. SSE, AVX). Il kernel consegna RSP non allineato per la chiamata di funzioni C: 
  la CRT si occupa di allinearlo prima di saltare a main. Se chiamo funzioni C o printf da un _start scritto male, rischio un Segfault generato da istruzioni come movaps.
- inizializzazione del runtime: la CRT gestisce la configurazione del Thread Local Storage (TLS), la registrazione dei distruttori e l'esecuzione delle sezioni .init e .fini.
- interoperabilità: se l'obiettivo è scrivere librerie di funzioni in Assembly da linkare in progetti C, la mia funzione sarà una callee (invocata dal C). L'entry point _start è irrilevante: l'unica cosa che conta è rispettare 
  rigorosamente la convenzione di chiamata System V (registri per gli argomenti, registri da preservare, valore di ritorno in RAX).

In C quando eseguo un eseguibile ELF, la C Runtime (crt1.o) esegue una call main. In cima allo stack ([RSP]) c'è il return address e la CRT provvede ad allineare RSP a 16 byte come richiesto dall'ABI.
Quando faccio return 0 nel main viene eseguita l'istruzione ret in main, che restituisce il controllo alla CRT passando l'exit code in RAX. Sarà poi la CRT stessa a gestire il teardown del processo (inclusi il flush dei buffer I/O della libc, i distruttori e 
il rilascio delle risorse) prima di invocare sys_exit. Il main e' stato chiamato dalla _start della C runtime crt1.o,che cattura lo 0 e invoca sys_exit.  
La C Runtime non ha un chiamante all'interno dello spazio utente, non è un programma autonomo ed è fusa direttamente nel binario.Il suo "chiamante" concettuale è il Kernel Linux.
Quando eseguo un binario, la syscall execve legge l'header ELF dell'eseguibile, mappa i segmenti in RAM, imposta lo stack e inserisce il valore del campo e_entry dell'header ELF direttamente nel registro RIP della CPU.Il kernel passa così da Ring 0 (Kernel Mode) 
a Ring 3 (User Mode) saltando al primo byte dell'etichetta _start contenuta nel file oggetto crt1.o.Se il binario usa librerie dinamiche, il kernel salta prima al linker dinamico ld.so, che carica le dipendenze, e poi salta a _start.
La C Runtime è un insieme di file oggetto (crt1.o, crti.o, crtn.o) scritti per fare da "bootstrap". In fase di linking, gcc incolla fisicamente questo codice dentro il tuo file eseguibile finale. Fa parte dello stesso processo della applicazione.
Fa parte della C Standard Library del sistema (su Linux solitamente la glibc, o la musl) integrata con la toolchain del compilatore (gcc/binutils). In che linguaggio è scritto la C runtime? L'entry point _start è rigorosamente scritto in Assembly. 
In quel momento la CPU non ha uno stack frame valido per una funzione C; _start legge i parametri grezzi lasciati dal kernel su RSP (argc, argv), allinea lo stack a 16 byte per rispettare l'ABI e prepara i registri.
Una volta sistemati i registri di basso livello, l'Assembly di _start fa una call alla funzione C __libc_start_main. Quest'ultima (scritta in C) si occupa di inizializzare la memoria, il Thread Local Storage (TLS), eseguire i costruttori globali (sezione .init_array),
e infine chiamare il main(argc, argv, envp). Quindi la C runtime e' scritta in un misto di C e Assembly.
Crt1.o e' un file oggetto unico, statico e pre-compilato. Risiede nel sistema operativo in /usr/lib/x86_64-linux-gnu/crt1.o ed è stato compilato una volta sola quando sono stati installati i pacchetti di sviluppo della glibc.
Non cambia mai in base al codice che scrivo.

---MNEMONICI ASSEMBLY E RAPPORTO CON IL MACHINE CODE ISA--------------------------------------------   
Esiste una corrispondenza quasi 1:1 tra un'istruzione Assembly e la rispettiva istruzione binaria. Quasi perche' Assembly è un'astrazione debole.
Esistono circa un migliaio di mnemonici Assembly, ma le codifiche ISA binarie sottostanti sono decine di migliaia. Un mnemonico (dal greco, "che 
aiuta la memoria") è una parola in inglese leggibile dall'uomo che raggruppa un concetto operativo. Esempi di mnemonici sono MOV, ADD, PUSH, XOR. 
Nel linguaggio Assembly x86-64 esistono circa un migliaio di mnemonici in totale (includendo le estensioni SIMD/AVX). Ma la CPU (il silicio) non 
sa cosa sia un mnemonico, accetta solo byte binari, definiti dall'ISA. L'ISA x86-64 non ha una singola istruzione hardware per il concetto di 
"muovi i dati" (MOV), ma decine di istruzioni hardware completamente diverse a seconda di cosa stai muovendo e dove lo stai muovendo.
Uno stesso mnemonico pue' essere tradotto in decine di opcode diversi. Per es. con mov rax, rbx (copia da un registro 64-bit a un registro 64-bit), 
NASM la traduce in un opcode specifico(48 89 D8); con mov rax, 5 (copia un numero immediato/costante in un registro),invece, NASM usa una codifica 
binaria completamente diversa (Immediate encoding), traducendo nell'opcode 48 C7 C0 05 00 00 00.Con mov eax, ebx (copia da registro 32-bit a registro 32-bit)
NASM traduce in 89 D8; mov [rax], rbx (copia un registro nella RAM puntata da un altro registro) NASM traduce in 48 89 18.
In sostanza Assembly da l'illusione di semplicita' attraverso l'utilizzo di un migliaio di mnemonici, ma dietro le quinte si nasconde una complessita' hw
impressionante (il manuale ISA della Intel è lungo quasi 5000 pagine ed esistono decine di migliaia di combinazioni binarie valide nell'ISA).
Inoltre, NASM implementa delle "pseudo-istruzioni" a vantaggio del programmatore, che vengono espanse in più istruzioni macchina o convertite (ad esempio per caricare 
variabili sfruttando il RIP-relative addressing in modalità PIE). 

---STRUTTURA DEL MNEMONICO ASSEMBLY-----------------------------------------------------------------
A differenza della sintassi AT&T, la sintassi Intel compilata tramite NASM esprime sempre le istruzioni nella forma:
--Istruzione  --Destinazione,--Sorgente
Le parentesi quadre [...] indicano un accesso alla memoria puntata(dereferenziazione) dall'indirizzo contenuto all'interno:
ESEMPIO: mov rax, rbx ---> copia il valore del registro RBX nel registro RAX.
         mov rax, [rbx] ---> legge 8 byte dalla memoria situata all'indirizzo contenuto in RBX e li scrive in RAX.

In C se fai *ptr = 5, il compilatore sa di quanti byte è il puntatore (char* o int*), in genere 64 bit su architetture a 64 bit.
In Assembly, le parentesi quadre [...] servono per accedere al valore della memoria contenuta in un registro o in ram(tramite puntatore),ma per farlo la CPU deve sapere esattamente 
quanti byte leggere,il cosiddetto operatore di dimensione. Per esempio se faccio mov [rax], 5, la CPU ha in mano solo un indirizzo di memoria: non sa se scriverci il 5 in formato 1 byte (0x05), 
2 byte (0x0005), o 4 byte (0x00000005). Ecco perché va specificato esplicitamente: mov byte [rax], 5 oppure mov dword [rax], 5 .
Se la dimensione dell'operazione è ambigua (es. quando si lavora con costanti immediate), occorre specificare l'operatore di 
dimensione,cioe' i qualificatori di ampiezza della memoria manipolata:
-byte (1 byte / 8 bit) == char
-word (2 byte / 16 bit) == short
-dword (4 byte / 32 bit) == int 
-qword (8 byte / 64 bit) == long

---LISTA DEI MNEMONICI PIU' UTILIZZATI--------------------------------------------
-cmp (Compare): sintassi cmp dest, src .Esegue una sottrazione logica interna tra due operandi(dest - src) senza salvare il risultato ne' sovrascrivere il valore di dest,
 aggiornando unicamente i flag di stato nel registro RFLAGS. Se dest == src, lo ZF sara' 1,se diversi sara' 0,se la differenza negativa il Sign Flag(SF) sara' impostato a 1. 
-je / jz (Jump if Equal / Jump if Zero): sintassi je/jz label. Mnemonici di salto condizionale che compilano nello stesso identico opcode binario e che ispezionano esclusivamente lo ZF: se 1(cioe' 
 se il confronto precedente ha dato zero trovando i due valori ==), la cpu carica l'indirizzo di label dentro il RIP effettuando il salto. In caso contrario la direttiva viene completamente
 ignorata e la cpu passa alla istruzione successiva.
-jne / jnz (Jump if Not Equal): esatto opposto di je/jz.Mnemonici di salto condizionale che eseguono il salto se lo Zero Flag è 0. 
-jmp (Unconditional Jump): sintassi jmp label. Esegue un salto incondizionato verso un'etichetta,imponendo alla cpu di prendere l'indirizzo della label e metterlo in RIP, ignorando completamente gli RFLAGS.
-inc (Increment): sintassi inc reg oppure inc byte [ptr] . Incrementa di 1 il contenuto del registro o della cella di memoria passato come operando (utilizzabile per far avanzare un indice o un puntatore o un accumulatore come rax).
 Aggiorna lo ZF(se l'incremento porta il registro a zero per via di un wraparound/overflow) e lo SF,ma preserva immutato il Carry Flag(CF).
-xor (Exclusive OR): sintassi xor dst, src. Applicato su un registro con se stesso, è lo standard per azzerare un registro. Genera un opcode più compatto(2 bytes), cancella i 32 bit superiori dell'estensione a 64 bit e rompe 
 le dipendenze hardware nella pipeline della CPU. Infatti i circuiti interni di register renaming della CPU riconoscono xor reg, reg come un Dependency Breaker(un idioma speciale di azzeramento): dicono alla pipeline che il valore precedente 
 del registro può essere scartato subito.La cpu non eseguira' mai lo xor logico nelle ALU,ma riconoscera' direttamente l'operazione nella fase di decode,azzerando il registro. Questo rompe le dipendenze dalle istruzioni precedenti della pipeline
 e permette l'esecuzione Out-of-Order istantanea senza attendere che RAX venga liberato da altre unità d'esecuzione.
-loop (deprecata) : esiste solo per motivi di retrocompatibilita' legacy con li 8086,ma e' lenta,forza l'uso di RCX come contatore implicito ed e' mal ottimizzata per l'attuale pipeline out-of-order e superscalare delle cpu attuali. Ormai totalmente inutilizzata.
-mov dst, [src]: move sposta un quantitativo di dati che dipende esclusivamente dalla dimensione del registro che stai usando come operando. Per es. mov rax, rbx sposta 8 byte (64 bit), mov eax, ebx sposta 4 byte (32 bit), mov al, bl sposta 1 byte (8 bit).
 mov con consente lo spostamento memory to memory mov [dst], [src],perche' fisicamente non esiste un bus diretto da ram a ram che non passi dalla cpu. Il dato devo obbligatoriamente fare un viaggio di andata e ritorno passando per la cpu: quindi dalla ram al processore
 usando un registro temporaneo di appoggio,e poi da quel registro di nuovo alla ram. I due registri tra cui mov muove dati,cioe' la dimensione dei due operandi,deve essere identica,altrimenti l'assemblatore restituisce un errore. Se uno degli operandi si trova in memoria,
 e quindi va dereferenziato con [], la sua dimensione andra' dichiarata esplicitamente affinche' corrisponda esattamente alla dimensione del registro con cui sta interagendo.
-sub (Subtract): sintassi sub dst, src. Esegue l'operazione dst = dst - src e aggiorna i flag come ZF e SF in base al risultato dell'operazione
-cmpsb (Compare String Byte) : sintassi cmpsb. Confronta il byte puntato da RSI con quello puntato da RDI, eseguendo dietro le quinte una sottrazione logica [rsi] - [rdi], e aggiorna i registri RFLAGS compatibilmente col risultato.
 Inoltre incrementa o decrementa automaticamente rsi e rdi(a seconda dello stato del Direction Flag,che di solito e' impostato in avanti). Spesso usata con il prefisso repe o repne(Repeat while Equal / not equal).
-movsx e movzx(Move with Sign Extension/ Zero Extension) : sintassi movsx dest_grande, src_piccola. Entrambe queste mnemoniche servono a risolvere un' problema fisico dei calcolatori: spostare dati da un contenitore piu' piccolo ad uno piu' grande senza 
 alterare il valore del dato. Quindi queste estensioni correggono il principale limite di mov,ovvero la necessita' di usarla su registri src e dst con eguale dimensione di memoria. Movzx estende il dato riempendo tutti i bit superiori mancanti con zeri; si usa tipicamente 
 per i tipi unsigned. Es. se dl contiene 0xFF(255) movzx eax,dl rendera' eax 0x000000FF. Movsx invece estende il dato preservando il segno(complemento a due),quindi guarda l'msb del registro sorgente e lo copia in tutti i bit superiori mancanti del registro di destinazione.
 E' usato per i tipi signed.Es. dl contiene 0xFF,bit di segno = 1,quindi in signed corrisponde a -1, allora movsx eax, dl rende eax 0xFFFFFFFF, che corrisponde in 32 bit signed a -1.
-neg (Negate) : sintassi neg registro. Calcola il complemento a due dell'operando,invertendone il segno algebrico. Non usa registri esterni,modifica direttamente il registro in place. Ad esempio se il registro contiene -1(0xFFFFFFFFFFFFFFFF),il suo valore diventera' 1,cioe' 
 0x0000000000000001. Aggiorna gli RFLAGS,impostando il Carry flag a 1 se l'operando e' diverso da 0,lo ZF a 1 se l'operando era 0.
-test : sintassi test registro,registro. Esegue un AND bitwise tra due operandi,ma scarta il risultato e aggiorna unicamente gli RFLAGS. Quindi a differenza di and dst, src  ,non sovrascrive il risultato dell'operazione in dst, garantendo che i registri coinvolti
 non vengano modificati.

 ---SINTASSI ASSEMBLY--------------------------------------------------------------------------------
A livello di Assemblatore (NASM), i mnemonici delle istruzioni e i nomi dei registri sono rigorosamente case-insensitive (insensibili alle maiuscole/minuscole).
Da un punto di vista strettamente tecnico, scrivere MOV RAX, 5, mov rax, 5, o perfino una mostruosità come MoV rAx, 5 produce esattamente lo stesso risultato. 
L'assemblatore li interpreta tutti come la stessa astrazione e genera l'identico opcode binario hardware (48 C7 C0 05 00 00 00). Tuttavia se si consulta il 
mastodontico Intel 64 and IA-32 Architectures Software Developer's Manual (la "bibbia" hardware della CPU), ogni singolo mnemonico e' stampato in maiuscolo.
Questa è un'eredità diretta dell'informatica degli anni '70 e '80: i primi terminali (Teletype), i vecchi mainframe e i primi assemblatori commerciali (
come il MASM della Microsoft per MS-DOS) usavano il maiuscolo come standard, spesso perché i set di caratteri originari non supportavano le minuscole o lo facevano male.
Al tempo stesso, oggi, specialmente su sistemi Linux, macOS e nei progetti accademici lo standard assoluto è scrivere mnemonici e registri in minuscolo.
Il motivo è l'influenza egemonica del linguaggio C e di UNIX. C è un linguaggio strettamente case-sensitive in cui le parole chiave sono tutte minuscole. 
Quando gli sviluppatori UNIX hanno iniziato a scrivere codice misto C/Assembly (usando toolchain come GNU GCC e l'assemblatore GAS), la convenzione del minuscolo è stata portata
nel mondo Assembly per pura coerenza visiva ed estetica del codice sorgente.
Esiste pero' un' eccezione critica: le etichette e i simboli. Se è vero che NASM ignora il case per le istruzioni hardware (mnemonici) e per l'hardware interno (registri), 
le etichette (label), i nomi delle variabili e le chiamate di funzione sono strettamente CASE-SENSITIVE. Per chiamare malloc, si deve scrivere call malloc. Se scrivo call MALLOC, 
NASM lo compilerà, ma  il Linker (ld) ritornera' un errore di tipo `Undefined reference to MALLOC`, perché per il Linker (che segue le regole del C) malloc e MALLOC sono due stringhe 
testuali distinte che puntano a indirizzi di memoria potenzialmente diversi.
Lo stesso vale per le direttive di esportazione: con global ft_strlen, il C potrà chiamare ft_strlen(),ma global FT_STRLEN generera' un errore.

---PIE E FLAG NO-PIE--------------------------------------------------------------------------------
Per motivi di sicurezza i sistemi operativi caricano i programmi in memoria ad indirizzi casuali ogni volta che vengono eseguiti(ASLR - Address Space Layout Randomization). 
Per fare questo, il codice deve essere compilato come Position Independent Executable (PIE). Un programma PIE viene caricato in RAM ad un indirizzo base casuale ad ogni esecuzione.
Non potendo conoscere gli indirizzi assoluti in anticipo, come fa il programma ad accedere all'indirizzo di memoria di funzioni esterne come la funzione malloc della libreria del 
sistema operativo? Tramite la PLT (Procedure Linkage Table), una sezione del .text, e la GOT (Global Offset Table), una sezione di .data). Sono tabelle create dinamicamente: 
la call malloc in realtà non chiama subito la vera malloc, ma accede ad un piccolo stub (un mini-blocco di 3 istruzioni assembly nella PLT) tramite il quale legge l'indirizzo reale 
di malloc ,calcolato al momento del lancio del programma (runtime), dalla GOT e ci salta(jump). Al primo caricamento del programma, il Dynamic Linker del sistema operativo (es. ld.so) 
calcola dove si trova malloc e scrive l'indirizzo reale nella GOT. Dalla seconda volta in poi,il dato e' in cache e l'accesso istantaneo.
Il flag `-no-pie` vieta questo comportamento, forzando indirizzi statici vecchi di 20 anni. 
 
---PIPELINE DI ESECUZIONE DI UNA CPU----------------------------------------------------------------
La CPU lavora eseguendo un ciclo continuo:
1)Fetch: il backend preleva l'istruzione corrente dalla memoria ram all'indirizzo contenuto nel registro RIP (Instruction Pointer).
2)Decode: decodifica l'istruzione hardware dividendola in micro-operazioni.
3)Execute: esegue l'operazione nell'ALU o nell'FPU o nell'MMU o nell'NPU.
4)Increment: aggiorna RIP affinché punti all'istruzione successiva.

La CPU preleva gli opcodes dalla RAM, ma più specificamente dalla L1i-Cache (Instruction Cache) che specchia porzioni della RAM.  La CPU ha una Cache L1d (Dati) e una L1i (Istruzioni),e non 
legge la RAM byte per byte :il controller di memoria trasferisce blocchi da 64 byte alla volta (chiamati Cache Line) dalla RAM alla L1i. La CPU poi fa il fetch degli opcode direttamente dalla L1i.
In x86-64, le istruzioni hanno lunghezza variabile; per es. ret è 1 byte (0xC3), ma altre istruzioni possono arrivare fino a 15 byte. La CPU decodifica e capisce da sola quanti byte deve consumare 
prima di incrementare RIP. Gli indirizzi che usiamo in C(i puntatori) sono logici (o virtuali),un' astrazione. Un programma crede di avere la RAM tutta per sé,ma la MMU usa una tabella del Kernel 
(Page Table) per tradurre istantaneamente l'indirizzo logico 0x4000 in un indirizzo fisico (il vero condensatore sul banco di RAM installato sulla scheda madre).

La decodifica avviene in hw nel backend della cpu: circuiti dedicati prendono l'opcode binario e lo scompongono in micro-operazioni (µops). 
A seconda dell'istruzione, l'hardware smista la µop all'unità competente per la sua esecuzione(execution units):
-ALU (Arithmetic Logic Unit) per matematica intera e bitwise.
-FPU / SIMD (Floating Point / Vector Unit) per virgola mobile o operazioni su vettori. L'FPU (Floating Point Unit) calcola numeri con la virgola mobile classici 
 (scalari, uno alla volta). SIMD (Single Instruction Multiple Data) usa registri vettoriali enormi (es. XMM a 128 bit, YMM a 256 bit, ZMM a 512 bit). Un vettore e' sinonimo di array hardware. 
 In un registro da 256 bit posso entrare fino a otto float da 32 bit e si puo' sommarli tutti in parallelo a un altro registro in un singolo ciclo di clock con una singola istruzione macchina.
 La FPU classica x87 è tecnicamente deprecata su architetture x86-64 moderne. Oggi i float scalari (singoli) non si calcolano più nella FPU, ma nei registri XMM della SIMD ignorando i restanti bit vettoriali.
-AGU (Address Generation Unit) per calcolare indirizzi di memoria fisici prima di accedervi, lavorando con la MMU(Memory Managment Unit).
 Se scrivo [rbx + rcx*4 + 8], l'AGU esegue la moltiplicazione e le somme via hardware per trovare l'indirizzo da passare alla MMU,che a sua volta,interrogando la sua
 cache interna(TLB) e la Page Table del Kernel,trovera' i condensatori esatti sul banco fisico di ram installato nella mobo.

---REGISTRI DELLA CPU-------------------------------------------------------------------------------
I registri sono celle di memoria ad altissima velocità,estremamente piu' veloci della gia' velocissima ram, situate direttamente all'
interno della CPU. Nella architettura x86-64 sono presenti 16 Registri di uso generale (GPR - General Purpose Registers) a 64 bit, 
a cui si aggiungono registri speciali di stato e di controllo( es. CR3).
I 16 registri a 64 bit risalgono ai primordi della informatica: i primi 8 (RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP) sono estensioni dei 
registri a 32 bit di Intel; gli altri 8 (R8-R15) sono stati aggiunti da Amd con x86-64.
La gerarchia dei registri prevede che ognuno di essi possa essere suddiviso per accedere a porzioni(sottoregistri) di dimensione minore 
per ragioni di compatibilità retroattiva: 32, 16, 8-bit(bassi),8 bit(alti).
64    32    16 8L 8H
------------------
RAX	EAX	AX	AL	AH
RBX	EBX	BX	BL	BH
RCX	ECX	CX	CL	CH
RDX	EDX	DX	DL	DH
RSI	ESI	SI	SIL	-
RDI	EDI	DI	DIL	-
RBP	EBP	BP	BPL	-
RSP	ESP	SP	SPL	-
R8-15 R8D-15D R8W-15W R8B-15B  -

---REGISTRI GENERAL PURPOSE-------------------------------------------------------------------------
1. RAX (Accumulator): obbligatorio per i valori di ritorno delle funzioni e usato implicitamente in divisioni e moltiplicazioni.
   E' inoltre il registro dove viene messo l'id della syscall da eseguire prima di usare il mnemonico syscall per evocarla.
   Se faccio la divisione Assembly div rbx, la CPU assume implicitamente che il dividendo si trovi nei registri RDX e RAX (per formare 
   un numero a 128 bit,il doppio del divisore), e piazza il risultato in RAX e il resto in RDX. Prima di una divisione con segno (idiv), se il divisore è a 64 bit, 
   si usa l'istruzione cqo (Convert Quadword to Octaword) per estendere il bit di segno di RAX su tutti i 64 bit di RDX.
   Se la struttura ritornata e' superiore a 64 bit,la dimensione del registro,il compilatore C la posizione meta' in RAX e l'altra meta' in RDX.
   Se ancora piu' grande,ad esempio una struct di 500 byte passata per valore,scatta l'Hidden Pointer. Il chiamante alloca 500 bytes nello stack e passa dietro le quinte
   alla funzione l'indirizzo di quello spazio come primo argomento in RDI.
2. RBX (Base): registro generale callee-saved usato anticamente per offset di memoria.
3. RCX (Counter): usato nei cicli hardware(istruzione loop o rep). È il 4° argomento nelle funzioni C. Loop e' un'istruzione base che decrementa RCX e 
   salta alla label specificata se RCX non è zero. Oggi è considerata obsoleta e lenta perché mal digerita dalla pipeline della CPU. Rep invece non è un'istruzione, 
   è un prefisso hardware che si attacca a operazioni su stringhe (come movsb, stosb). Esegue la micro-istruzione ripetutamente finché RCX non è zero. È devastantemente 
   più veloce di un ciclo manuale perché bypassa la fase di fetch per ogni singola iterazione e lavora a blocchi nei bus interni della CPU.
4. RDX (Data): estensione matematica per divisioni/moltiplicazioni e 3° argomento C. 
5. RSI (Source Index) e RDI (Destination Index): nati per operazioni massive (spostare MB di dati) su stringhe. Per es. l'istruzione 
   rep movsb prende RCX come contatore, legge da RSI e scrive su RDI via hardware. Oggi si preferiscono funzioni ottimizzate SIMD, 
   ma si usano questi registri per passare il 1° e 2° argomento nelle chiamate a funzione C per pura convenzione ABI.
6. RBP (Frame Pointer) e RSP (Stack Pointer): gestione dello stack.
7. R8  fino a R15: aggiunti per avere più spazio. R8 e R9 sono il 5° e 6° argomento C. R12-R15 sono callee-saved.

---SOTTOREGISTRI------------------------------------------------------------------------------------
La divisione in sottoregistri è pura architettura hardware,ottenuta attraverso il wiring dei registri. Si tratta in sostanza sempre 
della stessa porzione di silicio (hardware masking).  Non si va sotto gli 8 bit perché il byte è la minima unità indirizzabile logicamente in ram.
Un importante effetto collaterale delle scritture nei sottoregistri a 32-bit e' che in x86-64, esse azzerano automaticamente i 32 bit superiori(MSB) del 
corrispondente registro a 64-bit. Le scritture su sotto-registri a 8 o 16 bit ,invece, preservano intatti i bit rimanenti del registro a 64 bit.
Le CPU moderne eseguono le istruzioni Out-of-Order (fuori ordine),cioe' non necessariamente nell'ordine in cui vengono fetchate dal backend e sottoposte al 
frontend. Se scrivo 8 bit in AL, la CPU non sa se la successiva istruzione userà l'intero registro RAX. Questo crea una Partial Register Stall: i vecchi 56 bit 
di RAX restano intatti e la CPU deve fermare l'esecuzione per unire i nuovi 8 bit con i vecchi 56 bit (falsa dipendenza).
Per x86-64, AMD ha deciso che le operazioni a 32 bit sono le più frequenti in C (int). Per ottimizzare il calcolo e rompere la dipendenza dal passato 
del registro, l'hardware è cablato per cancellare istantaneamente con degli zeri i 32 bit superiori(zero-extension), azzerando le dipendenze hw e accelerando l'esecuzione.

---REGISTRI SPECIALI---------------------------------------------------------------------------------
Registri Speciali, non general purpose:
-RSP (stack pointer): mantiene l'indirizzo dell'ultimo elemento inserito in cima allo stack.
-RBP (base/frame pointer): utilizzato storicamente per ancorare il frame corrente della funzione sullo stack: punta all'inizio del frame. Un frame (o stack frame) è semplicemente la fetta di stack dedicata 
 alla singola esecuzione di una funzione. Contiene i parametri salvati, l'indirizzo a cui tornare e le variabili locali. Quando la funzione fa ret, il frame viene dimenticato.
-RIP (instruction pointer): mantiene l'indirizzo di memoria dell'istruzione successiva da eseguire. Quando il backend fa il fetch dell'istruzione corrente,avanza automaticamente
 RIP prima ancora che il frontend abbia finito di eseguirla.
-RFLAGS(registri di stato): registro contenente una serie di flag a singolo bit aggiornati dalle operazioni aritmetico-logiche per riflettere lo stato dell'ultima operazione:
        -ZF (zero flag): impostato a 1 se il risultato dell'operazione è zero.
        -SF (sign flag): impostato al bit più significativo del risultato (indica un valore negativo se in complemento a due).
        -CF (carry flag): impostato se si verifica un riporto/prestito in operazioni unsigned.
        -OF (overflow flag): impostato se si verifica un overflow aritmetico in operazioni signed.

I registri speciali che terminano in P(Pointer) contengono indirizzi di memoria (puntatori, esattamente come in C), ma hanno scopi hardware e convenzionali 
molto diversi e non intercambiabili.

1. RIP (Instruction Pointer): NON è un registro General Purpose, quindi per es. non ci si puo' scrivere direttamente con mov rip, 0x10 . Contiene sempre 
   l'indirizzo di memoria virtuale della prossima istruzione macchina da eseguire. In architettura Little Endian, RSP punta al bit meno significativo (LSB) 
   dell'intero dato salvato. Quando il sistema operativo (tramite la syscall execve) carica in RAM l'eseguibile, la CPU inizializza RIP all'indirizzo del punto 
   di ingresso (_start). Quest'ultimo non e' un pointer,ma una label, cioe' un' etichetta testuale che diventa un indirizzo di memoria,ed e' il vero e assoluto entry point del file ELF.
   Se per esempio nel terminale faccio ./mio_programma ciao mondo sto passando degli argomenti al programma. In C sono come argc (che in questo caso vale 3) e argv (un array di stringhe: ["./mio_programma", "ciao", "mondo", NULL]).
   Quando il kernel esegue la syscall execve per lanciare il programma, NON carica tutto l'eseguibile in RAM,ma crea solo la mappa della memoria (Virtual Memory).
   Il codice viene caricato in RAM "on-demand" (a blocchi di 4096 byte, o Page) solo quando la CPU tenta di eseguirlo(demand caching). 
   Il main in C non è la prima cosa eseguita: il file ELF specifica un indirizzo hardware di partenza chiamato tipicamente _start (fornito dalla libreria C) che prepara gli argomenti e poi chiama il main. 
   Quindi quando nella shell eseguo il mio eseguibile ,la shell esegue una fork,creando un processo figlio che e' un clone esatto del padre. Il figlio invoca la syscall execve che chiede al kernel di avviare il binario.Il kernel,dopo la chiamata a execve,
   prima ancora di eseguire una sola istruzione del machine code, svuota completamente la memoria virtuale del processo chiamante(il figlio,clone della shell),si disfa del vecchio stack e heap, e mappa al loro posto le sezioni .text, .data e .bss del file ELF,e poi prepara il nuovo stack,
   prendendo gli argomenti passati da terminale(argc e argv) e le variabili d'ambiente dell'sistema operativo e li pusha fisicamente in cima allo stack del processo appena creato.
   Se potessi ispezionare lo stack nell'istante in cui il Kernel cede il controllo a _start, troverei questa esatta struttura, letta dall'indirizzo puntato da RSP a scendere (verso indirizzi più alti):
   [RSP] --> contiene argc (un numero intero a 64-bit).
   [RSP + 8] --> contiene argv[0] (il puntatore alla stringa "./mio_programma").
   [RSP + 16] --> contiene argv[1] (il puntatore alla stringa "ciao").
   [RSP + 24] --> contiene argv[2] (il puntatore alla stringa "mondo").
   [RSP + 32] --> contiene un puntatore NULL (che segna la fine di argv).
   [RSP + 40] --> contiene envp[0] (il puntatore alla prima variabile d'ambiente, es. "USER=tobia")
   .... e così via.
   Soltanto dopo il kernel setta il registro RIP all'indirizzo di _start e fa partire l'esecuzione in user space. L'entrypoint _start non la scrive il programmatore, ma viene iniettata automaticamente all'inizio dell' eseguibile dal Linker (la preleva da un file oggetto 
   precompilato della glibc chiamato crt1.o, dove "crt" sta per C Runtime).La C runtime e' la stessa che al momento del return 0 cattura lo 0 ed invoca per conto dell'utente la sys_exit.
    Il codice Assembly contenuto dentro _start deve fare da ponte tra lo stack grezzo preparato dal Kernel e la comodità del int main(int argc, char **argv). Ecco cosa fa passo passo l'Assembly di _start:
   -Estrae gli argomenti dallo stack: legge il valore puntato da RSP (argc) e lo mette nel registro RDI (primo argomento per convenzione ABI). Poi calcola l'indirizzo del primo puntatore argv (RSP + 8) e lo mette in RSI (secondo argomento ABI).
   -Identifica le variabili d'ambiente (envp): scorre lo stack per trovare il NULL che separa argv da envp e salva anche quel puntatore.
   -Allinea lo Stack: come impone la System V ABI, forza l'allineamento di RSP ai 16 byte mascherando i bit più bassi (spesso con un and rsp, ~15).
   -Chiama il Main: ora che i registri RDI e RSI contengono correttamente argc e argv, e lo stack è allineato a 16 byte, _start fa call main (in realtà, nelle libc moderne, chiama una funzione intermedia chiamata __libc_start_main, passandole l'indirizzo del main, ma concettualmente è la stessa cosa).
   -Gestisce la morte del processo: questa è la parte cruciale. Quando il tuo main in C fa return 0; in realta' fa una ret. L'istruzione ret dice alla CPU di fare pop dallo stack e tornare al chiamante. Il chiamante del main è _start.
   _start preleva il valore di ritorno che il main ha lasciato nel registro RAX (es. lo 0), lo sposta in RDI, mette il numero 60 in RAX (il codice identificativo della syscall sys_exit), ed esegue l'istruzione hardware syscall. Il processo muore pulito.
    Cosa succederebbe se non ci fosse _start? Se ingannassi il linker compilando senza la glibc (opzione -nostdlib) e dicessi al Kernel di iniziare l'esecuzione direttamente dal tuo main, il programma andrebbe subito in Segmentation Fault. Il tuo main cercherebbe argc nei registri RDI o RSI, trovando 
    spazzatura (perché il Kernel li ha lasciati brutalmente sullo stack senza metterli nei registri conformi all'ABI), e quando il main fa return 0, l'istruzione ret troverebbe in cima allo stack non un indirizzo di ritorno valido, ma il numero di argc, causando uno schianto immediato dell'architettura.
    Quando NASM assembla il codice sostituisce la label start con le istruzioni Assembly scritte a partire dall'indirizzo di memoria fisico a cui si riferiva quella label. Questo codice estrae i parametri da riga di comando(argc e argv) che il kernel ha pushato sullo stack ,configura le variabili d'ambiente,
    allinea lo stack e infine esegue la call al main. Da lì, la CPU incrementa automaticamente RIP dopo aver fetchato ogni istruzione in base alla lunghezza in byte dell'istruzione stessa.
2. RSP (Stack Pointer): e' un general purpose,l'hardware lo usa implicitamente ogni volta che usi le istruzioni push, pop, call o ret,che si interfacciano 
   con la memoria stack. Contiene l'indirizzo dell'ultimo(msb) byte occupato in cima allo stack.
3. RBP (Base Pointer) : e' storicamente usato per salvare una copia statica di RSP all'inizio di una funzione.In sostanza all' inizio di una funzione si fa push rbp, 
   poi mov rbp, rsp. Essendo RBP fisso, permette di trovare facilmente le variabili locali e gli argomenti sullo stack a offset costanti (es. [rbp - 8] per la prima 
   variabile locale, - 16 per la seconda e cosi' via), anche se RSP cambia durante la funzione.

---STACK E ISTRUZIONI RSP---------------------------------------------------------------------------
La ram e' divisa in diverse sezioni logiche mappate dal kernel,che altro non sono che le rappresentazioni in ram delle sezioni del sorgente (il file elf sul disco):
-.text: codice eseguibile ,cioe' solo gli opcode(permessi di sola RX, lettura ed esecuzione).
-.data / .bss: variabili globali e locali statiche. Data contiene quelle inizializzate,bss quelle non inizializzate o che valgono zero. Le prime devono essere codificate nell'eseguibile su disco,appesantendolo,le seconde
 non pesano un singolo byte nell'eseguibile: il sistema operativo sa solo che all'avvio del programma deve allocare spazio per quelle variabili e riempirlo di zeri. Memoria RW(la cpu puo' leggere e scrivere,ma non eseguire,per evitare attacchi di tipo buffer overflow).
-.rodata : la memoria dedicata alle costanti(per esempio le stringhe literal o le variabili const). Solo lettura R,niente scrittura o esecuzione.
- heap: memoria allocata dinamicamente a runtime,che quindi non trova corrispondenza nel sorgente(es. malloc). Parte da un indirizzo basso (sinistra) e cresce verso destra (indirizzi crescenti).
- stack : sezione di memoria RW(read/write) usata per l'allocazione temporanea dei dati,parte da un indirizzo altissimo (destra estrema) e cresce verso sinistra (indirizzi decrescenti). Anche lei non ha un corrispondente diretto nel sorgente: il kernel crea lo stack vuoto al momento del lancio dell'eseguibile.
  Ci finiscono tutte le variabili locali dichiarate dentro le funzioni(main compreso),che nascono con la funzione e muoiono con il ret della funzione.

In un architettura x86-64, la memoria dello stack,se immmaginiamo la ram come un enorme riga di celle da un msb 0x0 posto a sinistra ad un lsb 0xFFFFFFFFFFFFFFFF
posto a destra(architettura little endian), cresce verso sinistra, cioe' verso indirizzi decrescenti.
Le istruzioni PUSH,POP,CALL e RET sono mnemonici Assembly a cui corrispondono istruzioni opcode ISA dirette che manipolano automaticamente RSP e la memoria puntata da esso:
-l'istruzione PUSH SRC(es. push rax) decrementa RSP di 8 byte (RSP = RSP - 8),cioe' lo sposta a sinistra e scrive gli 8 bytes contenuti in src all'indirizzo [RSP].
-l'istruzione POP DEST(es. pop rbx) legge gli 8 bytes all'indirizzo puntato  da [RSP], li mette nel registro di destinazione dest e incrementa RSP di 8 byte (RSP = RSP + 8),
 cioe' lo fa ritrarre verso destra.
-l'istruzione CALL <LABEL>: una label in assembly (es. ft_strlen) è solo un segnaposto testuale che il compilatore rimpiazza con un vero indirizzo di memoria. 
 Quando la CPU esegue l'istruzione call, compie tre azioni atomiche sul chip:
 1. calcolo del Return Address: prende l'indirizzo corrente in RIP e gli somma la lunghezza in byte dell'istruzione call stessa. Il risultato è l'indirizzo dell'istruzione successiva, ovvero il punto esatto a cui rientrare al termine della chiamata.
 2. push sullo Stack (RSP): decrementa RSP di 8 byte (sub rsp, 8), poiché lo stack cresce verso indirizzi di memoria inferiori,e poi scrive l'indirizzo di ritorno calcolato al punto 1 nella cella di memoria puntata da [RSP](push rip).
 3. caricamento del nuovo RIP: sovrascrive RIP con l'indirizzo della funzione target.
 Dal ciclo di clock successivo, il decodificatore della CPU inizia a pescare le istruzioni a partire dal nuovo indirizzo caricato in RIP. 
 Quindi in sostanza call spinge mplicitamente(push rip) l'indirizzo dell'istruzione successiva (push rip) sullo stack e salta a label(jmp label).
-l'istruzione RET(near return) esegue implicitamente un pop rip, quindi legge l'indirizzo a 64 bit presente nella cella di memoria puntata in quel momento dallo Stack Pointer ([RSP]),indirizzo che call aveva lasciato in cima allo stack,
 poi incrementa il registro RSP di 8 byte (add rsp, 8), completando a livello hardware l'operazione di pop,quindi carica l'indirizzo appena estratto direttamente dentro l'Instruction Pointer (RIP),cosi' che il chiamante
 possa riprendere l'esecuzione da dove si era interrotta prima della call.

 La simmetria tra ret e call e' assoluta: sono una l'esatto speculare dell'altra.
 
---DICHIARAZIONE DI UNA FUNZIONE IN ASSEMBLY--------------------------------------------------------
Nella sintassi NASM Assembly X86-64 la dichiarazione di una funzione non richiede alcuna firma(quella e' un astrazione di linguaggi di alto livello come C che non lascia traccia nel machine code),
ma semplicemente la definizione di tre elementi:
1) la definizione della direttiva global <nome_funzione> per rendere la funzione visibile all'esterno(che si tratti del main o del linker).
2) la definizione della label, cioe' del nome della funzione seguito dai :   ,che funge da segnaposto che marca l'indirizzo di memoria dell'inizio della funzione.
3) l'istruzione ret in fondo per restituire l'esecuzione al chiamante. Ret non ha nulla a che fare con il tipo di ritorno della funzione: ritornare un tipo o void e' un astrazione di C che serve 
   unicamente per definire se dobbiamo o meno inserire un valore di ritorno in rax. L'istruzione ret non ha nulla a che fare nemmeno col valore restituito: serve solo a fare pop del return adress
   ,salvato precedentemente dal chiamante in cima allo stack, per metterlo dentro RIP e restituire il flusso di esecuzione al chiamante. Senza ret la cpu non saprebbe che la funzione e' terminata e
   continuerebbe a leggere ed eseguire i byte successivi in memoria == Segfault.

Nessuna dichiarazione del tipo di ritorno,o del tipo e del numero di argomenti e' necessaria: Assembly e' un linguaggio di basso livello che si interfaccia direttamente con 
bytes,registri e indirizzi. Il solo "contratto" che va rispettato e' l'accordo implicito sugli standard dei registri che verranno usati(ABI): la funzione assume che il chiamante abbia gia' caricato gli
argomenti nei registri giusti e che il chiamante si aspetti il risultato nel registro rax.
La firma formale della funzione esiste esclusivamente nel prototipo dichiarato nel file header C(.h) o nel sorgente C,ma serve solo al compilatore per controllare i tipi prima di generare le istruzioni che riempiono
i registri.
   
STRUTTURA MINIMA DI BASE:
global

nome_funzione:
    ; istruzioni
    ret

---MEMORY ALIGNMENT E CHIAMATE A FUNZIONE-----------------------------------------------------------
Quando una funzione scritta in C ne chiama un'altra (o quando chiamiamo una funzione di libreria da Assembly), entrambe devono rispettare una convenzione binaria 
universale o calling convention(ABI- Application Binary Interface). Sui sistemi UNIX/Linux/macOS x86-64 si applica la System V AMD64 ABI. 
La System V ABI stabilisce che, al momento dell'esecuzione dell'istruzione call verso un'altra funzione, il registro RSP deve essere allineato a un multiplo di 16 byte (RSP % 16 == 0).
Un indirizzo allineato a 16 byte significa che è un multiplo intero di 16,quindi in binario gli ultimi 4 bit devono essere zero(16 & 15 == 0). In notazione esadecimale ogni carattere rappresenta 
esattamente un nibble di 4 bit. Quindi, qualsiasi indirizzo multiplo di 16 termina col carattere 0 (es. 0x7ffd9b8a0).
Prima che il chiamante esegua call, lo stack è allineato a 16 byte. L'istruzione call spinge il return address (8 byte) sullo stack.
All'ingresso della funzione, RSP NON è più allineato a 16 byte (risulta sfalsato di 8 byte: RSP % 16 == 8).Pertanto, prima di eseguire  una sotto-chiamata,cioe' una chiamata a funzioni esterne,senza eccezioni,
(call malloc, call write, ecc.),si deve ripristinare l'allineamento a 16 byte nello stack riservando memoria (es. sottraendo byte da RSP o effettuando un numero dispari di push). Un disallineamento provocherà 
Segmentation Fault imprevedibili  all'interno di funzioni di libreria C che usano istruzioni SIMD/SSE. La libreria standard C (glibc) per funzioni come malloc o printf usa pesantemente estensioni SIMD/Vectoriali per 
elaborazioni velocissime sui blocchi. Le istruzioni SIMD dell'ISA (come movaps) esigono che i dati in RAM si trovino ad indirizzi multipli di 16 (indirizzi che finiscono in 0). Se la memoria non è 
allineata a 16 byte, la CPU solleva un hardware fault (Segmentation Fault).Quando il codice esegue call malloc, la call fa implicitamente un push di 8 byte (RIP). Questo significa che dentro malloc, 
lo stack non è più un multiplo di 16, ma è sfalsato di 8. Per evitare che malloc crashi quando usa istruzioni SIMD, l'ABI impone che, prima di chiamare una qualsiasi funzione C, si debba assicurare che 
l' RSP sottratto di 8 (quello che avverrà con la call) risulti essere un multiplo di 16. Questo si fa tipicamente eseguendo una push fittizia o facendo sub rsp, 8 prima della call.

---REGISTRI E ARGOMENTI DELLE FUNZIONI--------------------------------------------------------------
I primi 6 argomenti di tipo intero o puntatore vengono passati esclusivamente tramite registri nel seguente ordine tassativo:
-1° Argomento: RDI
-2° Argomento: RSI
-3° Argomento: RDX
-4° Argomento: RCX
-5° Argomento: R8
-6° Argomento: R9
Gli eventuali argomenti successivi (dal 7° in poi) vengono spinti sullo stack in ordine inverso (da destra a sinistra).
Il valore di ritorno restituito da una funzione (un valore scalare, un intero o un puntatore) deve essere salvato nel registro RAX.
Se la funzione restituisce un valore a 8 bit (es. char), questo risiederà in AL; se a 32 bit (es. int), in EAX; se a 64 bit (es. puntatore o long), nell'intero RAX.
Gli argomenti non interi o di tipo float non passano da RDI o RSI: l'ABI System V richiede che i float passino nei registri vettoriali da XMM0 fino a XMM7.

---CALLER, CALLEE E REGISTRI------------------------------------------------------------------------
Il Chiamante (Caller) e' la funzione che sta eseguendo, per esempio il main. Il Chiamato (Callee) e' la funzione che viene invocata, ad esempio una chiamata ad ft_strlen nel main 
o in un altra funzione. In virtu' di questa distinzione, i registri sono divisi in due categorie operative: 
-Callee-Saved Registers (preservati dal chiamato / non-volatili): RBX, RBP, R12, R13, R14, R15.
 Se una funzione intende modificare uno di questi registri, ha l'obbligo di salvarne il valore originale sullo stack all'inizio della funzione (prologo) e ripristinarlo 
 prima di ritornare al chiamante (epilogo). Esempio: se il main ha un dato in RBX, e chiama ft_strlen, la convenzione ABI (System V) stabilisce che ft_strlen (il chiamato) non può 
 alterarlo in modo permanente. Se ft_strlen vuole usare RBX per i suoi scopi, deve pusharlo sullo stack, usarlo, e popparlo prima di fare ret.
-Caller-saved registers (preservati dal chiamante / volatili/ scratch register): RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11.
 Una qualsiasi chiamata di funzione esterna (es. call malloc o call write) potrebbe sovrascrivere indisturbata tutti questi registri! Esempio: se il main ha un valore importante 
 in RAX, e chiamo ft_strlen, quando ft_strlen ritorna, RAX sarà stato sovrascritto dal suo valore di ritorno. E' responsabilità del chiamante salvarlo nello stack prima del call.

L'uso di un registro non volatile da parte di una funzione non produrra' alcun errore in fase di compilazione: ne NASM ne' ld eccepiranno alcunche'. Il disastro avverra' a runtime: se il callee 
viola il contratto d'onore (ABI) che e' tenuto a rispettare e usa un registro(per esempio rbx), in cui il chiamante ha depositato un dato critico,senza farne il push sullo stack, quando la funzione fara'
ret e il chiamante riprende l'esecuzione convinto che rbx contenga ancora il valore originale, si presentera' uno spettro di errori che puo' andare dal segfault immediato alla corruzione silenziosa di dati
in memoria fino a crash inaspettati in esecuzione.

---DIFFERENZA TRA FUNZIONI DI LIBRERIA E SYSCALL----------------------------------------------------
Mentre le chiamate a funzioni C impiegano l'istruzione call, le chiamate di sistema(System Calls) dirette al kernel Linux (come read e write) impiegano l'istruzione hardware speciale ISA syscall. 
L' utente (Ring 3) non puo' interagire con l'hardware del PC (scrivere su schermo, aprire un file, mappare memoria). Lo vietano i circuiti della CPU. Solo l'OS (Linux, Ring 0, modalità privilegiata) può farlo.
Per scavalcare questo limite, l'ISA possiede l'istruzione syscall. Questa istruzione genera un interrupt: blocca la  esecuzione del codice user space, concede poteri assoluti al kernel Linux, il quale esegue la 
richiesta in base al numero che viene caricato nel registro RAX (es. 0 = sys_read, 1 = sys_write, 9 = mmap... ce ne sono circa 350) ,cioe' l'identificatore numerico della syscall (registry ID syscall) .
I registri in cui sono salvati gli argomenti delle syscall sono leggermente differenti da quelli delle funzioni di libreria:
-1° Argomento: RDI
-2° Argomento: RSI
-3° Argomento: RDX
-4° Argomento: R10 (DIVERSO: le funzioni C usano RCX, ma l'istruzione syscall usa R10). Syscall necessita di usare RCX internamente per salvare temporaneamente l'indirizzo RIP a cui tornare. Siccome RCX viene distrutto dall'istruzione, 
    il kernel Linux ha deciso che il 4° argomento della syscall deve risiedere in R10, a differenza delle funzioni C che usano RCX.
-5° Argomento: R8
-6° Argomento: R9

Effetti collaterali hardware: l'istruzione syscall sovrascrive internamente i registri RCX e R11. L'istruzione syscall fa una cosa estrema: scavalca il Ring di sicurezza della CPU saltando nello spazio del Kernel. Come fa il Kernel, una volta finito, a sapere esattamente a quale indirizzo RIP tornare,
senza sporcare la memoria stack (che cambierà contesto di esecuzione)? I progettisti hardware hanno deciso che, nell'istante in cui la CPU esegue syscall, il silicio prende fisicamente il valore corrente di RIP (l'indirizzo a cui tornare) e lo sovrascrive con violenza nel registro RCX. Contemporaneamente, 
prende lo stato dei flag (RFLAGS) e lo sbatte in R11. Ecco l'effetto collaterale: non si puo' usare RCX per passare argomenti, perché l'hardware lo piallerà in un nanosecondo per usarlo come salvataggio del RIP. Motivo per cui l'istruzione C usa RCX come 4° argomento, ma la chiamata syscall kernel ha dovuto ripiegare su R10.

---INTERRUPT E SYSCALL------------------------------------------------------------------------------
Un interrupt è un segnale di sistema che ordina alla CPU di sospendere immediatamente l'esecuzione del programma corrente per saltare a una routine speciale ad alta priorità (l'ISR - Interrupt Service Routine), per poi riprendere il programma originale da dove era stato interrotto.
sono il meccanismo hardware fondamentale che abilita la prelazione (preemption) e il multitasking reale. In un sistema operativo moderno, la concorrenza non è "cooperativa" (ovvero un programma non cede il controllo spontaneamente quando ha finito). L'hardware usa gli interrupt per strappare il controllo al codice in esecuzione:
-il Timer Interrupt (Heartbeat del OS): un chip hardware (l'APIC) invia alla CPU un interrupt a intervalli regolari (es. ogni millisecondo). Quando scatta, la CPU ferma qualsiasi programma stia eseguendo e passa il controllo allo scheduler del kernel, che decide se fare un context switch e passare a un altro processo.
-priorità: gli interrupt hardware hanno priorità assoluta rispetto all'esecuzione in user-space. Se arriva un pacchetto di rete, il controller NIC invia un segnale hardware: la CPU sospende l'app utente, esegue la routine di ricezione dati nel driver e poi restituisce il controllo.
La differenza tra interrupt sincrono e asincrono risiede nella relazione con il flusso di istruzioni che la CPU sta eseguendo in quel preciso istante:
1) Sincrono (Syscall / Traps / Eccezioni): e' deterministico e legato all'istruzione corrente. Se scrivo syscall, una divisione per zero (div), o accedo a un puntatore nullo, l'evento viene generato dalla CPU stessa mentre decodifica ed esegue quell'istruzione.
   Se rieseguo il programma con gli stessi dati, l'evento avverrà sempre e rigorosamente nello stesso identico punto (stesso RIP). È sincronizzato con il clock di esecuzione del codice.
2) Asincrono (Hardware Interrupts / IRQ): e' completamente slegato dal codice in esecuzione. Viene generato da un componente hardware esterno alla CPU (tastiera, SSD, scheda di rete, timer).
L'evento può scattare tra l'istruzione N e l'istruzione N+1 di un ciclo for in un momento del tutto imprevedibile. La CPU non sa quando arriverà: quando la linea elettrica del bus hardware si attiva, la CPU finisce l'istruzione corrente e devia l'esecuzione.

La differenza tra un interrupt (hardware o software) e l'istruzione syscall è profonda e riguarda la microarchitettura della CPU: syscall non usa la tabella degli interrupt (IDT) e non salva nulla sullo stack.
I tre meccanismi a confronto:
1) Interrupt Hardware (IRQ): asincrono, generato dalle periferiche fisiche (mouse, scheda di rete,controller ssd,tastiera, timer APIC). Quando la periferica ha dati pronti, invia un segnale elettrico a una linea fisica del processore o invia un messaggio di rete sul bus PCIe (tecnologia MSI/MSI-X).
La CPU ferma quello che sta facendo, salva lo stato completo sullo stack, consulta la IDT e salta al driver. Il driver è semplicemente il codice (la ISR) che il kernel carica in memoria e a cui la CPU salta quando riceve quel segnale hardware.
2) Interrupt Software / Trap (int 0x80): sincrono. Meccanismo storico a 32 bit, ormai obsoleto e inutilizzato per le syscall sui sistemi a 64 bit: la CPU simula un interrupt hw via software: legge la IDT (Interrupt Descriptor Table) in RAM, fa i controlli sui privilege level (DPL/CPL), cambia stack frame via TSS (Task State Segment) e 
pusha automaticamente sullo stack CS, RIP, RFLAGS, SS, e RSP.Latenza: enorme (anche oltre 100-200 cicli di clock solo per passare a Ring 0). Per quanto riguarda la richiesta di servizi al sistema operativo (syscall), sono obsoleti. Ma gli interrupt software (più propriamente detti eccezioni e trap) sono vivi e fondamentali per altre mansioni:
-Eccezioni del processore: quando fai una dereferenziazione NULL o accedi a memoria non mappata, l'MMU genera un Page Fault (#PF, interrupt 14). Quando dividi per zero, la CPU scatena un #DE (interrupt 0).
-Debugging: i breakpoint di GDB o l'istruzione int 3 (opcode 0xCC) invocano l'interrupt software 3 per fermare l'esecuzione e restituire il controllo al debugger.
3) Fast System Call (syscall): sincrono. Istruzione assembly dedicata introdotta con x86-64 (AMD64) per saltare interamente la IDT e i Push hardware sullo stack. Latenza: minima(~20-30 cicli di clock). Sui sistemi operativi moderni a 64 bit (Linux, macOS, Windows) le syscall usano esclusivamente 
le Fast System Call (syscall su architettura x86-64 e svc su ARM64).

Come funziona syscall a basso livello? All'avvio del sistema operativo (boot), il kernel Linux configura dei registri speciali interni alla CPU chiamati MSR (Model-Specific Registers) tramite l'istruzione wrmsr.I tre MSR fondamentali per le syscall sono:
1) IA32_LSTAR (Long Mode Target Address): contiene l'indirizzo di memoria virtuale della funzione di entry point del kernel per tutte le syscall (in Linux è entry_SYSCALL_64).
2) IA32_STAR: contiene i Segment Selector per i segmenti di codice e dati di Ring 0 (kernel) e Ring 3 (user).
3) IA32_FMASK (Flag Mask): maschera hw usata per azzerare automaticamente specifici bit di RFLAGS (ad esempio disabilita gli interrupt hardware azzerando l'Interrupt Flag IF). Il kernel scrive una sequenza di bit con valore 1 in corrispondenza di ogni flag di RFLAGS che vuole disabilitare quando si entra in Ring 0.

Cosa fa l'hardware quando eseguo syscall? Quando la CPU incontra l'opcode di syscall in modalità a 64 bit, esegue esclusivamente in hardware questa sequenza atomica:
1) Salva lo stato corrente nei registri (non sullo stack): copia l'indirizzo dell'istruzione successiva (RIP) nel registro RCX e il registro di stato RFLAGS nel registro R11. 
2) Applica la maschera di stato: RFLAGS = RFLAGS AND NOT (IA32_FMASK). Una pura operazione bitwise per azzerare specifici bit di controllo del registro RFLAGS al momento dell'ingresso in kernel mode. NOT inverte tutti i bit della maschera (IA32_FMASK),e l'AND tra RFLAGS e la maschera invertita forza a 0 gli indirizzi di RFLAGS accoppiati ad indirizzi
della maschera che sono attualmente 0(quindi che erano ad 1 nella maschera prima del not,e che sono quelli che voglio disabilitare in RFLAGS),lasciando inalterati tutti gli altri. Il bit piu' importante di IA32_FMASK e' il bit 9,che corrisponde all'interrupt Flag(IF) di RFLAGS. Quando la cpu esegue l'istruzione syscall,passa a ring 0,ma l'instruction 
pointer RIP nei primi microsecondi si trova ancora nell'entry point del kernel e la cpu sta ancora usando lo stack dell'utente(RSP punta in sostanza a memoria user space). Se in questo esatto istante arrivasse un interrupt hw e gli interrupt fossero attivi,la cpu tenterebbe di salvare lo stato dell'interrupt sullo stack utente,causando 
una corruzione immediata del kernel o aprendo la porta a vulnerabilita' di sicurezza(privilege escalation).Azzerando il bit IF, la cpu disabilita temporaneamente l'ascolto di qualsiasi interrupt hw,dando cosi' il tempo al kernel di eseguire in sicurezza l'istruzione swapgs,
sostituire RSP con uno stack sicuro in kernel space,e solo successivamente riattivare gli interrupt. Quindi nel passaggio al kernel mode IF viene impostato a 0 e gli interrupt disabilitati.
3) Cambia i Segmenti e il Livello di Privilegio (Ring 3 --> Ring 0): carica i nuovi CS e SS presi da IA32_STAR (impostando il CPL a 0).
4) Salta all'entry point del Kernel: carica l'Instruction Pointer RIP direttamente da IA32_LSTAR, bypassando completamente la mappa delle interruzioni

Chi ha priorita' tra un interrupt hw,uno software e una syscall?
Ecco come interagiscono e come l'architettura gestisce le precedenze:
1) Interrupt HW vs Flusso del programma (compresa syscall). Gli Interrupt Hardware hanno precedenza sull'esecuzione del codice normale. Se arriva un segnale da un chip esterno (es. tastiera o scheda di rete) mentre il mio programma sta per eseguire syscall, la CPU sospende l'esecuzione, gestisce l'interrupt hardware e solo dopo fa eseguire la syscall.
   L'unica eccezione e' se l'Interrupt Flag è disabilitato (IF = 0, come accade nei primi microsecondi di ingresso nel kernel), gli interrupt hardware rimangono in attesa ("pending") finché IF non viene riportato a 1.

2) Gerarchia di priorità x86 (Eventi nello STESSO ciclo di clock)
   Se durante l'esecuzione di un'istruzione si verifica un'eccezione interna e contemporaneamente arriva un segnale hardware sulla scheda madre, la CPU x86 applica questa gerarchia rigida:
   -Hardware Reset / Fault catastrofici: (es. Power Failure, Machine Check, errore di bus durante il fetch).
   -Eccezioni dell'istruzione corrente / Interrupt SW: (es. divisione per zero, Page Fault, int 3). Devono essere gestiti immediatamente perché l'istruzione corrente è fallita e non può essere completata.
   -Interrupt Hardware esterni: (es. APIC Timer, pacchetto di rete, disco). Vengono serviti subito dopo aver terminato/risolto l'istruzione corrente, prima di passare all'istruzione successiva indicata da RIP.

Perché è fondamentale distinguerli? Se fosse un interrupt software la CPU scriverebbe sulla memoria (stack dell'utente o del kernel) prima ancora di far eseguire una sola riga di codice al kernel. RCX e R11 non verrebbero toccati.
Con syscall invece ci sono zero accessi in RAM da parte dell'hardware. La CPU non fa nessun push sullo stack.Il cambio di stack da quello utente a quello kernel viene gestito successivamente dal codice del kernel stesso tramite l'istruzione swapgs.Ecco perché nell'ABI delle syscall rcx e r11 vengono distrutti: 
è la CPU stessa a sovrascriverli nell'istante in cui viene eseguita syscall.

---ERRNO--------------------------------------------------------------------------------------------
In C, le funzioni di sistema ritornano -1 in caso di errore e impostano la variabile globale del thread chiamata errno. A livello kernel, tuttavia, la meccanica è diversa:
se la syscall ha successo, RAX contiene il valore di ritorno positivo o nullo;se fallisce, il kernel restituisce in RAX un valore di errore negativo compreso 
nell'intervallo [−4095,−1] (corrispondente a -errno).
Per gestire in Assembly gli errori nel ritorno di una syscall, si verifica se il valore restituito da syscall in RAX è negativo (confrontandolo con la soglia d'errore).
Se negativo: si inverte il segno di RAX per isolare il codice d'errore positivo (es. neg rax),si preserva temporaneamente questo codice d'errore e poi si invoca la funzione 
della C library preposta a restituire l'indirizzo della variabile errno del thread corrente (tramite extern __errno_location su Linux).  
Bisogna ricordare di garantire l'allineamento dello stack a 16 byte prima di eseguire questa call!. La chiamata restituisce in RAX un puntatore alla variabile errno.
Si scrive il codice di errore positivo all'indirizzo puntato da RAX (mov [rax], errore).Infine si assegna -1 a RAX per indicare il fallimento al chiamante C.
ESEMPIO: il kernel, dopo aver eseguito sys_read, torna al codice del chiamante. Mette il numero di byte letti in RAX, ma se c'è un errore (file inesistente) non può generare variabili C. 
Si limita a restituire un numero negativo in RAX compreso tra -1 e -4095 (es. -2 per ENOENT). Come fa a sapere che un puntatore valido allocato con malloc (es. tramite mmap) non avrà mai 
un indirizzo tra -1 e -4095? Gli indirizzi di memoria su x86-64 sono numeri a 64 bit unsigned compresi tra 0 e 2^64 -1. Tuttavia, i valori di ritorno del Kernel in RAX vengono interpretati 
dal wrapper C in complemento a due (cioè come numeri signed). Lo spazio di indirizzamento della memoria virtuale di Linux riserva sempre gli ultimissimi kilobyte altissimi all'apice della memoria, 
vietando allocazioni lì in alto. Pertanto, qualsiasi valore in quel piccolissimo range finale (che castato a signed è -1 / -4095) è matematicamente inequivocabile: è un errore.
Per es. il numero -2 è 0xFFFFFFFFFFFFFFFE; se un programma allocasse memoria a quell'indirizzo astronomico, il wrapper crederebbe che sia l'errore -2. Per evitare questo, il Kernel Linux per convenzione 
non mappa mai l'ultimo frammento della memoria virtuale (quello che corrisponde ai numeri da -1 a -4095 in signed). Quindi, se RAX contiene una roba tipo 0xFFFFFFFFFFFFFFFE, il sistema sa al 100% che è un 
errore e non un puntatore restituito da malloc.
In C standard, le funzioni di libreria C per essere comode avvolgono (wrappano) l'istruzione syscall hardware (es. chiamando write() della unistd in C si invoca un wrapper che chiama l'istruzione 
hardware syscall numero 1). Questo wrapper prende il valore negativo restituito dal kernel, lo fa diventare positivo(il wrapper usa l'istruzione neg rax che inverte il segno trasformandolo in positivo) e lo salva 
dentro una variabile globale chiamata errno, restituendo -1 al chiamante, cosi' che lo sviluppatore possa guardare in errno. Tuttavia, errno non può essere una singola variabile globale in memoria (.bss): l
e variabili globali normali sono condivise tra tutti i thread del processo. Se il processo ha due thread logici che fanno syscall contemporanee ed entrambe falliscono, una sovrascriverebbe l'errore dell'altra, 
generando clamorose race condition. Pertanto errno è definita nello standard POSIX come Thread-Local Storage (TLS): il sistema operativo assegna a ogni singolo thread un piccolo segmento di memoria privata in cui e' 
salvata la propria copia isolata di errno.
Per accedere a errno in C si usa la libreria <errno.h>. Se in C scrivessi nel codice errno = 2 NON starei dichiarando una variabile globale: il preprocessore legge il file errno.h, trova  la direttiva 
#define errno (*__errno_location()),e ,prima ancora che il compilatore possa accedere al codice, trova la parola errno nel sorgente e la sostituisce con la parte destra della direttiva. Il codice consegnato 
al compilatore e' (*__errno_location()) = 2 ; il compilatore non ha mai visto la parola errno,ma solo una chiamata a funzione che ritorna un pointer, che viene dereferenziato e a cui viene assegnato il valore
di 2. Assembly ha un sistema di macro potentissimo (che usa una sintassi diversa, come %macro e %endmacro), che permette di creare alias di testo complessi per blocchi di istruzioni Assembly.
Ma NASM non legge i file header .h del C, quindi non ha idea di cosa sia la macro C #define errno. Quindi, se in Assembly voglio modificare l'errno del thread, non posso scrivere magicamente mov [errno], 2, 
perché per NASM la parola errno non significa assolutamente nulla. Bisogna bypassare l'astrazione del C e scrivere manualmente ciò che la macro nasconde: invocare la funzione della libc __errno_location
(il cui nome reale è formattato con i doppi underscore,usati storicamente nello standard C per nascondere simboli di sistema che non devono scontrarsi con i nomi usati dall'utente) che restituisce in RAX
l'indirizzo di memoria della errno del thread corrente. Il chiamante ha poi il compito di scrivere alla memoria puntata da quell' indirizzo l'errore convertito in numero positivo.
In sostanza in Assembly errno non è una vera variabile globale, è una macro complessa legata al segmento del thread! 
Una variabile è un costrutto semantico del linguaggio C destinato al compilatore: se dichiaro int x = 5;, sto dicendo al compilatore di riservare 4 byte di memoria fisica (nello stack o nella sezione .data) e di 
tenere traccia del suo indirizzo di memoria, oltre a ricordagli che contiene dati formattati come interi con segno, e di scriverci dentro il valore 5. La variabile esiste nello spazio e nel tempo durante l'esecuzione del programma. 
Una macro è pura, stupida e brutale sostituzione di testo destinata al preprocessore, che si occupa di un banale editing del sorgente prima della compilazione.



C
typedef struct s_list
{
    void          *data;
    struct s_list *next;
}   t_list;
Layout di Memoria di t_list in Architettura a 64 bit:
Un puntatore in architettura x86-64 occupa 8 byte (64 bit). Pertanto, l'offset di memoria della struttura t_list è rigorosamente determinato:  
PDF
+ 1

Offset 0 (0x00): Campo data (8 byte).  
PDF

Offset 8 (0x08): Campo next (8 byte).  
PDF

Dimensione totale del nodo: 16 byte.

  Indirizzo Nodo
  +-----------------------+-----------------------+
  |  void *data (8 byte)  | struct t_list *next   |
  +-----------------------+-----------------------+
  Offset: 0x00            Offset: 0x08
  
PDF
+ 4
Principi Operativi per i Bonus:
Navigazione del Puntatore: Per accedere al nodo successivo, si dereferenzia il puntatore offsettato di 8 byte: mov reg, [nodo_ptr + 8].

Invocazione di Function Pointer (cmp o free_fct):
Quando si passa un puntatore a funzione in un registro (es. RDX o RCX), l'invocazione dinamica si effettua mediante chiamata indiretta tramite il registro stesso (es. call rdx o call r10).

Preservazione dello Stato nei Cicli: Durante le iterazioni sulla lista che comportano chiamate esterne a cmp o free, i puntatori correnti della lista (t_list **begin_list) e i riferimenti ai nodi devono essere memorizzati nei registri Callee-Saved (RBX, R12-R15) per evitare che vengano corrotti dalla funzione di comparazione chiamata.
Rispondiamo a ogni singola domanda scendendo a livello dei transistor e del sistema operativo. Allacciati le cinture.

*/