## COSA E' UN PACKER

Un packer è uno strumento di manipolazione binaria post-compilazione che riceve in input un programma eseguibile (.exe, .dll, ELF) e genera un nuovo binario in cui il payload originale viene compresso e cifrato simultaneamente. La sua esistenza deriva dalla necessità di eludere l'analisi in memoria degli engine antivirus, mantenendo il contenuto dell'eseguibile offuscato fino al momento dell'esecuzione.

Allo stato dell'arte, un packer agisce come un incapsulatore (o protector) che altera la struttura statica del file e il suo flusso di esecuzione, operando attraverso due fasi meccaniche distinte:

1. Fase Statica (Packing Time)
Il packer disseziona il file originale e ne altera gli header (ELF o PE) per eseguire tre operazioni fondamentali:

- Cifratura e compressione: le sezioni originali (codice, dati e risorse) vengono sottoposte ad algoritmi crittografici e di compressione, trasformando opcode e dati in un blocco ad altissima entropia.

- Iniezione dello Stub: un frammento di codice assembly indipendente (lo stub o loader) viene iniettato nel file. Questo può avvenire creando una nuova sezione dedicata o sfruttando le cave di allineamento (spazi vuoti) tra i segmenti esistenti.

- Dirottamento dell'Entry Point: l'indirizzo di ingresso originale del binario (OEP - Original Entry Point) viene sovrascritto. Il nuovo Entry Point punterà direttamente allo stub, garantendo che al momento del lancio l'OS ceda il controllo a quest'ultimo.

2. Fase Dinamica (Runtime & Unpacking)
Quando il nuovo programma generato viene eseguito, questo viene caricato in memoria, compresso e cifrato, per poi essere decompresso e decifrato. Lo stub prende il controllo della CPU e orchestra il ripristino dell'eseguibile in RAM:   

- Elevazione dei privilegi di memoria: lo stub invoca le system call di gestione della memoria (es. mprotect su Linux o VirtualProtect su Windows) per alterare i flag di protezione delle pagine, passando da permessi di sola Lettura/Esecuzione (RX) a permessi di Scrittura (RWX).

- Ricostruzione del Payload: lo stub itera sulle sezioni cifrate mappate in memoria, applicando l'algoritmo inverso per decrittografarle e decomprimerle in-place.

- Risoluzione Dinamica delle Dipendenze: se l'eseguibile utilizza librerie dinamiche, lo stub deve occuparsi di ricostruire le tabelle di reindirizzamento (IAT in ambiente PE, GOT/PLT in ambiente ELF). Poiché il file su disco era cifrato, il loader del kernel non ha potuto effettuare il linking dinamico delle funzioni esterne.

- Salto all'OEP e consegna: terminata la ricostruzione, lo stub ripristina i permessi di memoria originali per rispettare i vincoli di sicurezza (W^X), cancella le proprie tracce e le chiavi dai registri, per poi eseguire un salto incondizionato (JMP/CALL) all'OEP. Il programma originale viene infine eseguito in maniera del tutto identica a come avrebbe fatto prima di passare attraverso il packer. 

# Evoluzione SOTA (State of the Art)
I packer moderni superano la semplice compressione o i cifrari lineari, implementando meccanismi di software protection concepiti per confondere le euristiche degli analisti e dei debugger:

- Code virtualization: lo stub non contiene istruzioni hardware native, ma implementa una Virtual Machine integrata con un set di istruzioni (ISA) proprietario e randomizzato per ogni processo di packing. Parti critiche del codice originale vengono tradotte in questo bytecode incomprensibile ai classici disassemblatori.

- Metamorfismo dello stub: il codice dello stub muta strutturalmente ad ogni esecuzione del packer. Utilizza l'alterazione del Control-Flow Graph (CFG), la dead-code insertion e la sostituzione polimorfica delle istruzioni (es. xor rax, rax sostituito da sub rax, rax) per impedire la creazione di firme crittografiche (signature) statiche.

- Anti-Tampering & Anti-Debugging avanzato: lo stub integra controlli ostili continui; interroga i registri hardware di debug (es. DR0-DR7), utilizza misurazioni temporali precise (es. istruzione rdtsc) per rilevare scostamenti di latenza tipici delle virtual machine o degli hypervisor, e verifica l'integrità del proprio codice in RAM per impedire il dumping della memoria decifrata.


## DEFINIZIONE E STRUTTURA DI UN ESEGUIBILE ELF

Il formato ELF (Executable and Linkable Format) è una specifica di formato dei files binari,lo standard architetturale de facto di disposizione dei dati del binario nei sistemi operativi UNIX-like (Linux, *BSD, Solaris). È definito nelle specifiche System V Application Binary Interface (si pronuncia "System Five" in riferimento alla System V Release 4 - SVR4, lo standard industriale definito da AT&T UNIX su cui si basano le moderne specifiche ELF e le ABI dei sistemi UNIX-like). I programmi che lo leggono o lo generano (compilatori, linker, kernel) sono scritti in C o C++, ma il file ELF in sé è solo una sequenza strutturata di byte grezzi su disco.I sistemi UNIX like condividono l'adesione concettuale agli standard POSIX / Single UNIX Specification: il modello di permessi basato su UID/GID, la filosofia "everything is a file" (astrazione di periferiche, socket e pipe come descrittori di file), le system call fondamentali per la gestione dei processi (fork, execve, wait) e l'adozione universale del formato ELF come standard per i binari eseguibili e le librerie condivise.

Non si tratta di un semplice contenitore di codice macchina, ma di un database relazionale strutturato che fornisce al sistema operativo e al linker dinamico tutte le topografie di memoria e i metadati necessari per caricare ed eseguire un processo, oppure per collegare moduli oggetto multipli.

Il concetto cardine per padroneggiare l'architettura ELF è il Dual View Paradigm: non si tratta di strutture fisiche separate o duplicate sul disco, ma di una duplice proiezione logica dei medesimi byte. Le stesse identiche sezioni binarie vengono lette attraverso due chiavi di accesso differenti: il linker le analizza come sezioni distinte tramite la Section Header Table (vista ad intestazioni di sezione), mentre il kernel del sistema operativo le ignora completamente e legge i medesimi byte aggregandoli in segmenti di memoria tramite la Program Header Table (vista ad intestazioni di programma).

- Linking View (vista per Sezioni): utilizzata da linker (ld), assembler e tool di analisi (es. objdump, readelf). Divide il file in Sezioni logiche (codice, dati inizializzati, tabelle di stringhe).

- Execution View (vista per Segmenti): utilizzata esclusivamente dal loader del kernel Linux (fs/binfmt_elf.c) al momento dell'esecuzione. Ignora del tutto le sezioni e divide il file in segmenti di memoria mappabili (Program Headers).

+---------------------------------------------------+
|                  ELF Header64                     |
+---------------------------------------------------+
|               Program Header Table                |
|  - PT_LOAD (Codice / Read-Exec)                   |
|  - PT_LOAD (Dati / Read-Write)                    |
|  - PT_NOTE / PT_DYNAMIC / ...                     |
+---------------------------------------------------+
|                  Sezioni del Binario              |
|  (.text, .rodata, .data, .bss, ecc.)              |
+---------------------------------------------------+
|               Section Header Table                |
+---------------------------------------------------+

Tutto in un file ELF a 64-bit fa riferimento ai tipi di base definiti in <elf.h>, come Elf64_Addr (8 byte), Elf64_Off (8 byte), Elf64_Word (4 byte), Elf64_Half (2 byte).
I tipi base della libreria <elf.h> non sono concetti astratti, ma tipi interi primitivi definiti in C tramite typedef per garantire la rigorosa portabilità cross-platform delle dimensioni (es. typedef uint64_t Elf64_Addr;, typedef uint16_t Elf64_Half;, typedef uint64_t Elf64_Off;).
I byte dell'eseguibile binario sul disco sono una sequenza continua di bit. Iniziano con l'ELF Header, poi ci sono le tabelle delle intestazioni, e infine i blocchi di dati grezzi (le istruzioni macchina del codice, le stringhe, le variabili).

Fisicamente le due Table sono array contigui di byte memorizzati nel file binario, disposti rigidamente secondo i layout geometrici delle strutture dati in C. Rappresentano blocchi di dati binari interpretati dal software in base a offset fissi.

Le tabelle (Elf64_Shdr e Elf64_Phdr) dunque non sono i dati stessi, ma gli indici descrittivi (i metadati) che dicono al software come interpretare i byte grezzi del file.

- ELF Header (Elf64_Ehdr): e' situato situato sempre all'offset 0x0 del file. È la "stele di Rosetta" che rappresenta il punto di partenza per l'analisi statica della struttura del file ,descrivendo dove trovare il resto delle tabelle. Occupa 64 byte.
  Componenti chiave dell'header:
  - e_ident[16]: i primi 4 byte sono il Magic Number (\x7F E L F). I successivi indicano la classe (32 o 64 bit), l'endianness (LSB/MSB) e la versione dell'ABI. Per esempio l'e_ident di un elf per un architettura little endian a 64 bit con ultima versione ABI assume questa configurazione hex esatta: \x7f \x45 \x4c \x46 \x02 \x01 \x01 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00.
        \x7f\x45\x4c\x46: Magic Number (\x7FELF).
        \x02: Classe (ELFCLASS64 = 64-bit).
        \x01: Endianness (ELFDATA2LSB = Little Endian).
        \x01: Versione ELF (EV_CURRENT).
        \x00: OS/ABI (ELFOSABI_SYSV).
        \x00 - \x00: Byte di padding ed estensione ABI non utilizzati.
  - e_type : definisce il ruolo del file (ET_REL = 1, ET_EXEC = 2, ET_DYN = 3, ET_CORE = 4).
  ET_EXEC indica un eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza,mentre ET_DYN (Position Independent Executable - PIE) rappresenta lo standard moderno.
  ET_CORE indica un file di dump della memoria generato dal kernel Linux quando un processo va in crash irreversibile (es. Segmentation Fault, SIGABRT). Contiene l'istantanea esatta dello stato dei registri della CPU, dello stack, dell'heap e delle aree di memoria del processo al momento della morte, impiegato dai debugger (come GDB) per l'analisi post-mortem.
  - e_machine: l'architettura target (per x86-64 è EM_X86_64 = 0x3E).Esistono decine di varianti standardizzate nelle specifiche ELF per identificare l'architettura hardware di destinazione (es. EM_386 = 3 per x86 a 32-bit, EM_X86_64 = 62 per AMD64/Intel 64, EM_ARM = 40, EM_AARCH64 = 183, EM_RISCV = 243).
  - e_entry: indirizzo virtuale (o l'offset nel caso di PIE) del punto di ingresso iniziale (OEP - Original Entry Point),cioe' della prima istruzione da cui la CPU deve iniziare l'esecuzione (spesso _start, non main).
  Se compili un programma C affidandomi alla toolchain standard della libc senza definire un entrypoint custom _start, il linker include automaticamente i moduli oggetto di avvio della libreria standard (crt1.o, crti.o, ecc.). Il simbolo _start (contenuto in crt1.o) è il vero punto d'ingresso fisico del processo: si occupa di inizializzare l'ambiente di runtime del C, invoca __libc_start_main, la quale esegue i costruttori globali e infine chiama la tua funzione main(). Di conseguenza, in questi casie_entry punta direttamente all'indirizzo del simbolo _start fornito dalla libc.
  - e_phoff: offset fisici assoluti dal byte zero in cui iniziano rispettivamente la Program Header Table.
  - e_phnum: numero di entry presenti nella Program Header Table.
  - e_shoff e e_shnum: offset e conteggio delle Section Headers.

La Program Header Table è un array separato di strutture (Elf64_Phdr) che funge da mappa per il kernel: il kernel usa questa mappa per sapere quali segmenti mappare nella memoria fisica/virtuale tramite il loader del kernel (fs/binfmt_elf.c). e con quali permessi di esecuzione.
L'Execution View e' un array di strutture (56 byte ciascuna) situate immediatamente dopo l'ELF Header.
Perché 56 e non 64 byte? Perché la somma esatta dei 7 campi che la compongono ammonta a 56 byte:
- p_type (Elf64_Word) = 4 byte
- p_flags (Elf64_Word) = 4 byte
- p_offset (Elf64_Off) = 8 byte
- p_vaddr (Elf64_Addr) = 8 byte
- p_paddr (Elf64_Addr) = 8 byte
- p_filesz (Elf64_Xword) = 8 byte
- p_memsz (Elf64_Xword) = 8 byte
- p_align (Elf64_Xword) = 8 byte 
Tutti i campi a 8 byte si trovano già su offset multipli di 8, quindi non è necessario alcun padding strutturale per portarla a 64 byte.
- p_type: il tipo di segmento. Il più importante è PT_LOAD (1), unico tipo di segmento mappato in memoria dal kernel tramite system call mmap. Un eseguibile contiene tipicamente più segmenti PT_LOAD (di solito almeno due principali nei binari standard: uno in sola lettura ed esecuzione R-X per codice e costanti, e uno in lettura e scrittura RW per dati e variabili non inizializzate). Possono essercene di più se ci sono particolari requisiti di allineamento delle pagine di memoria o segmenti di sola lettura separati (.rodata), ma lo standard per binari semplici è 2 o 3. Altri tipi critici sono PT_INTERP (contiene il percorso del linker dinamico, es. /lib64/ld-linux-x86-64.so.2) e PT_DYNAMIC (metadati per il linking a runtime).
- p_offset: l'offset in byte dell'inizio del segmento misurato rigorosamente a partire dall'inizio assoluto del file ELF su disco (offset 0x0).
- p_vaddr: indirizzo virtuale in ram a cui il segmento deve essere mappato nello spazio di indirizzamento virtuale del processo gestito dalla MMU (Memory Management Unit) del kernel.
- p_filesz: numero di bytes occupati dal segmento sul disco.
- p_memsz: i byte da riservare in RAM, cioe' la dimensione occupata dal segmento in memoria. Se p_memsz > p_filesz, la differenza viene allocata ed azzerata dal kernel: è il meccanismo con cui nasce la sezione .bss che contiene  dati non inizializzati, che non occupano spazio su disco per non sprecare byte. Le variabili globali non inizializzate o inizializzate a zero non necessitano di occupare spazio fisico sul disco rigido (non ha alcun senso salvare megabytes di zeri nel file eseguibile).
- p_flags: permessi del segmento per la Memory Management Unit (MMU)(PF_X (1), PF_W (2), PF_R (4)). Perché ogni segmento può avere permessi differenti? Nei sistemi moderni la protezione della memoria opera a livello di pagina (4 KB) tramite le tabelle delle pagine della CPU. Isolare il codice eseguibile (RX) dai dati modificabili (RW) e dalle costanti (R) impedisce vulnerabilità critiche come l'esecuzione di codice iniettato nello stack o nell'heap (implementando la regola di sicurezza W XOR X). Ogni segmento PT_LOAD possiede i suoi permessi specifici (p_flags) proprio per consentire al kernel di applicare una protezione differenziata alle varie aree della memoria del processo.
Le p_flags sono una maschera di bit combinata in OR logico: PF_X (1), PF_W (2), PF_R (4). Un segmento leggibile ed eseguibile avrà ad esempio PF_R | PF_X (4 + 1 = 5).
- p_align: granularità di allineamento, solitamente la dimensione della pagina di memoria (es. 0x1000 per 4KB o 0x200000 per le Huge Pages). p_vaddr modulo p_align deve essere uguale a p_offset modulo p_align.
Chi decide la granularità dell'allineamento? Il linker (es. ld), che si attiene ai vincoli dell'architettura hardware e del sistema operativo (solitamente la dimensione della pagina di memoria virtuale del kernel, che su x86-64 è standard a 4096 byte / 0x1000).

I Segmenti della Execution View non trovano corrispondenza nelle diverse componenti in cui e' diviso il sorgente Assembly. Nel codice assembly direttive come .data o.text servono a suddividere logicamente il codice. Il compilatore e l'assembler traducono queste direttive in sezioni all'interno del file oggetto. Il linker, successivamente, prende queste sezioni e le raggruppa in macro-blocchi chiamati segmenti (PT_LOAD), definiti in base ai permessi della MMU (es. un segmento PT_LOAD con permessi di sola lettura/esecuzione accorpa insieme .text, .rodata e altre sezioni affini).
Allo stesso modo non esiste una corrispondenza 1:1 tra i segmenti PT_LOAD della Execution View e le singole righe o sezioni del codice sorgente C o Assembly. Un segmento PT_LOAD è un macro-blocco di caricamento creato dal linker che unisce intere famiglie di sezioni in base ai permessi di accesso richiesti (es. un segmento PT_LOAD con permessi di sola lettura raggruppa .text e .rodata; un segmento con permessi di lettura/scrittura raggruppa .data e .bss).

Le sezioni in cui e' diviso un file sorgente .s (tramite direttive come .text o .data) hanno una corrispondenza diretta solo nella Linking View, ma vengono completamente appiattite e perse nella Execution View. Nella Linking View le sezioni .text scritte nel .s vengono conservate dall'assembler nel file oggetto .o e poi fuse dal linker nella Section Header Table dell'eseguibile finale. Se apro il binario con readelf -S, vedro' esattamente ogni singola sezione.
Al kernel invece non importa nulla delle sezioni. Il kernel vede solo i PT_LOAD (i segmenti). Il linker prende la sezione .text, la sezione .rodata, ecc., e le fonde brutalmente dentro un unico segmento PT_LOAD con permessi RX (Read-Execute). Di conseguenza, a livello di Program Headers (Execution View), le singole sezioni del sorgente scompaiono come entità distinte: esistono solo blocchi continui di memoria gestiti dalla MMU.

La Section Header Table , generalmente posta alla fine del file fisico, è un array di strutture (Elf64_Shdr) che funge da mappa logica: il linker usa questa mappa per sapere cosa sta manipolando. Il kernel invece ignora completamente questa tabella; serve a GDB, objdump o a un packer per mappare il significato logico dei byte. Ogni header descrive una Sezione. Un segmento PT_LOAD è quasi sempre un aggregato di più sezioni contigue.

Sezioni fondamentali:
- .text: il codice assembly eseguibile (mappato nel PT_LOAD con permessi RX).
- .rodata: dati di sola lettura, come costanti ,literal string o tabelle di salto degli switch (permessi R).
- .data: variabili globali o statiche inizializzate a valori non nulli (mappato nel PT_LOAD con permessi RW).
- .bss (Block Started by Symbol): variabili globali inizializzate a zero. Fisicamente sul disco, nel file eseguibile, non occupa spazio per i dati non inizializzati (ha p_filesz = 0 o comunque non alloca i byte dei dati nel file). Esiste solo come metadato descrittivo (p_memsz > p_filesz). Quando il kernel mappa il segmento in memoria tramite mmap, legge la differenza tra p_memsz e p_filesz e alloca ed azzera fisicamente blocchi di RAM (zero-fill pages). Quindi sul disco è solo un'istruzione di dimensione per la MMU, mentre in RAM diventa materia effettiva.
- .symtab (Symbol Table): una tabella che elenca tutti i simboli definiti o referenziati nel modulo (funzioni, variabili globali), associandoli al loro indirizzo o offset e dimensione. La symbol table non memorizza i nomi per intero, ma punta semplicemente all'offset numerico dei caratteri all'interno della .strtab per ottimizzare lo spazio.
- .strtab(String Table): una tabella di stringhe testuali in formato zero-terminated che contiene i nomi reali dei simboli (es. "main", "printf"). . Nei binari di produzione symtab e strtab sono spesso rimosse (stripped) per offuscare i nomi delle funzioni e ridurre le dimensioni.


# GCC VS CC VS CLANG
- gcc (GNU Compiler Collection): e' lo storico compilatore di riferimento del mondo GNU/Linux. Supporta molteplici linguaggi (C, C++, Fortran) e utilizza un backend basato su rappresentazioni intermedie (RTL e GIMPLE).
- clang: e' il frontend nativo per l'infrastruttura di compilazione LLVM, scritto in C++, celebre per la modularità, la pulizia dei messaggi d'errore e la velocità di elaborazione.
- cc (C Compiler): e' tradizionalmente un symlink o un wrapper POSIX standard che punta al compilatore di sistema predefinito (storicamente gcc, mentre nei sistemi moderni o su macOS viene spesso rimappato direttamente su clang).
- GNU Binutils: un pacchetto software fondamentale di utility di basso livello per la manipolazione di binari e file oggetto. Include strumenti chiave come as (l'assembler), ld (il linker), objdump (disassemblatore), readelf (analizzatore di strutture ELF), nm (analizzatore di simboli), strip (rimozione dei metadati di debug) e ar (gestore di archivi statici .a).


## PIPELINE DI GENERAZIONE DI UN ESEGUIBILE ELF: DA C AL BINARIO
La trasformazione di un sorgente in un ELF pronto per l'esecuzione avviene tramite la toolchain standard (es. GCC/Clang + GNU Binutils) che si occupa di tutte le fasi della pipeline di generazione richiamando internamente i singoli moduli (preprocessor, parser LLVM/GCC, assembler e linker).

1) Compilazione (Frontend & Backend): il codice C viene parsato (AST), ottimizzato tramite rappresentazioni intermedie (GIMPLE, RTL) e tradotto in assembly x86-64 puro.Possiamo identificare almeno 3 fasi differenti:
- Preprocessing (cpp): risoluzione delle macro (#define) e inclusione degli header (#include).
- Compilazione (Frontend): analisi lessicale, sintattica e semantica del codice C, traduzione in un Abstract Syntax Tree (AST) e generazione della rappresentazione intermedia (IR),come Gimple e RTL.Gimple e' un'IR ad alto livello basata su alberi semplificati e codice a tre indirizzi (3-address code), usata per applicare ottimizzazioni indipendenti dalla CPU (es. loop unrolling, rimozione del codice morto). RTL (Register Transfer Language) e' un'IR a basso livello simile a un assembly virtuale con un numero infinito di registri virtuali, usata dal backend per tradurre il codice nell'architettura hardware specifica (es. x86-64).
Entrambe sono strutture dati in memoria scritte interamente in C (il linguaggio con cui è implementato il sorgente di GCC).
- Ottimizzazione e generazione del codice (Backend): elaborazione delle ottimizzazioni indipendenti e dipendenti dalla CPU e traduzione finale in codice Assembly specifico per l'architettura target.

2) Assemblaggio (as): l'assembler traduce le mnemoniche in opcode binari; l'output è un file oggetto rilocabile 
(.o in Linux), un ELF di tipo ET_REL. Un ET_REL generato dall'assembler contiene già tutte le mnemoniche tradotte in opcode macchina per le istruzioni locali del file sorgente, ma non possiede gli indirizzi di memoria definitivi per qualsiasi riferimento esterno (variabili globali esterne, funzioni importate tramite extern, o chiamate a librerie). Per questi simboli, l'assembler inserisce placeholder azzerati come offset 0x0 e compila una tabella di rilocazione (.rel.text o .rela.text) che istruisce il linker su come calcolare e patchare quegli indirizzi in futuro.Un file ET_REL è costituito interamente da sezioni separate (es. .text, .data, .bss, .rela.text). Il linker esiste proprio per prendere queste sezioni separate da più file .o e fonderle insieme.
L'ET_REL inizia fisicamente con l'ELF Header (Elf64_Ehdr) impostato con e_type = ET_REL, seguito immediatamente dai dati grezzi delle sezioni (codice macchina, dati) e si conclude con la Section Header Table (fondamentale per i file oggetto, poiché il linker lavora esclusivamente con la vista a sezioni). Non possiede una Program Header Table (PHDR), poiché i file oggetto non vengono mai caricati direttamente in memoria dal kernel.

3) Linking Statico (ld): il linker prende in pasto multipli ET_REL .o (e archivi .a). Unisce le sezioni omologhe (tutte le .text insieme, tutte le .data insieme), risolve i simboli incrociati e applica le formule matematiche definite nelle rilocazioni per assegnare gli indirizzi di memoria virtuale definitivi.

4) Generazione dell'eseguibile: il risultato è un ELF di tipo ET_EXEC (eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza) o ET_DYN (Position Independent Executable - PIE, lo standard moderno). In un ET_DYN, gli indirizzi generati dal linker non sono assoluti, ma relativi al registro RIP, permettendo al kernel di caricare il binario in un punto casuale della RAM (ASLR - Address Space Layout Randomization)

Cosa accade in caso di compilazione temporanea di piu' sorgenti .c?L'assembler genera un file .o distinto per ciascuno di essi. Ciascun file .o possiede le proprie sezioni .text, .data, ecc. Il linker (ld) prende tutte le sezioni omologhe di tutti i file oggetto e le fonde insieme in macro-sezioni uniche nell'eseguibile finale (tutte le `.text` di tutti i file `.o` vengono concatenate in un'unica grande `.text` continua). Questa macro-sezione viene poi inserita nel rispettivo segmento `PT_LOAD`.


## COSA ACCADE QUANDO ESEGUO UN ESEGUIBILE BINARIO
La shell esegue un fork() e poi chiama execve sull'eseguibile("./a.out", ...).Il kernel apre il file, legge i primi byte e identifica il magic number \x7fELF. L'esecuzione passa all'handler load_elf_binary() all'interno del kernel source (fs/binfmt_elf.c). Load_elf_binary() e' la funzione cardine del sottosistema binario del kernel Linux (fs/binfmt_elf.c). Quando lancio un eseguibile, il kernel invoca questa funzione di bootstrap e parsing per leggere l'header ELF, allocare le strutture di task (mm_struct), mappare i segmenti PT_LOAD tramite la system call interna di mmap, configurare lo stack iniziale del processo e preparare il salto all'entry point.
Il kernel itera l'array dei Program Headers(mapping): per ogni PT_LOAD trovato, invoca un mmap() per copiare/associare quel blocco del file fisico nelle pagine di RAM del nuovo processo(Segmentazione), applicando i bit di protezione MMU specificati in p_flags (RX per il codice, RW per i dati).

Quale e' la differenza tra un binario statico e un binario dinamico? Il binario statico non contiene segmenti PT_INTERP. Il kernel imposta il registro RSP (lo stack con argv ed envp) e salta direttamente all'indirizzo e_entry letto dall'ELF Header. In caso di binario dinamico(SOTA per Linux) esiste un segmento PT_INTERP che punta a ld-linux.so. Il kernel mappa in memoria non solo il mio eseguibile, ma anche l'interprete (il linker dinamico). Invece di saltare all'e_entry del mio file, il kernel salta all'e_entry del linker dinamico.
C è un linguaggio compilato in codice macchina nativo e non ha bisogno di un interprete nel senso di una macchina virtuale (come Python o Java). Tuttavia, i moderni eseguibili Linux sono quasi sempre dinamici, ovvero dipendono da librerie condivise esterne (come la libc.so.6). Per consentire a un eseguibile di risolvere tali dipendenze a runtime, il file ELF incorpora un puntatore a un componente esterno: il linker dinamico (/lib64/ld-linux-x86-64.so.2). Nelle specifiche ELF, questo componente viene formalmente denominato Interprete (PT_INTERP). Di conseguenza, quando eseguo un binario dinamico, il kernel mappa in memoria sia l' eseguibile sia il linker dinamico, passando inizialmente il controllo a quest'ultimo affinché risolva i simboli e prepari l'ambiente, prima di cedere definitivamente l'esecuzione al punto d'ingresso del mio programma.
Il linker dinamico(ld.so) si attiva a runtime, analizza il segmento PT_DYNAMIC del tuo eseguibile, mappa in memoria tutte le librerie condivise necessarie (es. libc.so.6), risolve i simboli essenziali, popola la GOT (Global Offset Table) e la PLT (Procedure Linkage Table) per consentire le chiamate a funzioni esterne, e infine cede il controllo saltando all' e_entry (che solitamente punta a _start, la quale inizializza la libc e infine chiama main()). GOT (Global Offset Table)e' una tabella di puntatori situata nel segmento dati (RW) che contiene gli indirizzi di memoria effettivi delle funzioni esterne e delle variabili globali, risolti a runtime dal linker dinamico. PLT (Procedure Linkage Table) e' invece una tabella di stub di codice eseguibile (RX) che permette al codice di saltare verso funzioni esterne (es. printf). Poiché con i binari PIE e l'ASLR gli indirizzi cambiano a ogni avvio, la PLT funge da tramite: interroga la GOT; se la funzione non è ancora stata risolta, attiva il linker dinamico (ld.so) per risolverla al primo giro (lazy binding), memorizzando poi l'indirizzo nella GOT per tutte le chiamate successive.
Nei binari moderni Position Independent Executable (ET_DYN), gli indirizzi risolti dal linker non sono assoluti (fissi), ma relativi al registro RIP (Instruction Pointer). Quando si parla di indirizzamento relativo al RIP, si riferimento all'istruzione attualmente in esecuzione. Poiché il kernel carica il binario in un punto casuale della RAM (ASLR), il registro RIP punta dinamicamente a quell'indirizzo base effettivo; di conseguenza, il codice calcola gli offset interni sommando o sottraendo la distanza relativa rispetto alla posizione corrente del RIP, garantendo il funzionamento del programma indipendentemente dalla locazione fisica in memoria.
Il motivo per cui funziona è esattamente perché il RIP cambia in continuazione. L'indirizzamento RIP-relative non usa indirizzi assoluti (fissi), ma spostamenti relativi (offset) calcolati in base alla posizione corrente del processore.
ESEMPIO:
Se scrivo un'istruzione che deve leggere una variabile o saltare a una funzione vicina in un binario PIE il compilatore non genera un'istruzione con un indirizzo fisso del tipo mov rax, [0x401050], piuttosto una del tipo mov rax, [rip + 0x1234]. A runtime, la CPU esegue l'istruzione: prende il valore attuale del registro RIP (che punta all'istruzione successiva) e ci somma l'offset costante 0x1234.
Poiché l'offset in byte tra l'istruzione e la variabile è matematicamente fisso (indipendentemente da dove il kernel decide di caricare il programma in RAM grazie all'ASLR), la somma RIP + offset punterà sempre esattamente al bersaglio corretto. Il RIP cambia ad ogni ciclo di clock, ma la distanza relativa tra le parti del codice rimane immutata.

Linker statico e dinamico sono dunque due software completamente distinti che operano in momenti temporali e contesti operativi opposti: 
- lo statico (ld / GNU Binutils) e' un tool di sviluppo che gira sulla macchina di compilazione (offline).
Prende i file oggetto .o generati dall'assembler, risolve i simboli interni, calcola le rilocazioni e assembla il file ELF finale sul disco. Una volta terminata la compilazione, il compito di ld è esaurito. Il binario è pronto.
- il linker Dinamico (ld-linux-x86-64.so.2 / ld.so) e' una libreria di sistema (un eseguibile condiviso) che risiede fisicamente sul sistema operativo target (es. in /lib64/). Entra in azione a runtime: nel momento esatto in cui eseguo un binario dinamico, il kernel Linux legge l'header PT_INTERP, scopre che il binario ha bisogno di ld.so, mappa in memoria sia il tuo programma sia il linker dinamico, e cede inizialmente il controllo a quest'ultimo. ld.so si occupa di aprire le librerie condivise (.so), caricarle in RAM, risolvere i simboli esterni mancanti (popolando la GOT) e solo allora avviare l'esecuzione del tuo codice.

Che differenza c'e' allora tra funzione extern e funzione condivisa (dynamic)? extern` è una keyword di dichiarazione logica a livello di linguaggio C e Assembly che dice al compilatore di non generare un indirizzo definitivo per quel simbolo, lasciando un segnaposto per il linker. La funzione condivisa è l'entità binaria reale che risiede all'interno di una libreria esterna (.so). Quando dichiaro una funzione extern e questa appartiene a una libreria esterna (es. printf della libc), il linker inserisce una voce nella PLT/GOT, demandando al linker dinamico (ld-linux.so) il compito di risolverla a runtime agganciando l'indirizzo reale.








---

### 4. Toolchain, Compilazione e Rappresentazioni Intermedie

* **`ld` fa parte sia di GCC che delle GNU Binutils?** `ld` appartiene ufficialmente alle **GNU Binutils** (il pacchetto indipendente di tool di basso livello). GCC non include il codice sorgente di `ld` al suo interno, ma lo invoca esternamente come fase finale (il "linking driver") per produrre l'eseguibile.



* **La tabella di rilocazione (`ET_REL`) è un file di testo con gli offset?** **Assolutamente no.** Non è un file di testo leggibile. È una struttura dati binaria strutturata (un array di struct `Elf64_Rela`) incorporata all'interno del file oggetto binario.
* **Ogni file sorgente genera un file oggetto rilocabile?** Sì, ogni volta che un compilatore traduce un file sorgente con l'opzione `-c`, produce esattamente un file oggetto rilocabile (`.o` / `ET_REL`).

---

### 5. Tipi Base e Anatomia dei Byte ELF

* **I tipi base ELF sono basicamente `uint64`, `uint32`, `uint16`?** Esatto. Sono `typedef` primitivi in C (`uint64_t`, `uint32_t`, `uint16_t`, ecc.) ridefiniti in `<elf.h>` con nomi specifici dell'ABI (es. `Elf64_Addr`, `Elf64_Off`, `Elf64_Word`, `Elf64_Half`) per garantire la portabilità rigorosa delle dimensioni su qualsiasi architettura host.
* **Perché il primo byte del magic number è esattamente `127` (`\x7f`)?** Perché storicamente (nel design dei vecchi sistemi Unix e nei loader `a.out`/ELF) il valore `0x7F` non è un carattere ASCII stampabile (corrisponde al carattere `DEL`), scelto appositamente per impedire che il sistema tentasse di interpretare ed eseguire file di testo ASCII o binari corrotti come script di shell, bloccando immediatamente l'esecuzione prima di analizzare i caratteri alfabetici successivi (`E`, `L`, `F`).
* **`Elf64_Phdr` è la singola struttura o l'array?** `Elf64_Phdr` è la **definizione della singola struttura** (il singolo record di 56 byte). La Program Header Table è un **array contiguo** di queste strutture. Ciascuna di esse possiede il suo campo `p_type` (che specifica se si tratta di `PT_LOAD`, `PT_DYNAMIC`, `PT_NOTE`, ecc.).
* **Gli offset sul disco sono sempre calcolati a partire dal byte `0x0`?** Sì. Tutti gli offset nei file ELF (`e_phoff`, `e_shoff`, `p_offset`) sono **offset assoluti in byte** misurati rigorosamente a partire dall'inizio fisico del file sul disco (byte `0x0`).


---

### 6. Hardware e Kernel (`MMU`, `AGU`, `binfmt_elf.c`)

* **C'è differenza tra MMU e AGU?**
* **MMU (Memory Management Unit):** L'unità hardware della CPU che traduce gli indirizzi virtuali in indirizzi fisici tramite le tabelle delle pagine (Page Tables) e applica i bit di protezione della memoria (`RWX`).
* **AGU (Address Generation Unit):** L'unità hardware dedicata esclusivamente al calcolo degli indirizzi di memoria complessi per le istruzioni di load/store (es. basati su registri base, indice e scala: `[rax + rbx*4 + 0x123]`), sgravando le ALU principali.

* **`fs/binfmt_elf.c` è il sorgente del kernel con `load_elf_binary()`?** Esatto al 100%. È il file sorgente del kernel Linux che implementa il gestore del formato ELF, contenendo la funzione cardine `load_elf_binary()` che parsa gli header, alloca la memoria e invoca le `mmap` necessarie.
* **Sia i program che i section header fanno riferimento al file su disco?** Sì. Entrambe le tabelle contengono offset (`p_offset` per i segmenti, `sh_offset` per le sezioni) che puntano a posizioni fisiche precise all'interno del file binario memorizzato sul disco.

---

### 7. Linker Dinamico, Shared Libraries, GOT e PLT

* **`ld-linux.so` fa parte del codice sorgente del kernel?** No. È un eseguibile condiviso (userspace) che risiede sul filesystem (es. in `/lib64/ld-linux-x86-64.so.2`).
* **Cosa sta per `so`?** **Shared Object** (Oggetto Condiviso).
* **Cosa sono le librerie condivise?** File binari contenenti funzioni precompilate (come la `libc`) che non vengono incorporate staticamente nel codice dell'eseguibile, ma vengono mappate una sola volta in memoria RAM e condivise contemporaneamente tra tutti i processi in esecuzione che ne hanno bisogno.
* **GOT e PLT fanno parte dell'eseguibile? Sono su disco o in RAM? Sono process-specific?**
* *Fanno parte dell'eseguibile?* Sì, strutturalmente l'eseguibile contiene sezioni dedicate (`.plt` e `.got` / `.got.plt`) generate dal linker.
* *Disco o RAM?* Esistono come dati sul **disco** (nei segmenti `PT_LOAD`) e vengono caricati direttamente in **RAM** dal kernel al momento dell'esecuzione.
* *Process-specific?* Sì, ogni processo ha il suo spazio di indirizzamento virtuale isolato, quindi possiede le sue istanze private di GOT e PLT in RAM (sebbene le pagine delle librerie condivise sottostanti siano mappate in condivisione fisica a livello di kernel).



---

### 8. Associazioni tra Sezioni nei PT_LOAD e Concetti Correlati

* **Che associazioni ci sono tra sezioni dei file oggetto che finiscono negli stessi `PT_LOAD`?** Il linker raggruppa le sezioni in base alla **compatibilità dei permessi di memoria** definiti dalle specifiche ELF:
* Sezioni con permessi di sola lettura ed esecuzione (`.init`, `.text`, `.fini`, `.rodata`) confluiscono nel segmento `PT_LOAD` con flag `PF_R | PF_X`.
* Sezioni con permessi di lettura e scrittura (`.data`, `.bss`, `.init_array`, `.fini_array`, `.got`) confluiscono nel segmento `PT_LOAD` con flag `PF_R | PF_W`.
* *Questo vale anche per sezioni di file sorgente diversi?* **Sì, assolutamente.** Tutte le `.text` di tutti i file `.o` differenti vengono fuse insieme in un'unica grande sezione `.text` continua, che finisce nello stesso `PT_LOAD` eseguibile.





* **Cosa si intende per "dati grezzi" delle sezioni?** Sono i **byte binari puri** (gli opcode delle istruzioni macchina o i valori binari grezzi delle variabili/costanti) memorizzati nel file, privi di sovrastrutture di metadati, ma semplicemente puntati dagli offset delle tabelle.
* **Qual è la sequenza strutturale fisica esatta di un file ELF su disco?**
1. **ELF Header** (`Elf64_Ehdr`) (offset `0x0`, 64 byte fissi).
2. **Program Header Table** (`Elf64_Phdr` array).
3. **Sezioni Grezze / Dati dei Segmenti** (il corpo principale del binario: codice `.text`, dati `.data`, costanti `.rodata`, ecc.).
4. **Section Header Table** (`Elf64_Shdr` array) (solitamente posizionata in coda al file, sebbene il kernel la ignori completamente e si affidi esclusivamente alla Program Header Table).