## COSA E' UN PACKER

Un packer è uno strumento di manipolazione binaria post-compilazione che riceve in input un programma eseguibile (.exe, .dll, ELF) e genera un nuovo binario in cui il payload originale viene compresso e cifrato simultaneamente. La sua esistenza deriva dalla necessità di eludere l'analisi in memoria degli engine antivirus, mantenendo il contenuto dell'eseguibile offuscato fino al momento dell'esecuzione.

Allo stato dell'arte, un packer agisce come un incapsulatore (o protector) che altera la struttura statica del file e il suo flusso di esecuzione, operando attraverso due fasi meccaniche distinte:

1. Fase Statica (Packing Time)
Il packer disseziona il file originale e ne altera gli header (ELF o PE) per eseguire tre operazioni fondamentali:

Cifratura e Compressione: Le sezioni originali (codice, dati e risorse) vengono sottoposte ad algoritmi crittografici e di compressione, trasformando opcode e dati in un blocco ad altissima entropia.

Iniezione dello Stub: Un frammento di codice assembly indipendente (lo stub o loader) viene iniettato nel file. Questo può avvenire creando una nuova sezione dedicata o sfruttando le cave di allineamento (spazi vuoti) tra i segmenti esistenti.

Dirottamento dell'Entry Point: L'indirizzo di ingresso originale del binario (OEP - Original Entry Point) viene sovrascritto. Il nuovo Entry Point punterà direttamente allo stub, garantendo che al momento del lancio l'OS ceda il controllo a quest'ultimo.

2. Fase Dinamica (Runtime & Unpacking)
Quando il nuovo programma generato viene eseguito, questo viene caricato in memoria, compresso e cifrato, per poi essere decompresso e decifrato. Lo stub prende il controllo della CPU e orchestra il ripristino dell'eseguibile in RAM:   Elevazione dei Privilegi di Memoria: Lo stub invoca le system call di gestione della memoria (es. mprotect su Linux o VirtualProtect su Windows) per alterare i flag di protezione delle pagine, passando da permessi di sola Lettura/Esecuzione (RX) a permessi di Scrittura (RWX).Ricostruzione del Payload: Lo stub itera sulle sezioni cifrate mappate in memoria, applicando l'algoritmo inverso per decrittografarle e decomprimerle in-place.Risoluzione Dinamica delle Dipendenze: Se l'eseguibile utilizza librerie dinamiche, lo stub deve occuparsi di ricostruire le tabelle di reindirizzamento (IAT in ambiente PE, GOT/PLT in ambiente ELF). Poiché il file su disco era cifrato, il loader del kernel non ha potuto effettuare il linking dinamico delle funzioni esterne.Salto all'OEP e Consegna: Terminata la ricostruzione, lo stub ripristina i permessi di memoria originali per rispettare i vincoli di sicurezza (W^X), cancella le proprie tracce e le chiavi dai registri, per poi eseguire un salto incondizionato (JMP/CALL) all'OEP. Il programma originale viene infine eseguito in maniera del tutto identica a come avrebbe fatto prima di passare attraverso il packer. 

Evoluzione SOTA (State of the Art)
I packer moderni superano la semplice compressione o i cifrari lineari, implementando meccanismi di software protection concepiti per distruggere le euristiche degli analisti e dei debugger:

Code Virtualization: Lo stub non contiene istruzioni hardware native, ma implementa una Virtual Machine integrata con un set di istruzioni (ISA) proprietario e randomizzato per ogni processo di packing. Parti critiche del codice originale vengono tradotte in questo bytecode incomprensibile ai classici disassemblatori.

Metamorfismo dello Stub: Il codice dello stub muta strutturalmente ad ogni esecuzione del packer. Utilizza l'alterazione del Control-Flow Graph (CFG), la dead-code insertion e la sostituzione polimorfica delle istruzioni (es. xor rax, rax sostituito da sub rax, rax) per impedire la creazione di firme crittografiche (signature) statiche.

Anti-Tampering & Anti-Debugging Avanzato: Lo stub integra controlli ostili continui: interroga i registri hardware di debug (es. DR0-DR7), utilizza misurazioni temporali precise (es. istruzione rdtsc) per rilevare scostamenti di latenza tipici delle virtual machine o degli hypervisor, e verifica l'integrità del proprio codice in RAM per impedire il dumping della memoria decifrata.


## DEFINIZIONE E STRUTTURA DI UN ESEGUIBILE ELF
Fisicamente in che linguaggio è scritto l'ELF? L'ELF non è scritto in alcun linguaggio: è una specifica di formato file binario (uno standard architetturale di disposizione dei dati). I programmi che lo leggono o lo generano (compilatori, linker, kernel) sono scritti in C o C++, ma il file ELF in sé è solo una sequenza strutturata di byte grezzi su disco.
Il formato ELF (Executable and Linkable Format) è lo standard architetturale de facto per i file binari nei sistemi operativi UNIX-like (Linux, *BSD, Solaris). È definito nelle specifiche System V Application Binary Interface (ABI). Si pronuncia "System Five" (fa riferimento alla System V Release 4 - SVR4, lo standard industriale definito da AT&T UNIX su cui si basano le moderne specifiche ELF e le ABI dei sistemi UNIX-like).

Non si tratta di un semplice contenitore di codice macchina, ma di un database relazionale strutturato che fornisce al sistema operativo e al linker dinamico tutte le topografie di memoria e i metadati necessari per caricare ed eseguire un processo, oppure per collegare moduli oggetto multipli.

Il concetto cardine per padroneggiare l'architettura ELF è il Dual View Paradigm.Dual View Paradigm (Strutture fisiche vs logiche): Non si tratta di strutture fisiche separate o duplicate sul disco, ma di una duplice proiezione logica dei medesimi byte. Le stesse identiche sezioni binarie vengono lette attraverso due chiavi di accesso differenti: il linker le analizza come sezioni distinte tramite la Section Header Table, mentre il kernel del sistema operativo le ignora completamente e legge i medesimi byte aggregandoli in segmenti di memoria tramite la Program Header Table. Un file ELF possiede due strutture sovrapposte, lette da attori diversi in momenti diversi:

Linking View (Vista per Sezioni): Utilizzata da linker (ld), assembler e tool di analisi (es. objdump, readelf). Divide il file in Sezioni logiche (codice, dati inizializzati, tabelle di stringhe).

Execution View (Vista per Segmenti): Utilizzata esclusivamente dal loader del kernel Linux (fs/binfmt_elf.c) al momento dell'esecuzione. Ignora del tutto le sezioni e divide il file in Segmenti di memoria mappabili (Program Headers).
Occorre comprendere la differenza tra la vista ad intestazioni di sezione (Section Headers, usate da linker e debugger) e la vista ad intestazioni di programma (Program Headers, usata dall'loader del kernel Linux).

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

Fisicamente cosa sono le tabelle e in che linguaggio sono scritte? Fisicamente sono array contigui di byte memorizzati nel file binario, disposti rigidamente secondo i layout geometrici delle strutture dati in C. Non sono "scritte in un linguaggio", ma rappresentano blocchi di dati binari interpretati dal software in base a offset fissi.
1. Le tabelle e i byte: cosa significa "tramite"?
Le tabelle (Elf64_Shdr e Elf64_Phdr) non sono i dati stessi, ma gli indici descrittivi (i metadati) che dicono al software come interpretare i byte grezzi del file.

I byte sul disco sono una sequenza continua di bit. Iniziano con l'ELF Header, poi ci sono le tabelle delle intestazioni, e infine i blocchi di dati grezzi (le istruzioni macchina del codice, le stringhe, le variabili).

La Section Header Table è un array di strutture (Elf64_Shdr) che funge da mappa logica: dice "I byte che vanno dall'offset 0x1000 all'offset 0x2000 formano la sezione .text". Il linker usa questa mappa per sapere cosa sta manipolando.

La Program Header Table è un array separato di strutture (Elf64_Phdr) che funge da mappa per il kernel: dice "Prendi i byte dall'offset 0x1000 al 0x3000 del file e copiali in RAM all'indirizzo virtuale 0x400000 con permessi di sola esecuzione".

Quindi dire "tramite" significa che il parser (il linker o il kernel) legge i metadati della rispettiva tabella per capire dove andare a pescare, interpretare o mappare i byte del file. I byte del codice sono sempre gli stessi, ma la Section Table li seziona per nome e funzione logica (per il debug/linking), mentre la Program Table li accorpa in macro-blocchi continui (per la MMU del kernel).

Segmenti della Execution View vs Codice Sorgente Assembly (.s): Non coincidono. Nel codice assembly scrivi direttive come .section .text o .section .data per suddividere logicamente il codice. Il compilatore e l'assembler traducono queste direttive in sezioni all'interno del file oggetto. Il linker, successivamente, prende queste sezioni e le raggruppa in macro-blocchi chiamati segmenti (PT_LOAD), definiti in base ai permessi della MMU (es. un segmento PT_LOAD con permessi di sola lettura/esecuzione accorpa insieme .text, .rodata e altre sezioni affini).
Corrispondenza dei PT_LOAD nel sorgente C o Assembly: Non esiste una corrispondenza 1:1 con singole righe o sezioni del codice sorgente. Un segmento PT_LOAD è un macro-blocco di caricamento creato dal linker che unisce intere famiglie di sezioni in base ai permessi di accesso richiesti (es. un segmento PT_LOAD con permessi di sola lettura raggruppa .text e .rodata; un segmento con permessi di lettura/scrittura raggruppa .data e .bss).


2. La corrispondenza delle sezioni del .s nelle due viste
Le sezioni in cui dividi un file sorgente .s (tramite direttive come .section .text, .section .data) hanno una corrispondenza diretta nella Linking View, ma vengono completamente appiattite e perse nella Execution View.

Nella Linking View (Sezioni): Le tue sezioni .text scritte nel .s vengono conservate dall'assembler nel file oggetto .o e poi fuse dal linker nella Section Header Table dell'eseguibile finale. Se apri il binario con readelf -S, vedi esattamente ogni singola sezione.

Nella Execution View (Segmenti): Al kernel non importa nulla delle sezioni. Il kernel vede solo i PT_LOAD (i segmenti). Il linker prende la sezione .text, la sezione .rodata, ecc., e le fonde brutalmente dentro un unico segmento PT_LOAD con permessi RX (Read-Execute). Di conseguenza, a livello di Program Headers (Execution View), le singole sezioni del tuo sorgente scompaiono come entità distinte: esistono solo blocchi continui di memoria gestiti dalla MMU.



clang vs gcc vs cc:

gcc (GNU Compiler Collection): È lo storico compilatore di riferimento del mondo GNU/Linux. Supporta molteplici linguaggi (C, C++, Fortran) e utilizza un backend basato su rappresentazioni intermedie (RTL e GIMPLE).

clang: È il frontend nativo per l'infrastruttura di compilazione LLVM, scritto in C++, celebre per la modularità, la pulizia dei messaggi d'errore e la velocità di elaborazione.

cc (C Compiler): È tradizionalmente un symlink o un wrapper POSIX standard che punta al compilatore di sistema predefinito (storicamente gcc, mentre nei sistemi moderni o su macOS viene spesso rimappato direttamente su clang).

Cosa sono le GNU Binutils: Un pacchetto software fondamentale di utility di basso livello per la manipolazione di binari e file oggetto. Include strumenti chiave come as (l'assembler), ld (il linker), objdump (disassemblatore), readelf (analizzatore di strutture ELF), nm (analizzatore di simboli), strip (rimozione dei metadati di debug) e ar (gestore di archivi statici .a).

Tutto in un file ELF a 64-bit fa riferimento ai tipi di base definiti in <elf.h>, come Elf64_Addr (8 byte), Elf64_Off (8 byte), Elf64_Word (4 byte), Elf64_Half (2 byte).
Tipi base nella libreria <elf.h>: Non sono concetti astratti, ma tipi interi primitivi definiti in C tramite typedef per garantire la rigorosa portabilità cross-platform delle dimensioni (es. typedef uint64_t Elf64_Addr;, typedef uint16_t Elf64_Half;, typedef uint64_t Elf64_Off;).

ELF Header (Elf64_Ehdr): e' situato situato sempre all'offset 0x0 del file. È la "stele di Rosetta" che descrive dove trovare il resto delle tabelle. Occupa 64 byte.
ELF Header (Elf64_Ehdr): rappresenta il punto di partenza per l'analisi statica della struttura del file. Campi chiave da gestire:
- e_entry: Indirizzo virtuale del punto di ingresso iniziale (OEP - Original Entry Point).   
- e_phoff: Offset nel file in cui inizia la Program Header Table.
- e_phnum: Numero di entry presenti nella Program Header Table.
- e_shoff e e_shnum: Offset e conteggio delle Section Headers.


e_ident[16]: I primi 4 byte sono il Magic Number (\x7F E L F). I successivi indicano la classe (32 o 64 bit), l'endianness (LSB/MSB) e la versione dell'ABI.
Esempio di e_ident per un sistema a 64-bit, Little Endian (LSB) e ultima versione ABI:

I primi 16 byte (e_ident) assumono questa configurazione esadecimale esatta:
\x7f \x45 \x4c \x46 \x02 \x01 \x01 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00

\x7f\x45\x4c\x46: Magic Number (\x7FELF).

\x02: Classe (ELFCLASS64 = 64-bit).

\x01: Endianness (ELFDATA2LSB = Little Endian).

\x01: Versione ELF (EV_CURRENT).

\x00: OS/ABI (ELFOSABI_SYSV).

\x00 - \x00: Byte di padding ed estensione ABI non utilizzati.



e_type: Definisce il ruolo del file (ET_REL = 1, ET_EXEC = 2, ET_DYN = 3, ET_CORE = 4).
Cos'è ET_CORE? È un file di dump della memoria generato dal kernel Linux quando un processo va in crash irreversibile (es. Segmentation Fault, SIGABRT). Contiene l'istantanea esatta dello stato dei registri della CPU, dello stack, dell'heap e delle aree di memoria del processo al momento della morte, impiegato dai debugger (come GDB) per l'analisi post-mortem.

e_machine: L'architettura target (per x86-64 è EM_X86_64 = 0x3E).
Quanti tipi di e_machine esistono? Decine di varianti standardizzate nelle specifiche ELF per identificare l'architettura hardware di destinazione (es. EM_386 = 3 per x86 a 32-bit, EM_X86_64 = 62 per AMD64/Intel 64, EM_ARM = 40, EM_AARCH64 = 183, EM_RISCV = 243).



e_entry: L'indirizzo virtuale (o l'offset nel caso di PIE) della prima istruzione da cui la CPU deve iniziare l'esecuzione (spesso _start, non main).
e_entry nel caso di compilazione standard con GCC senza definire _start manualmente: Esatto. Se compili un programma C affidandoti alla toolchain standard senza definire un entrypoint custom, il linker include automaticamente i moduli oggetto di avvio della libreria standard (crt1.o, crti.o, ecc.). Il simbolo _start (contenuto in crt1.o) è il vero punto d'ingresso fisico del processo: si occupa di inizializzare l'ambiente di runtime del C, invoca __libc_start_main, la quale esegue i costruttori globali e infine chiama la tua funzione main(). Di conseguenza, e_entry punta direttamente all'indirizzo del simbolo _start fornito dalla libc.

e_phoff / e_shoff: Offset fisici assoluti dal byte zero in cui iniziano rispettivamente la Program Header Table e la Section Header Table.

e_phnum / e_shnum: Il numero di entry contenute nelle suddette tabelle.




Program Headers (Elf64_Phdr): descrivono i segmenti da mappare in memoria fisica/virtuale tramite l'loader del kernel (fs/binfmt_elf.c).
- PT_LOAD: unico segmento caricato in memoria dal kernel.
- p_offset: offset del segmento all'interno del file su disco.
A cosa fa riferimento p_offset e rispetto a cosa? È l'offset in byte dell'inizio del segmento misurato rigorosamente a partire dall'inizio assoluto del file ELF su disco (offset 0x0).
- p_vaddr: indirizzo virtuale a cui il segmento deve essere mappato in memoria. vaddr è un indirizzo in RAM? Sì, è il Virtual Address (indirizzo virtuale) a cui quel segmento deve essere proiettato nello spazio di indirizzamento virtuale del processo gestito dalla MMU (Memory Management Unit) del kernel.
- p_filesz: dimensione del segmento sul disco.
- p_memsz: dimensione del segmento occupata in memoria ram (se $p\_memsz > p\_filesz$, la differenza definisce la sezione .bss, inizializzata a zero). Perché memsz può essere superiore a filesz (Sezione .bss)? Poiché le variabili globali non inizializzate o inizializzate a zero non necessitano di occupare spazio fisico sul disco rigido (non salviamo megabyte di zeri nel file eseguibile), filesz per quel segmento è più piccolo (escludendo la porzione .bss), mentre memsz è più grande per indicare quanta RAM totale il kernel deve allocare e azzerare in fase di caricamento del processo.
- p_flags: permessi del segmento (PF_R, PF_W, PF_X)
Perché ogni segmento può avere permessi differenti? Non è gestito a livello dell'intero file? Nei sistemi moderni la protezione della memoria opera a livello di pagina (4 KB) tramite le tabelle delle pagine della CPU. Isolare il codice eseguibile (RX) dai dati modificabili (RW) e dalle costanti (R) impedisce vulnerabilità critiche come l'esecuzione di codice iniettato nello stack o nell'heap (implementando la regola di sicurezza W XOR X). Ogni segmento PT_LOAD possiede i suoi permessi specifici (p_flags) proprio per consentire al kernel di applicare una protezione differenziata alle varie aree della memoria del processo.


L'Execution View. È un array di strutture (56 byte ciascuna) situate immediatamente dopo l'ELF Header. Descrive al kernel quali blocchi del file devono essere caricati in RAM e con quali permessi.
Perché ogni struct della Program Header (Elf64_Phdr) è di 56 e non di 64 byte? Perché la somma esatta dei campi che la compongono ammonta a 56 byte:p_type (Elf64_Word) = 4 bytep_flags (Elf64_Word) = 4 bytep_offset (Elf64_Off) = 8 bytep_vaddr (Elf64_Addr) = 8 bytep_paddr (Elf64_Addr) = 8 bytep_filesz (Elf64_Xword) = 8 bytep_memsz (Elf64_Xword) = 8 bytep_align (Elf64_Xword) = 8 byteTotale: $4 + 4 + 8 + 8 + 8 + 8 + 8 + 8 = 56$ byte. Tutti i campi a 8 byte si trovano già su offset multipli di 8, quindi non è necessario alcun padding strutturale per portarla a 64 byte.

Program Header Table (Elf64_Phdr array)
I campi critici di un Elf64_Phdr sono:

p_type: Il tipo di segmento. Il più importante è PT_LOAD (1). Solo i segmenti PT_LOAD vengono effettivamente mappati in memoria dal kernel tramite system call mmap. Altri tipi critici sono PT_INTERP (contiene il percorso del linker dinamico, es. /lib64/ld-linux-x86-64.so.2) e PT_DYNAMIC (metadati per il linking a runtime).

p_offset: L'offset del segmento all'interno del file su disco.

p_vaddr: L'indirizzo virtuale a cui mappare il segmento.

p_filesz: I byte occupati dal segmento sul disco.

p_memsz: I byte da riservare in RAM. Se p_memsz > p_filesz, la differenza viene allocata ed azzerata dal kernel: è il meccanismo con cui nasce la sezione .bss (dati non inizializzati, che non occupano spazio su disco per non sprecare byte).

p_flags: Permessi della Memory Management Unit (MMU). PF_X (1), PF_W (2), PF_R (4).

p_align: Granularità di allineamento, solitamente la dimensione della pagina di memoria (es. 0x1000 per 4KB o 0x200000 per le Huge Pages). p_vaddr modulo p_align deve essere uguale a p_offset modulo p_align.
Chi decide la granularità dell'allineamento? Il linker (es. ld), che si attiene ai vincoli dell'architettura hardware e del sistema operativo (solitamente la dimensione della pagina di memoria virtuale del kernel, che su x86-64 è standard a 4096 byte / 0x1000).


Rapporti tra Linker, Execution View e Sorgenti: Il codice sorgente genera sezioni logiche; il linker aggrega, rilocca e mappa tali sezioni nei segmenti della Execution View (Program Headers) e calcola gli indirizzi di salto/memoria definitivi.



Section Header Table (Elf64_Shdr array)
La Linking View. Generalmente posta alla fine del file fisico. Il kernel ignora questa tabella; serve a GDB, objdump o a un packer per mappare il significato logico dei byte. Ogni header descrive una Sezione. Un segmento PT_LOAD è quasi sempre un aggregato di più sezioni contigue.

Sezioni fondamentali:

.text: Il codice assembly eseguibile (mappato nel PT_LOAD con permessi RX).

.rodata: Dati di sola lettura, come costanti stringa o tabelle di salto degli switch (permessi R).

.data: Variabili globali o statiche inizializzate a valori non nulli (mappato nel PT_LOAD con permessi RW).

.bss (Block Started by Symbol): Variabili globali inizializzate a zero. Non ha byte fisici nel file.

.symtab & .strtab: Tabelle dei simboli e stringhe associate. Nei binari di produzione spesso rimosse (stripped) per offuscare i nomi delle funzioni e ridurre le dimensioni.



## PIPELINE DI GENERAZIONE DI UN ESEGUIBILE ELF: DA C AL BINARIO
La trasformazione di un sorgente in un ELF pronto per l'esecuzione avviene tramite la toolchain standard (es. GCC/Clang + GNU Binutils):

Compilazione (Frontend & Backend): Il codice C viene parsato (AST), ottimizzato tramite rappresentazioni intermedie (GIMPLE, RTL) e tradotto in assembly x86-64 puro.

Assemblaggio (as): L'assembler traduce le mnemoniche in opcode binari. L'output è un file oggetto rilocabile, un ELF di tipo ET_REL. In questo stadio, gli indirizzi di memoria non esistono ancora: ogni riferimento a variabili globali o funzioni esterne è segnato come offset 0x0 accompagnato da una tabella di rilocazione (.rel.text / .rela.text) che istruisce il linker su come calcolare gli indirizzi in futuro.

File Oggetto (ET_REL) e Rilocazione
Cosa contiene un ET_REL generato dall'assembler? Contiene già tutte le mnemoniche tradotte in opcode macchina per le istruzioni locali del file sorgente, ma non possiede gli indirizzi di memoria definitivi per qualsiasi riferimento esterno (variabili globali esterne, funzioni importate tramite extern, o chiamate a librerie). Per questi simboli, l'assembler inserisce placeholder azzerati e compila una tabella di rilocazione (.rel.text o .rela.text) che istruisce il linker su come calcolare e patchare quegli indirizzi in futuro.

Esistono ancora le sezioni del sorgente in un ET_REL? Assolutamente sì. Un file ET_REL è costituito interamente da sezioni separate (es. .text, .data, .bss, .rela.text). Il linker esiste proprio per prendere queste sezioni separate da più file .o e fonderle insieme.

Com'è fatto fisicamente un ET_REL? Inizia con l'ELF Header (Elf64_Ehdr) impostato con e_type = ET_REL, seguito immediatamente dai dati grezzi delle sezioni (codice macchina, dati) e si conclude con la Section Header Table (fondamentale per i file oggetto, poiché il linker lavora esclusivamente con la vista a sezioni). Non possiede una Program Header Table (PHDR), poiché i file oggetto non vengono mai caricati direttamente in memoria dal kernel.




Linking Statico (ld): Il linker prende in pasto multipli ET_REL (e archivi .a). Unisce le sezioni omologhe (tutte le .text insieme, tutte le .data insieme), risolve i simboli incrociati e applica le formule matematiche definite nelle rilocazioni per assegnare gli indirizzi di memoria virtuale definitivi.

Generazione dell'Eseguibile: Il risultato è un ELF di tipo ET_EXEC (eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza) o ET_DYN (Position Independent Executable - PIE, lo standard moderno). In un ET_DYN, gli indirizzi generati dal linker non sono assoluti, ma relativi al registro RIP, permettendo al kernel di caricare il binario in un punto casuale della RAM (ASLR - Address Space Layout Randomization)

Fasi della compilazione, Frontend e Backend:

Preprocessing (cpp): Risoluzione delle macro (#define) e inclusione degli header (#include).

Compilazione (Frontend): Analisi lessicale, sintattica e semantica del codice C, traduzione in un Abstract Syntax Tree (AST) e generazione della rappresentazione intermedia (IR).

Ottimizzazione e Generazione del Codice (Backend): Elaborazione delle ottimizzazioni indipendenti e dipendenti dalla CPU e traduzione finale in codice Assembly specifico per l'architettura target (es. x86-64).

Assemblaggio (as): Conversione delle istruzioni Assembly in opcode binari grezzi, producendo il file oggetto rilocabile (.o).

Linking (ld): Fusione di più file oggetto e librerie per generare l'eseguibile finale.

Chi se ne occupa: I driver di compilazione (gcc o clang) orchestrano l'intera pipeline richiamando internamente i singoli moduli (preprocessor, parser LLVM/GCC, assembler e linker).








Cosa accade a livello di sistema operativo quando esegui un comando come ./programma?

La System Call: La shell esegue un fork() e poi chiama execve("./programma", ...).

L'Intervento del Kernel: Il kernel apre il file, legge i primi byte e identifica il magic number \x7fELF. L'esecuzione passa all'handler load_elf_binary() all'interno del kernel source (fs/binfmt_elf.c).
Cos'è load_elf_binary()? È la funzione cardine del sottosistema binario del kernel Linux (fs/binfmt_elf.c). Quando lanci un eseguibile, il kernel invoca questa funzione di bootstrap e parsing per leggere l'header ELF, allocare le strutture di task (mm_struct), mappare i segmenti PT_LOAD tramite la system call interna di mmap, configurare lo stack iniziale del processo e preparare il salto all'entry point.

Il Mapping (Segmentazione in RAM): Il kernel itera l'array dei Program Headers. Per ogni PT_LOAD trovato, invoca un mmap() per copiare/associare quel blocco del file fisico nelle pagine di RAM del nuovo processo, applicando i bit di protezione MMU specificati in p_flags (RX per il codice, RW per i dati).

Bivio Architetturale (Statico vs Dinamico):

Binario Statico: Nessun segmento PT_INTERP presente. Il kernel imposta il registro RSP (lo stack con argv ed envp) e salta direttamente all'indirizzo e_entry letto dall'ELF Header.

Binario Dinamico (SOTA per Linux): Esiste un segmento PT_INTERP che punta a ld-linux.so. Il kernel mappa in memoria non solo il tuo eseguibile, ma anche l'interprete (il linker dinamico). Invece di saltare all'e_entry del tuo file, il kernel salta all'e_entry del linker dinamico.
Binario dinamico, linker, interprete e il ruolo del runtime: C è un linguaggio compilato in codice macchina nativo e non ha bisogno di un interprete nel senso di una macchina virtuale (come Python o Java). Tuttavia, i moderni eseguibili Linux sono quasi sempre dinamici, ovvero dipendono da librerie condivise esterne (come la libc.so.6). Per consentire a un eseguibile di risolvere tali dipendenze a runtime, il file ELF incorpora un puntatore a un componente esterno: il linker dinamico (/lib64/ld-linux-x86-64.so.2). Nelle specifiche ELF, questo componente viene formalmente denominato Interprete (PT_INTERP). Di conseguenza, quando esegui un binario dinamico, il kernel mappa in memoria sia il tuo eseguibile sia il linker dinamico, passando inizialmente il controllo a quest'ultimo affinché risolva i simboli e prepari l'ambiente, prima di cedere definitivamente l'esecuzione al punto d'ingresso del tuo programma.

Linking a Runtime (ld.so): Il linker dinamico si sveglia, analizza il segmento PT_DYNAMIC del tuo eseguibile, mappa in memoria tutte le librerie condivise necessarie (es. libc.so.6), risolve i simboli essenziali, popola la GOT (Global Offset Table) e la PLT (Procedure Linkage Table) per consentire le chiamate a funzioni esterne, e infine cede il controllo saltando al tuo e_entry (che solitamente punta a _start, la quale inizializza la libc e infine chiama main()).
Cosa sono GOT e PLT?

GOT (Global Offset Table): Una tabella di puntatori situata nel segmento dati (RW) che contiene gli indirizzi di memoria effettivi delle funzioni esterne e delle variabili globali, risolti a runtime dal linker dinamico.

PLT (Procedure Linkage Table): Una tabella di stub di codice eseguibile (RX) che permette al codice di saltare verso funzioni esterne (es. printf). Poiché con i binari PIE e l'ASLR gli indirizzi cambiano a ogni avvio, la PLT funge da tramite: interroga la GOT; se la funzione non è ancora stata risolta, attiva il linker dinamico (ld.so) per risolverla al primo giro (lazy binding), memorizzando poi l'indirizzo nella GOT per tutte le chiamate successive.

Indirizzi nei file PIE (ET_DYN) e il registro RIP: Nei binari moderni Position Independent Executable (ET_DYN), gli indirizzi risolti dal linker non sono assoluti (fissi), ma relativi al registro RIP (Instruction Pointer). Quando si parla di indirizzamento relativo al RIP, facciamo riferimento all'istruzione attualmente in esecuzione. Poiché il kernel carica il binario in un punto casuale della RAM (ASLR), il registro RIP punta dinamicamente a quell'indirizzo base effettivo; di conseguenza, il codice calcola gli offset interni sommando o sottraendo la distanza relativa rispetto alla posizione corrente del RIP, garantendo il funzionamento del programma indipendentemente dalla locazione fisica in memoria.
3. Come cazzo funziona l'indirizzamento RIP-relative se il RIP cambia in continuazione?
Il motivo per cui funziona è esattamente perché il RIP cambia in continuazione. L'indirizzamento RIP-relative non usa indirizzi assoluti (fissi), ma spostamenti relativi (offset) calcolati in base alla posizione corrente del processore.

Se scrivi un'istruzione che deve leggere una variabile o saltare a una funzione vicina in un binario PIE (Position Independent Executable), il compilatore non genera un'istruzione con un indirizzo fisso del tipo mov rax, [0x401050].

Genera invece un'istruzione del tipo mov rax, [rip + 0x1234].

A runtime, la CPU esegue l'istruzione: prende il valore attuale del registro RIP (che punta all'istruzione successiva) e ci somma l'offset costante 0x1234.

Poiché l'offset in byte tra l'istruzione e la variabile è matematicamente fisso (indipendentemente da dove il kernel decide di caricare il programma in RAM grazie all'ASLR), la somma RIP + offset punterà sempre esattamente al bersaglio corretto. Il RIP cambia ad ogni ciclo di clock, ma la distanza relativa tra le parti del codice rimane immutata.


4. Linker statico vs Linker dinamico: sono la stessa cosa?
No, sono due software completamente distinti che operano in momenti temporali e contesti operativi opposti:

Il Linker Statico (ld / GNU Binutils):

È un tool di sviluppo che gira sulla macchina di compilazione (offline).

Prende i file oggetto .o generati dall'assembler, risolve i simboli interni, calcola le rilocazioni e assembla il file ELF finale sul disco. Una volta terminata la compilazione, il compito di ld è esaurito. Il binario è pronto.

Il Linker Dinamico (ld-linux-x86-64.so.2 / ld.so):

È una libreria di sistema (un eseguibile condiviso) che risiede fisicamente sul sistema operativo target (es. in /lib64/).

Entra in azione a runtime, nel momento esatto in cui lanci il programma. Quando esegui un binario dinamico, il kernel Linux legge l'header PT_INTERP, scopre che il binario ha bisogno di ld.so, mappa in memoria sia il tuo programma sia il linker dinamico, e cede inizialmente il controllo a quest'ultimo. ld.so si occupa di aprire le librerie condivise (.so), caricarle in RAM, risolvere i simboli esterni mancanti (popolando la GOT) e solo allora avviare l'esecuzione del tuo codice.

















Ecco le risposte puntuali, dettagliate e rigorose a tutti i tuoi quesiti, senza filtri e al massimo livello tecnico:

---

### 1. PT_LOAD, Sezioni e File Multipli (`.c`)

* **Sì, un eseguibile contiene tipicamente più segmenti `PT_LOAD**` (di solito due principali nei binari standard: uno in sola lettura ed esecuzione `R-X` per codice e costanti, e uno in lettura e scrittura `RW-` per dati e variabili non inizializzate).
* **Compilazione di più sorgenti `.c` contemporaneamente:** Quando compili più file `.c` (es. `gcc main.c utils.c -o program`), l'assembler genera un file `.o` distinto per ciascuno di essi. Ciascun file `.o` possiede le proprie sezioni `.text`, `.data`, ecc. Il linker (`ld`) prende **tutte le sezioni omologhe di tutti i file oggetto** e le fonde insieme in macro-sezioni uniche nell'eseguibile finale (tutte le `.text` di tutti i file `.o` vengono concatenate in un'unica grande `.text` continua). Questa macro-sezione viene poi inserita nel rispettivo segmento `PT_LOAD`.
* **Quanti `PT_LOAD` contiene un eseguibile?** Minimo 2 (uno per il codice eseguibile, uno per i dati scrivibili). Possono essercene di più se ci sono particolari requisiti di allineamento delle pagine di memoria o segmenti di sola lettura separati (`.rodata`), ma lo standard per binari semplici è 2 o 3.

---

### 2. La Sezione `.bss`

* **Esiste solo in RAM?** Sì e no. Fisicamente sul disco, nel file eseguibile, **non occupa spazio** per i dati non inizializzati (ha `p_filesz = 0` o comunque non alloca i byte dei dati nel file). Esiste solo come metadato descrittivo (`p_memsz` > `p_filesz`). Quando il kernel mappa il segmento in memoria tramite `mmap`, legge la differenza tra `p_memsz` e `p_filesz` e **alloca ed azzera fisicamente blocchi di RAM** (zero-fill pages). Quindi sul disco è solo un'istruzione di dimensione per la MMU, mentre in RAM diventa materia effettiva.

---

### 3. Sistemi Operativi UNIX-like

* **Cosa hanno in comune?** Condividono l'adesione concettuale agli standard POSIX / Single UNIX Specification: il modello di permessi basato su UID/GID, la filosofia *"everything is a file"* (astrazione di periferiche, socket e pipe come descrittori di file), le system call fondamentali per la gestione dei processi (`fork`, `execve`, `wait`) e l'adozione universale del formato **ELF** come standard per i binari eseguibili e le librerie condivise.

---

### 4. Toolchain, Compilazione e Rappresentazioni Intermedie

* **`ld` fa parte sia di GCC che delle GNU Binutils?** `ld` appartiene ufficialmente alle **GNU Binutils** (il pacchetto indipendente di tool di basso livello). GCC non include il codice sorgente di `ld` al suo interno, ma lo invoca esternamente come fase finale (il "linking driver") per produrre l'eseguibile.
* **Cosa sono GIMPLE e RTL?** Sono le rappresentazioni intermedie (Intermediate Representations - IR) usate internamente dal compilatore GCC:
* **GIMPLE:** Un'IR ad alto livello basata su alberi semplificati e codice a tre indirizzi (3-address code), usata per applicare ottimizzazioni indipendenti dalla CPU (es. loop unrolling, rimozione del codice morto).
* **RTL (Register Transfer Language):** Un'IR a basso livello simile a un assembly virtuale con un numero infinito di registri virtuali, usata dal backend per tradurre il codice nell'architettura hardware specifica (es. x86-64).
* *In che linguaggio sono scritte?* Sono strutture dati in memoria scritte interamente in **C** (il linguaggio con cui è implementato il sorgente di GCC).


* **La tabella di rilocazione (`ET_REL`) è un file di testo con gli offset?** **Assolutamente no.** Non è un file di testo leggibile. È una struttura dati binaria strutturata (un array di struct `Elf64_Rela`) incorporata all'interno del file oggetto binario.
* **Ogni file sorgente genera un file oggetto rilocabile?** Sì, ogni volta che un compilatore traduce un file sorgente con l'opzione `-c`, produce esattamente un file oggetto rilocabile (`.o` / `ET_REL`).

---

### 5. Tipi Base e Anatomia dei Byte ELF

* **I tipi base ELF sono basicamente `uint64`, `uint32`, `uint16`?** Esatto. Sono `typedef` primitivi in C (`uint64_t`, `uint32_t`, `uint16_t`, ecc.) ridefiniti in `<elf.h>` con nomi specifici dell'ABI (es. `Elf64_Addr`, `Elf64_Off`, `Elf64_Word`, `Elf64_Half`) per garantire la portabilità rigorosa delle dimensioni su qualsiasi architettura host.
* **Perché il primo byte del magic number è esattamente `127` (`\x7f`)?** Perché storicamente (nel design dei vecchi sistemi Unix e nei loader `a.out`/ELF) il valore `0x7F` non è un carattere ASCII stampabile (corrisponde al carattere `DEL`), scelto appositamente per impedire che il sistema tentasse di interpretare ed eseguire file di testo ASCII o binari corrotti come script di shell, bloccando immediatamente l'esecuzione prima di analizzare i caratteri alfabetici successivi (`E`, `L`, `F`).
* **`Elf64_Phdr` è la singola struttura o l'array?** `Elf64_Phdr` è la **definizione della singola struttura** (il singolo record di 56 byte). La Program Header Table è un **array contiguo** di queste strutture. Ciascuna di esse possiede il suo campo `p_type` (che specifica se si tratta di `PT_LOAD`, `PT_DYNAMIC`, `PT_NOTE`, ecc.).
* **Gli offset sul disco sono sempre calcolati a partire dal byte `0x0`?** Sì. Tutti gli offset nei file ELF (`e_phoff`, `e_shoff`, `p_offset`) sono **offset assoluti in byte** misurati rigorosamente a partire dall'inizio fisico del file sul disco (byte `0x0`).
* **Le `p_flags` sono un OR bitwise tra i permessi?** Sì. Sono una maschera di bit combinata in OR logico: `PF_X` (1), `PF_W` (2), `PF_R` (4). Un segmento leggibile ed eseguibile avrà `PF_R | PF_X` ($4 + 1 = 5$).

---

### 6. Hardware e Kernel (`MMU`, `AGU`, `binfmt_elf.c`)

* **C'è differenza tra MMU e AGU?**
* **MMU (Memory Management Unit):** L'unità hardware della CPU che traduce gli indirizzi virtuali in indirizzi fisici tramite le tabelle delle pagine (Page Tables) e applica i bit di protezione della memoria (`RWX`).
* **AGU (Address Generation Unit):** L'unità hardware dedicata esclusivamente al calcolo degli indirizzi di memoria complessi per le istruzioni di load/store (es. basati su registri base, indice e scala: `[rax + rbx*4 + 0x123]`), sgravando le ALU principali.


* **Cosa sono le tabelle dei simboli e le stringhe associate?**
* **Symbol Table (`.symtab`):** Una tabella che elenca tutti i simboli definiti o referenziati nel modulo (funzioni, variabili globali), associandoli al loro indirizzo o offset e dimensione.
* **String Table (`.strtab`):** Una tabella di stringhe testuali in formato zero-terminated che contiene i nomi reali dei simboli (es. `"main"`, `"printf"`). La symbol table non memorizza i nomi per intero, ma punta semplicemente all'offset numerico dei caratteri all'interno della `.strtab` per ottimizzare lo spazio.


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


* **Differenza tra funzione `extern` e funzione condivisa (dynamic):**
* `extern` è una **keyword di dichiarazione logica a livello di linguaggio C** che dice al compilatore di non generare un indirizzo definitivo per quel simbolo, lasciando un segnaposto per il linker.
* La **funzione condivisa** è l'entità binaria reale che risiede all'interno di una libreria esterna (`.so`). Quando dichiari una funzione `extern` e questa appartiene a una libreria esterna (es. `printf` della `libc`), il linker inserisce una voce nella PLT/GOT, demandando al **linker dinamico** (`ld-linux.so`) il compito di risolverla a runtime agganciando l'indirizzo reale.


* **Cosa si intende per "dati grezzi" delle sezioni?** Sono i **byte binari puri** (gli opcode delle istruzioni macchina o i valori binari grezzi delle variabili/costanti) memorizzati nel file, privi di sovrastrutture di metadati, ma semplicemente puntati dagli offset delle tabelle.
* **Qual è la sequenza strutturale fisica esatta di un file ELF su disco?**
1. **ELF Header** (`Elf64_Ehdr`) (offset `0x0`, 64 byte fissi).
2. **Program Header Table** (`Elf64_Phdr` array).
3. **Sezioni Grezze / Dati dei Segmenti** (il corpo principale del binario: codice `.text`, dati `.data`, costanti `.rodata`, ecc.).
4. **Section Header Table** (`Elf64_Shdr` array) (solitamente posizionata in coda al file, sebbene il kernel la ignori completamente e si affidi esclusivamente alla Program Header Table).