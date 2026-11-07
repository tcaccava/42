## COSA E' UN PACKER

Un packer è uno strumento di manipolazione binaria post-compilazione che riceve in input un programma eseguibile (.exe, .dll, ELF) e genera un nuovo binario in cui il payload originale viene compresso e cifrato simultaneamente. La sua esistenza deriva dalla necessità di eludere l'analisi in memoria degli engine antivirus, mantenendo il contenuto dell'eseguibile offuscato fino al momento dell'esecuzione.
Allo stato dell'arte, un packer agisce come un incapsulatore (o protector) che altera la struttura statica del file e il suo flusso di esecuzione, operando attraverso due fasi meccaniche distinte:

1. Fase Statica (Packing Time)
Il packer disseziona il file originale e ne altera gli header (ELF o PE) per eseguire tre operazioni fondamentali:

- Cifratura e compressione: le sezioni originali (codice, dati e risorse) vengono sottoposte ad algoritmi crittografici e di compressione, trasformando opcode e dati in un blocco ad altissima entropia.

- Iniezione dello Stub: un frammento di codice assembly indipendente (lo stub o loader) viene iniettato nel file. Questo può avvenire creando una nuova sezione dedicata o sfruttando le cave di allineamento (spazi vuoti) tra i segmenti esistenti.

- Dirottamento dell'Entry Point: l'indirizzo di ingresso originale del binario (OEP - Original Entry Point),cioe' il campo e_entry nell'ELF Header, viene sovrascritto. Il nuovo Entry Point punterà direttamente allo stub, garantendo che al momento del lancio l'OS ceda il controllo a quest'ultimo.

2. Fase Dinamica (Runtime & Unpacking)
Quando il nuovo binario viene eseguito, il kernel mappa in memoria i suoi segmenti che si trovano già fisicamente in uno stato compresso e cifrato sul disco. A quel punto lo stub prende il controllo della CPU per decifrare e decomprimere il payload al volo direttamente in RAM, ripristinando il codice originale prima di cedergli l'esecuzione:

- Elevazione dei privilegi di memoria: lo stub invoca le system call di gestione della memoria (es. mprotect su Linux o VirtualProtect su Windows) per alterare i flag di protezione delle pagine, passando da permessi di sola Lettura/Esecuzione (RX) a permessi di Scrittura (RWX).

- Ricostruzione del Payload: lo stub itera sulle sezioni cifrate mappate in memoria, applicando l'algoritmo inverso per decrittografarle e decomprimerle in-place.

- Risoluzione Dinamica delle dipendenze: se l'eseguibile utilizza librerie dinamiche, lo stub deve occuparsi di ricostruire le tabelle di reindirizzamento (IAT in ambiente PE, GOT/PLT in ambiente ELF). Poiché il file su disco era cifrato, il loader del kernel non ha potuto effettuare il linking dinamico delle funzioni esterne.

- Salto all'OEP e consegna: terminata la ricostruzione, lo stub ripristina i permessi di memoria originali per rispettare i vincoli di sicurezza (W^X), cancella le proprie tracce e le chiavi dai registri, per poi eseguire un salto incondizionato (JMP/CALL) all'OEP. Il programma originale viene infine eseguito in maniera del tutto identica a come avrebbe fatto prima di passare attraverso il packer. 

# Evoluzione SOTA (State of the Art)
I packer moderni superano la semplice compressione o i cifrari lineari, implementando meccanismi di software protection concepiti per confondere le euristiche degli analisti e dei debugger:

- Code virtualization: lo stub non contiene istruzioni hardware native, ma implementa una Virtual Machine integrata con un set di istruzioni (ISA) proprietario e randomizzato per ogni processo di packing. Parti critiche del codice originale vengono tradotte in questo bytecode incomprensibile ai classici disassemblatori.

- Metamorfismo dello stub: il codice dello stub muta strutturalmente ad ogni esecuzione del packer. Utilizza l'alterazione del Control-Flow Graph (CFG), la dead-code insertion e la sostituzione polimorfica delle istruzioni (es. xor rax, rax sostituito da sub rax, rax) per impedire la creazione di firme crittografiche (signature) statiche.

- Anti-Tampering & Anti-Debugging avanzato: lo stub integra controlli ostili continui; interroga i registri hardware di debug (es. DR0-DR7), utilizza misurazioni temporali precise (es. istruzione rdtsc) per rilevare scostamenti di latenza tipici delle virtual machine o degli hypervisor, e verifica l'integrità del proprio codice in RAM per impedire il dumping della memoria decifrata.


## DEFINIZIONE E STRUTTURA DI UN ESEGUIBILE ELF

Il formato ELF (Executable and Linkable Format) è una specifica di formato dei files binari,lo standard architetturale de facto di disposizione dei dati del binario nei sistemi operativi UNIX-like (Linux, *BSD, Solaris).I sistemi UNIX like condividono l'adesione concettuale agli standard POSIX / Single UNIX Specification: il modello di permessi basato su UID/GID, la filosofia "everything is a file" (astrazione di periferiche, socket e pipe come descrittori di file), le system call fondamentali per la gestione dei processi (fork, execve, wait) e l'adozione universale del formato ELF come standard per i binari eseguibili e le librerie condivise. È definito nelle specifiche System V Application Binary Interface (si pronuncia "System Five" in riferimento alla System V Release 4 - SVR4, lo standard industriale definito da AT&T UNIX su cui si basano le moderne specifiche ELF e le ABI dei sistemi UNIX-like). I programmi che leggono o generano files in formato ELF(compilatori, linker, kernel) sono scritti in C o C++, ma il file ELF in sé è solo una sequenza strutturata di byte grezzi su disco.
Non si tratta di un semplice contenitore di codice macchina, ma di un database relazionale strutturato che fornisce al sistema operativo e al linker dinamico tutte le topografie di memoria e i metadati necessari per caricare ed eseguire un processo, oppure per collegare moduli oggetto multipli.

Il concetto cardine per padroneggiare l'architettura ELF è il Dual View Paradigm: non si tratta di strutture fisiche separate o duplicate sul disco, ma di una duplice proiezione logica dei medesimi byte. Le stesse identiche sezioni binarie vengono lette attraverso due chiavi di accesso differenti: il linker le analizza come sezioni distinte tramite la Section Header Table (vista ad intestazioni di sezione), mentre il kernel del sistema operativo le ignora completamente e legge i medesimi bytes aggregandoli in segmenti di memoria tramite la Program Header Table (vista ad intestazioni di programma).

- Linking View (vista per Sezioni): utilizzata da linker (ld), assembler e tool di analisi (es. objdump, readelf). Divide il file in Sezioni logiche (codice, dati inizializzati, tabelle di stringhe).

- Execution View (vista per Segmenti): utilizzata esclusivamente dal loader del kernel Linux (fs/binfmt_elf.c) al momento dell'esecuzione. Ignora del tutto le sezioni e divide il file in segmenti di memoria mappabili (Program Headers).

STRUTTURA ESEMPIO DI UN FILE ELF

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

Tutto in un file ELF a 64-bit fa riferimento ai tipi di base definiti nella libreria <elf.h>, come Elf64_Addr (8 byte), Elf64_Off (8 byte), Elf64_Word (4 byte), Elf64_Half (2 byte).
I tipi base della libreria <elf.h> non sono concetti astratti, ma tipi interi primitivi(basicamente degli uint64, uint32, uint16) definiti in C tramite typedef per garantire la rigorosa portabilità cross-platform delle dimensioni (es. typedef uint64_t Elf64_Addr, typedef uint16_t Elf64_Half, typedef uint64_t Elf64_Off).

I byte dell'eseguibile binario sul disco sono una sequenza continua di bit: partendo dal byte 0x0, niziano con l'ELF Header(i primi 64 bytes), poi c'e' la Program Header Table(organizzata in Segmenti,che iniziano tipicamente dopo l'header all' offset 0x40), poi blocchi di dati grezzi (il corpo enorme del binario costituito dalle istruzioni macchina del codice(.text), le costanti(.rodata), le variabili(.data)),e infine la Section Header Table(organizzata in Sezioni,collocati alla fine assoluta del fine sul disco).
Fisicamente le due Table sono array contigui di byte memorizzati nel file binario, disposti rigidamente secondo i layout geometrici delle strutture dati in C. Rappresentano blocchi di dati binari interpretati dal software in base a offset fissi.
Le tabelle (Elf64_Shdr e Elf64_Phdr) dunque non sono i dati stessi, ma gli indici descrittivi (i metadati) che dicono al software come interpretare i byte grezzi del file.

* ELF Header (Elf64_Ehdr): e' situato situato sempre all'offset 0x0 del file. È la "stele di Rosetta" che rappresenta il punto di partenza per l'analisi statica della struttura del file ,descrivendo dove trovare il resto delle tabelle. Occupa 64 byte.
  Componenti chiave dell'header:
  - e_ident[16]: i primi 4 byte sono il Magic Number (\x7F E L F), bytes 0-3.  127 (\x7f) e' il primo byte perché storicamente (nel design dei vecchi sistemi Unix e nei loader a.out/ELF) il valore 0x7F non è un carattere ASCII stampabile (corrisponde al carattere DEL,i printable vanno da 32 a 126 ascii), scelto appositamente per impedire che il sistema tentasse di interpretare ed eseguire file di testo ASCII o binari corrotti come script di shell, bloccando immediatamente l'esecuzione prima di analizzare i caratteri alfabetici successivi (E, L, F). I successivi bytes indicano la classe (32 (01) o (64) 02 bit, byte 4), l'endianness (LSB (01)/MSB(02), byte 5) e la versione dell'ABI. Per esempio l'e_ident di un ELF per un architettura little endian a 64 bit con ultima versione ABI assume questa configurazione hex esatta: \x7f \x45 \x4c \x46 \x02 \x01 \x01 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00.
        \x7f\x45\x4c\x46: Magic Number (\x7FELF).
        \x02: Classe (ELFCLASS64 = 64-bit).
        \x01: Endianness (ELFDATA2LSB = Little Endian).
        \x01: Versione ELF (EV_CURRENT).
        \x00: OS/ABI (ELFOSABI_SYSV).
        \x00 - \x00: Byte di padding ed estensione ABI non utilizzati.
  - e_type: definisce il ruolo del file (ET_REL = 1, ET_EXEC = 2, ET_DYN = 3, ET_CORE = 4).
  ET_EXEC indica un eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza,mentre ET_DYN (Position Independent Executable - PIE) rappresenta lo standard moderno.
  ET_CORE indica un file di dump della memoria generato dal kernel Linux quando un processo va in crash irreversibile (es. Segmentation Fault, SIGABRT). Contiene l'istantanea esatta dello stato dei registri della CPU, dello stack, dell'heap e delle aree di memoria del processo al momento della morte, impiegato dai debugger (come GDB) per l'analisi post-mortem. ET_REL e' il tipo dei file oggetti .o in cui non sono ancora risolti gli indirizzi di tutti i simboli esterni al sorgente dell'eseguibile(es. funzioni importate).
  - e_machine: l'architettura target (per x86-64 è EM_X86_64 = 0x3E).Esistono decine di varianti standardizzate nelle specifiche ELF per identificare l'architettura hardware di destinazione (es. EM_386 = 3 per x86 a 32-bit, EM_X86_64 = 62 per AMD64/Intel 64, EM_ARM = 40, EM_AARCH64 = 183, EM_RISCV = 243).
  - e_entry: indirizzo virtuale (o l'offset nel caso di PIE) del punto di ingresso iniziale (OEP - Original Entry Point),cioe' della prima istruzione da cui la CPU deve iniziare l'esecuzione ( se uso la toolchain standard (GCC/Clang), il linker 
  Se compilo un programma C affidandomi alla toolchain standard della libc(GCC/Clang) senza definire un entrypoint custom _start, il linker imposta automaticamente e_entry in modo che punti a un simbolo chiamato _start, un frammento di codice assembly fornito dal C Runtime (precisamente dal file oggetto di sistema crt1.o) e include automaticamente i moduli oggetto di avvio della libreria standard (crt1.o, crti.o, ecc.). Il simbolo _start (contenuto in crt1.o) è il vero punto d'ingresso fisico del processo: allinea lo stack pointer (RSP) a 16 byte e pulisce i registri,si occupa di inizializzare l'ambiente di runtime del C, invoca __libc_start_main, la quale raccoglie gli argomenti della shell per formare argc e argv e esegue i costruttori globali ,e infine chiama la funzione main(). Di conseguenza, in questi casi e_entry punta direttamente all'indirizzo del simbolo _start fornito dalla libc.
  - e_phoff: offset fisici assoluti dal byte 0x0 in cui inizia la Program Header Table.
  - e_phnum: numero di entry presenti nella Program Header Table.
  - e_shoff e e_shnum: offset e conteggio delle Section Headers.

* La Program Header Table è un array separato di strutture (Elf64_Phdr) che funge da mappa per il kernel: il kernel usa questa mappa per sapere quali segmenti mappare nella memoria fisica/virtuale tramite il loader del kernel (fs/binfmt_elf.c). e con quali permessi di esecuzione.
L'Execution View e' un array di strutture (56 byte ciascuna) situate immediatamente dopo l'ELF Header. Perché 56 e non 64 byte? Perché la somma esatta dei 7 campi che la compongono ammonta a 56 byte:
- p_type (Elf64_Word) = 4 byte
- p_flags (Elf64_Word) = 4 byte
- p_offset (Elf64_Off) = 8 byte
- p_vaddr (Elf64_Addr) = 8 byte
- p_paddr (Elf64_Addr) = 8 byte
- p_filesz (Elf64_Xword) = 8 byte
- p_memsz (Elf64_Xword) = 8 byte
- p_align (Elf64_Xword) = 8 byte 
Tutti i campi a 8 byte si trovano già su offset multipli di 8, quindi non è necessario alcun padding strutturale per portarla a 64 byte.
- p_type: il tipo di segmento. Il più importante è PT_LOAD (1), unico tipo di segmento mappato in memoria dal kernel tramite system call mmap. Un eseguibile contiene tipicamente più segmenti PT_LOAD (di solito almeno due principali nei binari standard: uno in sola lettura ed esecuzione R-X per codice e costanti, e uno in lettura e scrittura RW per dati e variabili non inizializzate). Possono essercene di più se ci sono particolari requisiti di allineamento delle pagine di memoria o segmenti di sola lettura separati (.rodata), ma lo standard per binari semplici è almeno 2 o 3. Altri tipi critici sono PT_INTERP (contiene il percorso del linker dinamico, es. /lib64/ld-linux-x86-64.so.2) ,PT_DYNAMIC (metadati per il linking a runtime) e PT_NOTE.
- p_offset: l'offset in byte dell'inizio del segmento misurato rigorosamente a partire dall'inizio assoluto del file ELF su disco (offset 0x0).
- p_vaddr: indirizzo virtuale in RAM a cui il segmento deve essere mappato nello spazio di indirizzamento virtuale del processo gestito dalla MMU (Memory Management Unit) del kernel.
- p_filesz: numero di bytes occupati dal segmento sul disco.
- p_memsz: i byte da riservare in RAM, cioe' la dimensione occupata dal segmento in memoria. Se p_memsz > p_filesz, la differenza viene allocata ed azzerata dal kernel: è il meccanismo con cui nasce la sezione .bss che contiene  dati non inizializzati, che non occupano spazio su disco per non sprecare byte. Le variabili globali non inizializzate o inizializzate a zero non necessitano di occupare spazio fisico sul disco rigido (non ha alcun senso salvare megabytes di zeri nel file eseguibile).
- p_flags: permessi del segmento per la Memory Management Unit (MMU)(PF_X (1), PF_W (2), PF_R (4)). Perché ogni segmento può avere permessi differenti? Nei sistemi moderni la protezione della memoria opera a livello di pagina (4 KB) tramite le tabelle delle pagine della CPU. Isolare il codice eseguibile (RX) dai dati modificabili (RW) e dalle costanti (R) impedisce vulnerabilità critiche come l'esecuzione di codice iniettato nello stack o nell'heap (implementando la regola di sicurezza W XOR X). Ogni segmento PT_LOAD possiede i suoi permessi specifici (p_flags) proprio per consentire al kernel di applicare una protezione differenziata alle varie aree della memoria del processo. Le p_flags sono una maschera di bit combinata in OR logico: PF_X (1), PF_W (2), PF_R (4). Un segmento leggibile ed eseguibile avrà ad esempio PF_R | PF_X (4 + 1 = 5).
I permessi sono dichiarati a livello logico nei segmenti nell'ELF, ma vengono applicati fisicamente a livello di singola pagina (4KB) dalla CPU: l'ELF specifica nel PT_LOAD del Program Header i permessi che desidera per quel macro-blocco (es. PF_R | PF_X),il kernel legge questa richiesta, calcola quante pagine da 4096 byte servono per ospitare quel segmento e chiama mmap.
La MMU (l'hardware della CPU) ignora totalmente cosa sia un file ELF o un segmento. Lei opera solo sulle Page Tables. Il kernel, quindi, prende i permessi del segmento ELF e li salva sui bit di protezione di ciascuna pagina fisica da 4KB che compone quel segmento. Ecco perché il linker allinea rigorosamente i segmenti a multipli della dimensione di una pagina (campo p_align a 0x1000): se un segmento di codice RX e un segmento di dati RW finissero a cavallo della stessa pagina fisica da 4KB, la MMU andrebbe in tilt non potendo assegnare permessi doppi alla stessa pagina, e il kernel sarebbe costretto a dare permessi RWX all'intera pagina, distruggendo la sicurezza.
Se un segmento richiede la mappatura di una quantità di byte non multipla di 4096, il sistema arrotonda per eccesso al confine di pagina successivo, riempiendo lo spazio residuo non utilizzato con degli zeri. Un payload da 1 a 4096 byte occupa fisicamente una singola pagina, mentre un payload da 4097 a 8192 byte forza tassativamente l'allocazione di due pagine contigue.
- p_align: granularità di allineamento, solitamente la dimensione della pagina di memoria (es. 0x1000 per 4KB o 0x200000 per le Huge Pages). p_vaddr modulo p_align deve essere uguale a p_offset modulo p_align. Chi decide la granularità dell'allineamento? Il linker (es. ld), che si attiene ai vincoli dell'architettura hardware e del sistema operativo (solitamente la dimensione della pagina di memoria virtuale del kernel, che su x86-64 è standard a 4096 byte / 0x1000).

Quindi Elf64_Phdr è la definizione della singola struttura (il singolo record di 56 byte). La Program Header Table è un array contiguo di queste strutture. Ciascuna di esse possiede il suo campo p_type (che specifica se si tratta di PT_LOAD, PT_DYNAMIC, PT_NOTE ecc.).
I Segmenti PT_LOAD della Execution View non trovano corrispondenza 1:1 nelle diverse componenti(singole righe o sezioni) in cui e' diviso il sorgente Assembly o il codice C. Nel codice assembly direttive come .data o. text servono a suddividere logicamente il codice. Il compilatore e l'assembler traducono queste direttive in sezioni all'interno del file oggetto .o. Il linker, successivamente, prende queste sezioni e le raggruppa in macro-blocchi chiamati segmenti (PT_LOAD), definiti in base base alla compatibilità dei permessi di memoria definiti dalle specifiche ELF,cioe' le p_flags lette dalla MMU:
- sezioni con permessi di sola lettura ed esecuzione (.init, .text, .fini, .rodata) confluiscono nel segmento PT_LOAD con flag PF_R | PF_X.
- sezioni con permessi di lettura e scrittura (.data, .bss, .init_array, .fini_array, .got) confluiscono nel segmento PT_LOAD con flag PF_R | PF_W.
Questo ovviamente vale anche per sezioni di file sorgente diversi: per es. tutte le .text di tutti i file .o differenti vengono fuse insieme in un'unica grande sezione .text continua, che finisce nello stesso PT_LOAD eseguibile.

Le sezioni in cui e' diviso un file sorgente .s (tramite direttive come .text o .data) hanno una corrispondenza diretta solo nella Linking View, ma vengono completamente appiattite e perse nella Execution View. Nella Linking View le sezioni .text scritte nel .s vengono conservate dall'assembler nel file oggetto .o e poi fuse dal linker nella Section Header Table dell'eseguibile finale. Se apro il binario con readelf -S, vedro' esattamente ogni singola sezione.
Al kernel invece non importa nulla delle sezioni. Il kernel vede solo i PT_LOAD (i segmenti). Il linker prende la sezione .text, la sezione .rodata, ecc., e le fonde brutalmente dentro un unico segmento PT_LOAD con permessi RX (Read-Execute). Di conseguenza, a livello di Program Headers (Execution View), le singole sezioni del sorgente scompaiono come entità distinte: esistono solo blocchi continui di memoria gestiti dalla MMU.

* La Section Header Table , generalmente posta alla fine del file fisico, è un array di strutture (Elf64_Shdr) che funge da mappa logica: il linker usa questa mappa per sapere cosa sta manipolando. Il kernel invece ignora completamente questa tabella; serve a GDB, objdump o a un packer per mappare il significato logico dei byte. Ogni header descrive una Sezione. Un segmento PT_LOAD è quasi sempre un aggregato di più sezioni contigue.
Nel file oggetto (.o / ET_REL) esistono solo le Sezioni. La Program Header Table non esiste fisicamente e non ci sono segmenti, semplicemente perché un file oggetto non è progettato per essere mappato in RAM e mandato in esecuzione dal kernel.Nell'eseguibile finale (a.out / ET_DYN o ET_EXEC) esistono invece contemporaneamente sia sezioni che segmenti: i segmenti si trovano all'inizio (dettati dalla Program Header Table) per permettere al kernel di caricare il processo. le sezioni rimangono come appendici descrittive alla fine del file (nella Section Header Table).
Il dettaglio fondamentale è che il kernel ignora totalmente le sezioni nell'a.out. Sono lì esclusivamente come cortesia del linker per permettere a disassemblatori (objdump), debugger (gdb) o tool di analisi (readelf -S) di mappare simbolicamente il binario. Se prendo il mio a.out e lancio il comando strip -s a.out, disintegro e rimuovo fisicamente buona parte della Section Header Table e delle symbol tables dal disco: l'eseguibile peserà meno, objdump diventerà quasi cieco, ma il programma continuerà a funzionare perfettamente, perché il kernel legge unicamente i segmenti.

Sezioni fondamentali della Section View:
- .text: il codice assembly eseguibile (mappato nel PT_LOAD con permessi RX).
- .rodata: dati di sola lettura, come costanti ,literal string o tabelle di salto degli switch (permessi R).
- .data: variabili globali(file-scope) o statiche(cioe' locali a una funzione o static a livello di file) entrambe inizializzate a valori non nulli (mappato nel PT_LOAD con permessi RW).
- .bss (Block Started by Symbol): variabili statiche o globali non inizializzate o inizializzate a zero. Lo standard C (ISO C) garantisce rigorosamente che qualsiasi variabile con static storage duration (sia essa globale o dichiarata static) che non viene inizializzata esplicitamente dallo sviluppatore debba essere inizializzata automaticamente a zero all'avvio del programma (0 per gli interi, 0.0 per i float, NULL per i puntatori).Fisicamente sul disco, nel file eseguibile,la sezione .bss non occupa spazio,quindi ha p_filesz = 0. Esiste solo come metadato descrittivo in RAM(p_memsz > p_filesz). Quando il kernel mappa il segmento in memoria tramite mmap, legge la differenza tra p_memsz e p_filesz e alloca ed azzera fisicamente blocchi di RAM (zero-fill pages). Quindi sul disco è solo un'istruzione di dimensione per la MMU, mentre in RAM diventa materia effettiva. A livello di Section Header Table la sezione .bss è contrassegnata dal tipo SHT_NOBITS, un flag esplicito che dice al linker e ai tool di analisi che questa sezione non occupa byte fisici nel file, confermando ulteriormente quanto indicato dai Program Headers.
- .symtab (Symbol Table): un array contiguo di struct Elf64_Sym,ciascuna delle quali e' per un entry,cioe' un simbolo. che elenca tutti i simboli definiti o referenziati nel modulo (funzioni, variabili globali), associandoli al loro indirizzo o offset e dimensione. Fornisce al linker tutti i metadati cruciali per identificare, collegare o rilocare i blocchi di memoria. A livello ABI, un Simbolo è un'etichetta identificativa associata a uno specifico indirizzo di memoria (codice o dati) che deve essere visibile e manipolabile dal linker per risolvere incroci tra file oggetto o librerie. Non tutto ciò che si scrive in C diventa un simbolo ELF:
  - Funzioni (main inclusa): presenti,diventano simboli di tipo STT_FUNC, puntando all'indirizzo della loro prima istruzione in .text.
  - Variabili Globali e Statiche: presenti,diventano simboli di tipo STT_OBJECT, mappando lo spazio allocato in .data o .bss.
  - Costanti Globali (const int): presenti, sono simboli STT_OBJECT associati alla sezione .rodata.
  - Variabili Locali (Stack): assenti. Non esistono in .symtab. A livello hardware, una variabile locale è solo un'allocazione effimera di memoria gestita tramite aritmetica del frame pointer ([RBP - 0x10]). Conservano un nome testuale solo se si aggiungono esplicitamente le sezioni DWARF di debug (flag -g), ma restano invisibili al linker e assenti dalle tabelle ELF canoniche.
  - Macro (#define) e Immediati: Assenti. Scompaiono integralmente nel preprocessing (tradotti in valori letterali integrati negli opcode macchina).
  - Definizioni di struct: assenti. La definizione astratta di un tipo di dato (il suo template) serve solo al compilatore frontend per validare i cast e calcolare gli offset di memoria. Solo una istanza fisica globale di quella struct genererà una entry STT_OBJECT nella tabella, la cui dimensione equivarrà al sizeof dell'intera struttura.
La symbol table non memorizza i nomi per intero, ma punta semplicemente all'offset numerico dei caratteri all'interno della .strtab per ottimizzare lo spazio. Le specifiche ELF separano i metadati numerici dal testo per mantenere ogni singola struct Elf64_Sym rigorosamente a 24 byte: questo permette ai tool e al linker di iterare l'intera tabella a complessità O(1) sfruttando la pura aritmetica dei puntatori (symtab[i]), senza dover parsare o scavalcare stringhe di lunghezza variabile.
Struttura di una singola Entry in .symtab (Elf64_Sym),peso 24 bytes:
  - st_name: un intero a 4 byte che funge da offset. Punta all'indice numerico esatto all'interno della sezione .strtab in cui inizia la stringa zero-terminated del nome.
  - st_value: l'indirizzo virtuale (nell'eseguibile finale) o l'offset (nel file rilocabile) in cui è allocato il simbolo.
  - st_size: la dimensione fisica in byte (es. la quantità di byte dell'opcode per una funzione, o 4 byte per un dato uint32_t).
  - st_info: un singolo byte che, diviso tramite maschere bitwise, codifica sia il Tipo (Oggetto, Funzione, Sezione) sia il Binding (Globale, Locale, Weak). I Tipi di Binding (STB - Symbol Binding). Il binding definisce la "visibilità" e la priorità di un simbolo agli occhi del linker quando deve risolvere gli incroci tra decine di file oggetto (.o) diversi:
    - STB_LOCAL (Locale): simboli invisibili al di fuori del file oggetto in cui sono definiti. In C, corrispondono a tutto ciò che dichiaro con la keyword static (funzioni o variabili globali). Il linker non li userà mai per risolvere riferimenti esterni (extern) provenienti da altri file. Posso avere 100 file .c diversi che dichiarano una funzione locale static void init(), e il linker non genererà alcun conflitto.
    - STB_GLOBAL (Globale): simboli visibili a tutti i moduli del programma. In C, corrispondono alle normali dichiarazioni di funzioni e variabili globali. Risolvono attivamente le dipendenze esterne. Regola ferrea architetturale: se il linker trova in due file oggetto diversi due simboli STB_GLOBAL con lo stesso identico nome, il processo esplode con il fatale errore di multiple definition.
    - STB_WEAK (Debole): e' un simbolo globale "sacrificabile". In C si dichiara usando __attribute__((weak)). Se il linker si ritrova a dover unire due file in cui esiste un simbolo WEAK e un simbolo GLOBAL con lo stesso nome, non genera alcun errore: scarta silenziosamente la definizione debole e collega tutto a quella globale. Se trova solo quella debole, usa quella. È una tecnica usatissima dalle librerie (come la libc) per fornire un'implementazione di default di una funzione, garantendo allo sviluppatore la libertà di riscriverla (sovrascriverla) nel proprio codice senza mandare in crash la compilazione.
  - st_other: specifica la visibilità (es. Default, Hidden per impedire l'esportazione nelle librerie dinamiche).
  - st_shndx: l'indice della Section Header a cui appartiene il simbolo (es. punta alla riga della tabella che definisce .text se è una funzione).
- .strtab(String Table): una tabella,o meglio una gigantesca matrice, di stringhe testuali in formato zero-terminated(cioe' separata dal byte nullo \0) che contiene i nomi reali dei simboli (es. "main", "printf").
- .shstrtab(Section Header String Table): contiene solo e unicamente i nomi testuali delle sezioni stesse (es. .text, .data, .bss, .symtab, .shstrtab). Se guardo una riga della Section Header Table (Elf64_Shdr), c'e' un campo chiamato sh_name. Quel campo è banalmente un intero (un offset in byte). Quando readelf -S legge quel numero, va nella sezione .shstrtab, conta quel numero di byte e legge la stringa corrispondente (es. .text).

Nei binari di produzione symtab e strtab sono spesso rimosse (stripped) per offuscare i nomi delle funzioni e ridurre le dimensioni.
Ecco un esempio di codice C chirurgico per dissezionare esattamente come ogni elemento viene mappato nella .symtab dell'eseguibile.

        C
        // 1. Variabile globale standard
        int var_globale = 10; 

        // 2. Variabile globale statica
        static int var_globale_statica = 20; 

        // 3. Funzione statica
        static void mia_funzione() {
            // 4. Variabile locale standard
            int loc_normale = 1;
            
            // 5. Variabile locale statica
            static int loc_statica = 2;
        }

        // 6. Funzione standard
        int main() {
            // 7. Variabile locale del main
            int loc_main = 3;
            
            mia_funzione();
            return 0;
        }
Ecco l'esatta autopsia della .symtab generata dall'assembler e dal linker per questo codice:

  1) var_globale
  Binding ELF: STB_GLOBAL
  Tipo e Sezione: STT_OBJECT, allocata in .data.
  Motivo: Essendo dichiarata fuori da tutto senza static, è visibile all'intero programma. Il linker la registra come globale affinché altri file .o possano collegarsi ad essa usando extern int var_globale;.
  2) var_globale_statica
  Binding ELF: STB_LOCAL
  Tipo e Sezione: STT_OBJECT, allocata in .data.
  Motivo: È scritta a livello globale (file-scope), ma l'uso di static in questo contesto agisce sul linkage. Dice al compilatore: "Nascondi questo simbolo al di fuori di questo specifico file .c". Il linker la inserisce in .symtab come STB_LOCAL. Se un altro file tenta di fare un extern su di essa, il linker darà errore di undefined reference.
  3) mia_funzione
  Binding ELF: STB_LOCAL
  Tipo e Sezione: STT_FUNC, allocata in .text.
  Motivo: Esattamente come la variabile precedente. static su una funzione ne confina la visibilità al solo file sorgente in cui è scritta. Il linker le assegna STB_LOCAL impedendo ad altri file oggetto di invocarla.
  4) loc_normale
  Binding ELF: ASSENTE. Non esiste nella tabella dei simboli.
  Motivo: È una variabile automatica. A livello hardware non ha un indirizzo di memoria fisso; viene creata dinamicamente sullo Stack della CPU spingendo giù il registro RSP (sub rsp, 4) e manipolata tramite offset del frame pointer ([rbp - 4]). Poiché cessa di esistere quando la funzione termina, il linker non ha alcun motivo di conoscerla o tracciarla.
  5) loc_statica
  Binding ELF: STB_LOCAL
  Tipo e Sezione: STT_OBJECT, allocata in .data.
  Motivo: Qui static cambia significato. Non agisce sulla visibilità (che rimane confinata dentro le parentesi graffe della funzione per il compilatore C), ma altera la storage duration. Dice al compilatore: "Non metterla sullo Stack effimero, ma salvala in RAM permanente". Fisicamente viene stoccata in .data (accanto a var_globale). Affinché il codice possa trovarla a runtime, il compilatore deve per forza generare un simbolo ELF. Lo marca come STB_LOCAL (spesso storpiandone il nome in .strtab, es. loc_statica.1234) in modo che nessun'altra parte del programma all'infuori di quella funzione possa accedervi.
  6) main
  Binding ELF: STB_GLOBAL
  Tipo e Sezione: STT_FUNC, allocata in .text.
  Motivo: È una funzione standard. Deve essere STB_GLOBAL perché deve essere rintracciabile dal file oggetto di startup del sistema C (crt1.o), il quale contiene l'entry point logico _start che esegue la chiamata al tuo main.
  7) loc_main
  Binding ELF: ASSENTE.
  Motivo: Identico a loc_normale. È spazzatura temporanea sullo Stack, invisibile all'architettura ELF.
Questa chiarificazione coglie la differenza che esiste in C riguardo ai due tipi di utilizzo della keyword 'static':
- a livello globale (File-Scope): restringe il linkage (la visibilità logica) del simbolo al solo Translation Unit (il file .c che sto compilando). Marca l'ELF come STB_LOCAL nella .symtab.
Il linker lo "vede", ma sa che non deve usarlo per risolvere eventuali riferimenti esterni (extern) provenienti da altri file .o.
- a livello locale (Function-Scope): modifica la storage duration (il ciclo di vita della memoria) della variabile. Invece di essere distrutta alla fine della funzione (come succede alle normali variabili allocate sullo Stack), sopravvive per tutta la durata del programma. Il valore di inizializzazione viene assegnato una sola volta (a compile-time o al primissimo avvio del programma), e le chiamate successive alla funzione modificheranno o leggeranno sempre la stessa cella di memoria, ignorando l'inizializzazione originaria. Rimuove la variabile dallo Stack effimero (invisibile ai metadati) e la promuove fisicamente in .data (se inizializzata a un valore non zero) o in .bss (se non inizializzata o inizializzata a zero). Per farla trovare al programma, il compilatore le crea un simbolo STB_LOCAL nella .symtab, spesso alterandone il nome interno (name mangling) per evitare conflitti con altre eventuali variabili static locali chiamate allo stesso modo in altre funzioni.

A tal proposito,vale la pena ricordare che lo standard ISO C definisce esattamente quattro tipi di scope per gli identificatori. Lo scope è un costrutto puramente sintattico e astraente che esiste solo nel frontend del compilatore (fase di parsing/AST) per risolvere lo shadowing dei nomi e i cicli di vita temporanei. Non appena il codice viene tradotto in Assembly, lo scope svanisce: rimangono solo offset calcolati rispetto a RBP/RSP (per le variabili di blocco) o veri e propri simboli ELF pronti per il linker (per entità a file scope o statiche). Inoltre, c'è un errore concettuale diffuso: quella che la maggior parte dei programmatori chiama "function scope" in riferimento alle variabili locali, in realtà è il block scope. Eccoli:
1) Block scope (Scope di blocco): si applica a qualsiasi identificatore dichiarato all'interno di una coppia di parentesi graffe { } o nella lista parametri della definizione di una funzione. La visibilità termina alla graffa di chiusura. Le normali variabili locali di una funzione possiedono un block scope, non un function scope.
2) File scope (Scope di file): si applica agli identificatori dichiarati al di fuori di qualsiasi blocco o lista di parametri (variabili globali e dichiarazioni di funzioni). La visibilità va dal punto di dichiarazione fino alla fine del file sorgente (EOF).
3) Function scope (Scope di funzione): in C, questo scope è riservato esclusivamente alle label del goto. Una label è l'unico identificatore visibile in qualsiasi punto all'interno dell'intera funzione in cui è definita, indipendentemente dal livello di annidamento dei blocchi. Nessuna variabile locale possiede un function scope. Le etichette (label) per il goto (es. cleanup_memory:) sono in sostanza le uniche entità in tutto il linguaggio C a ignorare completamente le parentesi graffe. Se mi trovo all'interno di tre cicli for annidati e si verifica un errore critico, potrei voler saltare a una routine di pulizia posta alla fine della funzione. L'istruzione goto cleanup_memory; esegue un salto in avanti (forward jump) verso un'etichetta che si trova fuori dal blocco corrente e che non è stata ancora dichiarata nel flusso di lettura sequenziale. Se le label del goto avessero un normale Block Scope, non potrei saltare liberamente dentro o fuori dai blocchi annidati. Per permettere la traduzione in salti assembly incondizionati (jmp), il compilatore C innalza le etichette al Function Scope: il nome della label diventa univoco e visibile in qualsiasi riga, blocco o annidamento all'interno della funzione in cui è definita, dal prologo all'epilogo. La pessima fama del goto deriva dalle crociate di Dijkstra degli anni '60 per prevenire lo spaghetti code. Tuttavia, nel C moderno di sistema (e il kernel Linux ne è letteralmente tappezzato), il goto è la best practice architettonica per l'error handling e il cleanup delle risorse, noto come pattern "Single Exit Point". Invece di annidare dieci if o duplicare le chiamate a free() in ogni ramo d'errore, esegui un goto cleanup_memory; verso il fondo della funzione, deallochi tutto in ordine inverso e ritorni pulito. È la simulazione più efficiente dei blocchi try/catch/finally e produce un control-flow graph molto più snello per il compilatore.
4) Function prototype scope: si applica ai nomi dei parametri usati esclusivamente nella dichiarazione di un prototipo di funzione (non nella sua implementazione). La visibilità termina alla parentesi tonda di chiusura.Quando scrivo in un header file: 

void elabora_dati(int larghezza, int altezza);
I nomi larghezza e altezza non allocano memoria e non esistono al di fuori di quelle due parentesi. Nascono alla parentesi tonda di apertura e vengono distrutti dal parser alla parentesi di chiusura. Fino allo standard C89, questo scope era del tutto inutile, tanto che la prassi era omettere i nomi: void elabora_dati(int, int);. Dal C99 in poi, con l'introduzione dei VLA (Variable Length Arrays) passati come argomenti, il Function Prototype Scope è diventato un vincolo matematico per il parser. Prendiamo un prototipo del tipo :

void trasforma_matrice(int righe, int colonne, int matrice[righe][colonne]);
In questo caso, omettere i nomi è impossibile. Il compilatore frontend deve memorizzare i nomi righe e colonne all'interno di questo minuscolo scope temporaneo per poter valutare la geometria dell'array matrice. Non appena incontra la parentesi ), quei nomi svaniscono nel nulla, garantendo che non vadano mai in conflitto con eventuali macro o variabili globali omonime presenti nel resto del file sorgente. È utile per sintassi come i VLA (Variable Length Arrays) passati come parametri, es: void calc(int len, int array[len]);.Introdotti nel C99, i VLA permettono di dimensionare un array a runtime (es. int array[n];),cioe' commettono l'eresia di allocare la memoria sullo Stack invece che sull'Heap,manipolando dinamicamente lo Stack spostando in basso il registro RSP in tempo di esecuzione. A differenza dei classici array statici(int array[100]) dove la dimensione e' nota a compile time,per cui il compilatore sa esattamente quanti byte allocare sulla stack,il cui frame viene quindi preparato in modo deterministico e sicuro(nel prologo della funzione, genera un'istruzione assembly con un numero hardcodato e immutabile: sub rsp, 100), il VLA e' uno strano ibrido tra gli array statici e quelli dinamici in cui,a livello assembly, il compilatore non può più hardcodare la dimensione del frame. Deve generare un codice che prende il valore della variabile n, lo moltiplica per 4, e lo sottrae dinamicamente al registro RSP in tempo di esecuzione (sub rsp, rax). Gli VLA sono universalmente condannati per la totale assenza di error handling: se provo a dichiarare  un array statico enorme come int array[999999999], il compilatore se ne accorge durante la compilazione e mi blocca o avvisa; se malloc fallisce mi restituisce NULL e posso gestire l'errore; se invece la n di un VLA è troppo grande (magari sporcata da un input utente,dalla lettura di un file corrotto o un input malevolo),il registro RSP (il puntatore allo stack) verrà sparato istantaneamente a decine di megabyte di distanza, sfondando la "Guard Page" del kernel e il limite rigido dello Stack (spesso 8MB, stack smash) e atterrando in una zona di memoria non mappata o riservata ad altre librerie, causando un Segmentation Fault o aprendo le porte a exploit di tipo Stack Clash usati per la privilege escalation. Il disastro è stato tale che il C11 li ha declassati a feature opzionale e Linus Torvalds li ha bannati per sempre dai sorgenti del kernel.


* Infine i dati grezzi sono i byte binari puri (gli opcode delle istruzioni macchina o i valori binari grezzi delle variabili/costanti, che confluiscono nelle sezioni .text per il codice, .data per le variabili, .rodata per le costanti, ecc.) memorizzati nel file, privi di sovrastrutture di metadati, ma semplicemente puntati dagli offset delle tabelle.


## GCC VS CC VS CLANG

- gcc (GNU Compiler Collection): e' lo storico compilatore di riferimento del mondo GNU/Linux. Supporta molteplici linguaggi (C, C++, Fortran) e utilizza un backend basato su rappresentazioni intermedie (RTL e GIMPLE).
- clang: e' il frontend nativo per l'infrastruttura di compilazione LLVM, scritto in C++, celebre per la modularità, la pulizia dei messaggi d'errore e la velocità di elaborazione.
- cc (C Compiler): e' tradizionalmente un symlink o un wrapper POSIX standard che punta al compilatore di sistema predefinito (storicamente gcc, mentre nei sistemi moderni o su macOS viene spesso rimappato direttamente su clang).
- GNU Binutils: un pacchetto software fondamentale di utility di basso livello per la manipolazione di binari e file oggetto. Include strumenti chiave come as (l'assembler), ld (il linker), objdump (disassemblatore), readelf (analizzatore di strutture ELF), nm (analizzatore di simboli), strip (rimozione dei metadati di debug) e ar (gestore di archivi statici .a). ld appartiene ufficialmente alle GNU Binutils : GCC non include il codice sorgente di ld al suo interno, ma lo invoca esternamente come fase finale (il "linking driver") per produrre l'eseguibile.


## PIPELINE DI GENERAZIONE DI UN ESEGUIBILE ELF: DA C AL BINARIO

La trasformazione di un sorgente in un ELF pronto per l'esecuzione avviene tramite la toolchain standard (es. GCC/Clang + GNU Binutils) che si occupa di tutte le fasi della pipeline di generazione richiamando internamente i singoli moduli (preprocessor, parser LLVM/GCC, assembler e linker).

1) Compilazione (Frontend & Backend): il codice C viene parsato (AST), ottimizzato tramite rappresentazioni intermedie (GIMPLE, RTL) e tradotto in assembly x86-64 puro. Possiamo identificare almeno 3 fasi differenti:
- Preprocessing (cpp): risoluzione delle macro (#define),inclusione degli header (#include),rimozione dei commenti. L'output e' ancora codice C, ma preprocessato(file con estensione .i).
- Compilazione (cc1, Frontend): analisi lessicale, sintattica e semantica del codice C, traduzione in un Abstract Syntax Tree (AST) e generazione della rappresentazione intermedia (IR),come Gimple e RTL. Gimple e' un'IR ad alto livello basata su alberi semplificati e codice a tre indirizzi (3-address code), usata per applicare ottimizzazioni indipendenti dalla CPU (es. loop unrolling, rimozione del codice morto). RTL (Register Transfer Language) e' un'IR a basso livello simile a un assembly virtuale con un numero infinito di registri virtuali, usata dal backend per tradurre il codice nell'architettura hardware specifica (es. x86-64).
Entrambe sono strutture dati in memoria scritte interamente in C (il linguaggio con cui è implementato il sorgente di GCC).
- Ottimizzazione e generazione del codice (cc1, Backend): elaborazione delle ottimizzazioni indipendenti e dipendenti dalla CPU e traduzione finale in codice Assembly specifico per l'architettura target(file con estensione .s).

2) Assemblaggio (as): l'assembler traduce le mnemoniche Assembly in opcode binari. L'output è un file oggetto rilocabile (.o in Linux), un ELF di tipo ET_REL. Un ET_REL generato dall'assembler contiene già tutte le mnemoniche tradotte in opcode macchina per le istruzioni locali del file sorgente, ma non possiede gli indirizzi di memoria definitivi per qualsiasi riferimento esterno (variabili globali esterne, funzioni importate tramite extern, o chiamate a librerie). Per questi simboli, l'assembler inserisce placeholder azzerati come offset 0x0 e compila una tabella di rilocazione (.rel.text o .rela.text) che istruisce il linker su come calcolare e patchare quegli indirizzi in futuro. Un file ET_REL è costituito interamente da sezioni separate (es. .text, .data, .bss, .rodata, .rela.text). Il linker esiste proprio per prendere queste sezioni separate da più file .o e fonderle insieme.
L'ET_REL inizia fisicamente con l'ELF Header (Elf64_Ehdr) impostato con e_type = ET_REL, seguito immediatamente dai dati grezzi delle sezioni (codice macchina, dati) e si conclude con la Section Header Table (fondamentale per i file oggetto, poiché il linker lavora esclusivamente con la vista a sezioni). Non possiede una Program Header Table (PHDR), poiché i file oggetto non vengono mai caricati direttamente in memoria dal kernel.

3) Linking Statico (collect2 / ld): gcc invoca un wrapper chiamato collect2, il quale a sua volta chiama il linker ld. Quest'ultimo prende in pasto multipli ET_REL .o (e archivi .a). Unisce le sezioni omologhe (tutte le .text insieme, tutte le .data insieme), risolve i simboli incrociati e gli indirizzi e applica le formule matematiche definite nelle rilocazioni per assegnare gli indirizzi di memoria virtuale definitivi.

4) Generazione dell'eseguibile: il risultato è un ELF di tipo ET_EXEC (eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza) o ET_DYN (Position Independent Executable - PIE, lo standard moderno). In un ET_DYN, gli indirizzi generati dal linker non sono assoluti, ma relativi al registro RIP, permettendo al kernel di caricare il binario in un punto casuale della RAM (ASLR - Address Space Layout Randomization).

Cosa accade in caso di compilazione temporanea di piu' sorgenti .c? ogni volta che un compilatore traduce un file sorgente con l'opzione -c, l'assembler produce esattamente un file oggetto rilocabile (.o / ET_REL). Ciascun file .o possiede le proprie sezioni .text, .data, ecc. Il linker (ld) prende tutte le sezioni omologhe di tutti i file oggetto e le fonde insieme in macro-sezioni uniche nell'eseguibile finale (tutte le .text di tutti i file .o vengono concatenate in un'unica grande .text continua). Questa macro-sezione viene poi inserita nel rispettivo segmento PT_LOAD.


## COSA ACCADE QUANDO ESEGUO UN ESEGUIBILE BINARIO

La shell esegue un fork() e poi chiama execve sull'eseguibile("./a.out", ...). Il kernel apre il file, legge i primi byte e identifica il magic number \x7fELF. L'esecuzione passa all'handler load_elf_binary() all'interno del kernel source (fs/binfmt_elf.c). fs/binfmt_elf.c è il file sorgente del kernel Linux che implementa il gestore del formato ELF e che contiene la funzione cardine load_elf_binary(). Quando lancio un eseguibile, il kernel invoca questa funzione di bootstrap e parsing per leggere l'header ELF, allocare le strutture di task (mm_struct), mappare i segmenti PT_LOAD tramite la system call interna di mmap, configurare lo stack iniziale del processo e preparare il salto all'entry point.
Il kernel itera l'array dei Program Headers(mapping): per ogni PT_LOAD trovato, invoca un mmap() per copiare/associare quel blocco del file fisico nelle pagine di RAM del nuovo processo(Segmentazione), applicando i bit di protezione MMU specificati in p_flags (RX per il codice, RW per i dati).
La MMU (Memory Management Unit) e' l'unità hardware della CPU che traduce gli indirizzi virtuali in indirizzi fisici tramite le tabelle delle pagine (Page Tables) e applica i bit di protezione della memoria (RWX).
La AGU (Address Generation Unit) e' invece l' unità hardware dedicata esclusivamente al calcolo degli indirizzi di memoria complessi per le istruzioni di load/store (es. basati su registri base, indice e scala: [rax + rbx*4 + 0x123]), sgravando le ALU principali.

Quale e' la differenza tra un binario statico e un binario dinamico? Il binario statico non contiene segmenti PT_INTERP. Il kernel imposta il registro RSP (lo stack con argv ed envp) e salta direttamente all'indirizzo e_entry letto dall'ELF Header. In caso di binario dinamico(SOTA per Linux) esiste un segmento PT_INTERP che punta a ld-linux.so. ld-linux.so non fa parte del sorgente del kernel, ma e' un eseguibile condiviso (userspace) che risiede sul filesystem (es. in /lib64/ld-linux-x86-64.so.2). so sta per Shared Object(Oggetto condiviso).Il kernel mappa in memoria non solo il mio eseguibile, ma anche l'interprete (il linker dinamico). Invece di saltare all'e_entry del mio file, il kernel salta all'e_entry del linker dinamico.
C è un linguaggio compilato in codice macchina nativo e non ha bisogno di un interprete nel senso di una macchina virtuale (come Python o Java). Tuttavia, i moderni eseguibili Linux sono quasi sempre dinamici, ovvero dipendono da librerie condivise esterne (come la libc.so.6). Le librerie condivise sono file binari contenenti funzioni precompilate (come la libc) che non vengono incorporate staticamente nel codice dell'eseguibile, ma vengono mappate una sola volta in memoria RAM e condivise contemporaneamente tra tutti i processi in esecuzione che ne hanno bisogno. Per consentire a un eseguibile di risolvere tali dipendenze a runtime, il file ELF incorpora un puntatore a un componente esterno: il linker dinamico (/lib64/ld-linux-x86-64.so.2). Nelle specifiche ELF, questo componente viene formalmente denominato Interprete (PT_INTERP). Di conseguenza, quando eseguo un binario dinamico, il kernel mappa in memoria sia l' eseguibile sia il linker dinamico, passando inizialmente il controllo a quest'ultimo affinché risolva i simboli e prepari l'ambiente, prima di cedere definitivamente l'esecuzione al punto d'ingresso del mio programma.
Il linker dinamico(ld.so) si attiva a runtime, analizza il segmento PT_DYNAMIC dell' eseguibile, mappa in memoria tutte le librerie condivise necessarie (es. libc.so.6), risolve i simboli essenziali, popola la GOT (Global Offset Table) e la PLT (Procedure Linkage Table) per consentire le chiamate a funzioni esterne, e infine cede il controllo saltando all' e_entry (che solitamente punta a _start, la quale inizializza la libc e infine chiama main()). GOT (Global Offset Table)e' una tabella di puntatori situata nel segmento dati (RW) che contiene gli indirizzi di memoria effettivi delle funzioni esterne e delle variabili globali, risolti a runtime dal linker dinamico. PLT (Procedure Linkage Table) e' invece una tabella di stub di codice eseguibile (RX) che permette al codice di saltare verso funzioni esterne (es. printf). Poiché con i binari PIE e l'ASLR gli indirizzi cambiano a ogni avvio, la PLT funge da tramite: interroga la GOT; se la funzione non è ancora stata risolta, attiva il linker dinamico (ld.so) per risolverla al primo giro (lazy binding), memorizzando poi l'indirizzo nella GOT per tutte le chiamate successive.
GOT e PLT sono strutturalmente parte dell'eseguibile, che contiene sezioni dedicate (.plt e .got / .got.plt) generate dal linker dinamico. Esistono come dati sul disco (nei segmenti PT_LOAD) e vengono caricati direttamente in RAM dal kernel al momento dell'esecuzione. Ogni processo ha il suo spazio di indirizzamento virtuale isolato, quindi possiede le sue istanze private di GOT e PLT in RAM (sebbene le pagine delle librerie condivise sottostanti siano mappate in condivisione fisica a livello di kernel).

Nei binari moderni Position Independent Executable (ET_DYN), gli indirizzi risolti dal linker non sono assoluti (fissi), ma relativi al registro RIP (Instruction Pointer). Quando si parla di indirizzamento relativo al RIP, si riferimento all'istruzione attualmente in esecuzione. Poiché il kernel carica il binario in un punto casuale della RAM (ASLR), il registro RIP punta dinamicamente a quell'indirizzo base effettivo; di conseguenza, il codice calcola gli offset interni sommando o sottraendo la distanza relativa rispetto alla posizione corrente del RIP, garantendo il funzionamento del programma indipendentemente dalla locazione fisica in memoria.
Il motivo per cui funziona è esattamente perché il RIP cambia in continuazione. L'indirizzamento RIP-relative non usa indirizzi assoluti (fissi), ma spostamenti relativi (offset) calcolati in base alla posizione corrente del processore.
ESEMPIO:
Se scrivo un'istruzione che deve leggere una variabile o saltare a una funzione vicina in un binario PIE il compilatore non genera un'istruzione con un indirizzo fisso del tipo mov rax, [0x401050], piuttosto una del tipo mov rax, [rip + 0x1234]. A runtime, la CPU esegue l'istruzione: prende il valore attuale del registro RIP (che punta all'istruzione successiva) e ci somma l'offset costante 0x1234.
Poiché l'offset in byte tra l'istruzione e la variabile è matematicamente fisso (indipendentemente da dove il kernel decide di caricare il programma in RAM grazie all'ASLR), la somma RIP + offset punterà sempre esattamente al bersaglio corretto. Il RIP cambia ad ogni ciclo di clock, ma la distanza relativa tra le parti del codice rimane immutata.

Linker statico e dinamico sono dunque due software completamente distinti che operano in momenti temporali e contesti operativi opposti: 
- lo statico (ld / GNU Binutils) e' un tool di sviluppo che gira sulla macchina di compilazione (offline). Prende i file oggetto .o generati dall'assembler, risolve i simboli interni, calcola le rilocazioni e assembla il file ELF finale sul disco. Una volta terminata la compilazione, il compito di ld è esaurito e il binario e' pronto.
- il linker Dinamico (ld-linux-x86-64.so.2 / ld.so) e' una libreria di sistema (un eseguibile condiviso) che risiede fisicamente sul sistema operativo target (es. in /lib64/). Entra in azione a runtime: nel momento esatto in cui eseguo un binario dinamico, il kernel Linux legge l'header PT_INTERP, scopre che il binario ha bisogno di ld.so, mappa in memoria sia il tuo programma sia il linker dinamico, e cede inizialmente il controllo a quest'ultimo. ld.so si occupa di aprire le librerie condivise (.so), caricarle in RAM, risolvere i simboli esterni mancanti (popolando la GOT) e solo allora avviare l'esecuzione del codice.

Che differenza c'e' allora tra funzione extern e funzione condivisa (dynamic)? extern è una keyword di dichiarazione logica a livello di linguaggio C e Assembly che dice al compilatore di non generare un indirizzo definitivo per quel simbolo, lasciando un segnaposto per il linker. La funzione condivisa è l'entità binaria reale che risiede all'interno di una libreria esterna (.so). Quando dichiaro una funzione extern e questa appartiene a una libreria esterna (es. printf della libc), il linker inserisce una voce nella PLT/GOT, demandando al linker dinamico (ld-linux.so) il compito di risolverla a runtime agganciando l'indirizzo reale.


## READELD E OBJDUMP

Readelf e objdump sono due analizzatori binari appartenenti alla suite GNU Binutils, ma differiscono radicalmente nel motore di parsing e nell'obiettivo d'uso. La differenza architetturale chiave risiede nella dipendenza dalla libreria BFD (Binary File Descriptor).

* Readelf è un parser nativo e indipendente. Non utilizza la libreria BFD, il che significa che legge unicamente le strutture dati grezze definite in <elf.h>. È lo strumento di elezione per l'analisi chirurgica dei metadati (header, segmenti, sezioni, rilocazioni) perché mostra esattamente ciò che è scritto sul disco, senza astrazioni o correzioni silenziose. Supporta esclusivamente file in formato ELF.
* Objdump è un'interfaccia costruita sopra la libreria BFD. Poiché BFD funge da strato di astrazione universale, objdump è in grado di leggere molteplici formati binari (ELF, PE/COFF per Windows, Mach-O per macOS, a.out, b.out). Il suo scopo primario non è l'analisi dei metadati ELF (dove risulta meno preciso di readelf a causa del layer BFD), ma il disassemblaggio del codice macchina in istruzioni leggibili e l'ispezione mista (sorgente/assembly).

# Tipi di File Analizzabili

Entrambi i tool operano sulla stessa famiglia di artefatti generati dalla toolchain C/C++, ma con focus differenti:

- File oggetto rilocabili (.o / ET_REL): per ispezionare sezioni .text non ancora linkate e tabelle di rilocazione .rela .
- Eseguibili (ET_EXEC e ET_DYN/PIE): i binari finali generati dal linker.
- Librerie condivise (.so / ET_DYN): per mappare i simboli esportati e la PLT/GOT.
- Archivi statici (.a): archivi ar contenenti multipli file .o.
- Core dumps (ET_CORE): generati dal kernel al crash del processo, analizzati principalmente con readelf per mappare lo stato dei registri e i segmenti di memoria (PT_NOTE) salvati al momento del segfault.

# Analisi delle flag di readelf

Le flag di readelf sono progettate per dissezionare le tabelle logiche e fisiche dell'eseguibile. L'aggiunta della flag -W (--wide) è sempre mandatoria nei sistemi a 64-bit per impedire che il terminale tronchi gli indirizzi a 8 byte mandando a capo l'output.

### Ispezione degli Header principali

* -h (--file-header): stampa l'ELF Header (Elf64_Ehdr). Rivela il magic number, l'endianness, l'entry point (e_entry) e l'offset delle due tabelle principali (Program e Section Header).
* -l (--program-headers o --segments): stampa la Program Header Table (Elf64_Phdr). Fondamentale per vedere la Execution View usata dal kernel, il mapping dei segmenti PT_LOAD, i loro permessi (R, W, E) e l'allineamento.
* -S (--section-headers o --sections): stampa la Section Header Table (Elf64_Shdr). Mostra la Linking View, l'indirizzo base, l'offset fisico e i flag (alloc, execute, write) di sezioni come .text, .data, .bss.
* -e (--headers): combina -h, -l e -S in un unico output.

### Analisi dei Simboli e Rilocazioni

* -s (--syms o --symbols): esegue il dump della tabella dei simboli statica (.symtab). Mostra funzioni e variabili, la loro dimensione e a quale sezione appartengono (o UND se esterne).
* --dyn-syms: mostra esclusivamente i simboli dinamici (.dynsym), ovvero quelli necessari al linker dinamico a runtime.
* -r (--relocs): stampa le tabelle di rilocazione (es. .rela.text in un .o, o le rilocazioni PLT/GOT in un eseguibile dinamico). Indica gli offset da patchare e le formule matematiche associate.
* -d (--dynamic): mostra il contenuto del segmento PT_DYNAMIC. Indica le librerie condivise richieste (NEEDED, es. libc.so.6) e l'array di inizializzazione.

### Estrazione dei dati grezzi (Payload & Debug)

* -x <numero_o_nome_sezione> (--hex-dump): stampa un dump esadecimale e ASCII del contenuto puro di una sezione (es. readelf -x .rodata a.out).
* -p <numero_o_nome_sezione> (--string-dump): stampa il contenuto di una sezione interpretando i byte come stringhe terminate da null (zero-terminated). Usato su .strtab o .shstrtab.
* -n (--notes): decodifica i segmenti o le sezioni di tipo NOTE. Fondamentale nell'analisi dei core dump o per leggere l'ABI-tag.


## Analisi delle flag di objdump

Le flag di objdump sono orientate al reverse engineering del codice eseguibile e all'analisi dell'opcode.

### Controllo del Disassemblatore

* -d (--disassemble): smonta solo le sezioni che si suppone contengano codice eseguibile (tipicamente .text, .init, .plt). Converte l'opcode binario in assembly.
* -D (--disassemble-all): forza il disassemblaggio di tutte le sezioni, indipendentemente dai flag della sezione. Fondamentale nell'analisi di malware o packer, dove istruzioni valide potrebbero essere nascoste in sezioni non eseguibili (es. .data).
* -M <flavor> (--disassembler-options): modifica la sintassi. In x86-64, il default è att (AT&T, opcode sorgente, destinazione, prefissi % e $). Usare -M intel converte l'output in sintassi Intel (opcode destinazione, sorgente, memoria tra parentesi quadre).
* -S (--source): intercala il codice sorgente (es. in C) al codice assembly smontato. Richiede che il binario sia stato compilato con i simboli di debug (flag -g in GCC/Clang).

### Estrazione Dati e Header (BFD-based)

* -f (--file-headers): mostra le informazioni generali dell'header in formato agnostico BFD (non ELF puro). Riporta l'architettura target e i flag di stato del file (es. HAS_SYMS, D_PAGED).
* -h (--section-headers): stampa la tabella delle sezioni. È l'equivalente BFD di readelf -S, ma formattato diversamente e limitato da ciò che la libreria BFD considera una "sezione".
* -s (--full-contents): genera un hexdump affiancato ad ASCII di tutte le sezioni che contengono dati fisici sul disco (ignora .bss). Simile a readelf -x, ma su tutto il file.
* -j <nome_sezione> (--section): filtra l'output di -d o -s per operare esclusivamente su una sezione target (es. objdump -d -j .text -M intel a.out).

### Simboli e Rilocazioni a Runtime

* -r (--reloc): stampa le rilocazioni inline al disassemblaggio. Utilissimo sui file oggetto (.o) per capire quale simbolo esterno sta rimpiazzando i placeholder 0x0 nel codice assembly.
* -R (--dynamic-reloc): mostra esclusivamente le rilocazioni dinamiche usate dal linker a runtime (la tabella delle entries della GOT che verranno popolate da ld.so).
* -t (--syms): stampa la tabella dei simboli in formato BFD, indicando flags logici che specificano se il simbolo è globale (g), locale (l), una funzione (F) o un dato (O).
* -T (--dynamic-syms): equivalente a -t, ma per i simboli esportati dinamicamente.

## PERCHE' UN MAIN ANCHE BANALE GENERA DECINE DI SEZIONI E SEGMENTI DIVERSI?

Quando compilo anche il programma più banale, GCC/Clang avvolgono il main() all'interno di un'enorme infrastruttura di sistema precompilata: il C Runtime (CRT), il framework per il linking dinamico e le moderne mitigazioni di sicurezza del kernel Linux. Un main con un banale include della libreria stdio.h e un write di una stringa arriva a contenere fino a 31 sezioni nella linking view e 13 segmenti nella execution view.
Le 31 sezioni derivano dal fatto che il linker fonde il file oggetto con una serie di librerie standard e moduli di avvio:
* il Boilerplate del C Runtime (CRT): GCC compila il main, ma l'entry point reale punta a _start. Per gestire questo, il linker ha importato staticamente nel binario i file oggetto di sistema crt1.o, crti.o e crtn.o. Questi file aggiungono codice boilerplate e iniettano sezioni come .init, .fini, .init_array e .fini_array, che servono a eseguire i costruttori globali prima che il mio main parta, e i distruttori quando faccio un return o chiami exit().
* l'infrastruttura dinamica (PIE): dal momento che il binario è compilato come ET_DYN (PIE) e utilizza una libreria condivisa (write è in realtà un wrapper in C importato dalla libc.so.6), il compilatore deve costruire tutte le tabelle per permettere al linker dinamico (ld.so) di risolvere quell'indirizzo in RAM. Questo genera un mare di sezioni: .interp, .dynamic, .dynsym (simboli dinamici), .dynstr (stringhe dinamiche), le hash table per velocizzare la ricerca (.hash / .gnu.hash), le rilocazioni (.rela.dyn, .rela.plt) e ovviamente .plt (Procedure Linkage Table) e .got / .got.plt (Global Offset Table).
* unwinding dello stack: anche in C puro, i moderni compilatori includono le tabelle DWARF per il frame-unwinding, necessarie per funzioni come backtrace(), per il debugging o per la cancellazione dei thread POSIX. Questo crea le sezioni .eh_frame e .eh_frame_hdr.
* note e metadati: il linker inserisce firme digitali o hash per debuggare il binario. Genera .note.gnu.build-id (l'hash univoco della compilazione) e .note.ABI-tag (la versione minima del kernel Linux supportata).
* gestione stringhe e costanti: .rodata per le costanti (come "stringa"), .shstrtab (i nomi testuali delle sezioni stesse) e .strtab.

In passato, un binario Linux aveva essenzialmente due o tre segmenti PT_LOAD (uno per il codice RX, uno per i dati RW). Oggi, le policy di sicurezza richiedono una separazione iper-granulare dei permessi di pagina (W^X rigoroso), spingendo il linker a segmentare la memoria in modo quasi ossessivo.

* 1. I 4 Segmenti PT_LOAD moderni: per impedire esecuzioni di codice malevolo, la memoria viene divisa in 4 blocchi fisici distinti, ciascuno con permessi MMU restrittivi:

* PT_LOAD **R--**: contiene l'ELF header iniziale, i Program Headers e le note.
* PT_LOAD **R-X**: contiene esplicitamente solo il codice eseguibile (.init, .text, .plt, .fini).
* PT_LOAD **R--**: contiene i dati a sola lettura post-rilocazione (costanti, .rodata, .eh_frame).
* PT_LOAD **RW-**: contiene i dati mutabili (.data, .bss, e le parti scrivibili della .got).

* 2. I Segmenti di Protezione GNU:

* PT_GNU_STACK: un segmento con permessi **RW-**. Non contiene dati fisici sul disco (filesz = 0), ma serve a comunicare esplicitamente al kernel che lo stack del tuo processo non deve essere eseguibile (prevenendo attacchi buffer overflow). Senza questo header, per retrocompatibilità, il kernel Linux lascerebbe lo stack eseguibile.
* PT_GNU_RELRO: (RELocation Read-Only). Indica a ld.so che, dopo aver risolto gli indirizzi a runtime (es. write), alcune pagine della GOT e di .dynamic devono essere immediatamente bloccate in modalità Read-Only per prevenire attacchi di GOT Overwrite.
* PT_GNU_PROPERTY: configura mitigazioni hardware moderne a livello di CPU, come l'Intel CET (Control-flow Enforcement Technology) o gli shadow stacks.
* PT_GNU_EH_FRAME: un segmento dedicato unicamente a far trovare rapidamente la tabella .eh_frame_hdr al sistema per l'unwinding.

* 3. I Segmenti Strutturali:

* PT_PHDR: dice al kernel dove caricare la tabella stessa dei Program Headers in memoria.
* PT_INTERP: passa la stringa /lib64/ld-linux-x86-64.so.2 al kernel.
* PT_DYNAMIC: punta alla tabella dinamica per ld.so.
* 2 o più PT_NOTE: usati per mappare in RAM le note generate in fase di link (build-id, ABI-tag).

Se voglio vedere la differenza tra il mio codice e l'infrastruttura di sistema, posso riscrivere il file C invocando la syscall pura tramite inline assembly (syscall invece di chiamare write, per sbarazzarmi della libc) e compilando con il comando:
gcc -nostdlib -static file.c

Nostdlib dice a GCC (e di riflesso al linker ld) di non linkare la libreria standard del C (libc) e non includere i file oggetto di startup del sistema (crt1.o, crti.o, crtn.o).

Questo escluderà il linking dinamico, il C runtime, la libc e tutti i boilerplate di GCC. Eseguendo di nuovo readelf -h, vedro' l'overhead crollare a pochissime sezioni e segmenti.
Quando scrivo di usare syscall,non intendo la funzione fornita da unistd.h,perche' e' anch' essa un costrutto della libc, ma l'istruzione hardware syscall sfruttando l'assembly inline di GCC, rispettando rigorosamente la System V ABI per x86-64.

Ecco un esempio di sorgente C definitivo, ridotto all'osso, in cui non abbiamo più a disposizione printf, malloc, strlen, né alcun wrapper. Poiché elimino crt1.o, elimino anche la funzione di inizializzazione di sistema che va a chiamare il main(). Ecco perché nel codice ho dovuto chiamare direttamente _start: è il simbolo che il linker cerca di default come e_entry dell'header ELF.
Rimuovendo il C Runtime (CRT), spazzo via tutte le sezioni .init, .fini, .init_array, .eh_frame e i segmenti associati. Siamo in un ambiente freestanding puro:

// Nessun #include. Non ci servono header della libc.
// Sostituiamo main() con _start, il vero entry point richiesto dal linker
void _start() {
    const char msg[] = "a\n";
    
    // 1. Syscall: sys_write (RAX = 1)
    __asm__ volatile (
        "syscall"
        : // Nessun output
        : "a" (1),          // RAX = 1 (sys_write)
          "D" (1),          // RDI = 1 (stdout)
          "S" (msg),        // RSI = puntatore al buffer
          "d" (2)           // RDX = lunghezza (2 byte)
        : "rcx", "r11", "memory" // Registri sporcati dall'istruzione syscall
    );

    // 2. Syscall: sys_exit (RAX = 60)
    // Se non chiamo sys_exit, la CPU continuerà a eseguire spazzatura oltre la fine
    // della funzione _start, causando un Segmentation Fault.
    __asm__ volatile (
        "syscall"
        : 
        : "a" (60),         // RAX = 60 (sys_exit)
          "D" (0)           // RDI = 0 (exit code)
        : "rcx", "r11", "memory"
    );
}

Static dice al linker di risolvere tutte le dipendenze al momento della compilazione e incorporare il codice macchina di tutto ciò che serve direttamente dentro il file binario.
In questo esempio specifico (dove non usiamo funzioni esterne), dice semplicemente di non preparare il terreno per le librerie dinamiche. Static elimina completamente il linker dinamico. Spariscono il segmento PT_INTERP (addio ld-linux.so), il segmento PT_DYNAMIC, la PLT (.plt), la GOT (.got) e tutte le tabelle dei simboli dinamici (.dynsym, .dynstr, .hash). L'eseguibile passa dal tipo ET_DYN (PIE) al tipo ET_EXEC (eseguibile assoluto base fisso).

Se compilo il codice soprastante con `gcc -nostdlib -static file.c` e poi lancio `readelf -l a.out`, i 13 segmenti originari saranno magicamente crollati a soli 2 o 3 segmenti PT_LOAD, e le 31 sezioni a malapena 4 o 5 (.text, .rodata, .symtab, .strtab, .shstrtab).


## SYSCALL MMAP
Contenuta nella libreria <sys/mman.h>, a livello kernel mmap (Memory Mapped) altera le page tables della CPU per mappare fisicamente le pagine di un file su disco all'interno dello spazio di indirizzamento virtuale del mio processo. 

```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

```

### Argomenti

1. `addr`: l'indirizzo di memoria in cui mappare il file sul disco.
Passando NULL chiedo al kernel di trovare autonomamente un'area libera nella memoria virtuale. E' possibile anche forzare manualmente un indirizzo,ma il rischio e' quello di collisioni con la memoria allocata per altri processi.
2. `length` : la dimensione in bytes della memoria da mappare. 
3. `prot` (protezione hardware): setta i permessi della Memory Management Unit (MMU) per le pagine mappate.
4. `flags` (comportamento della memoria): decide chi vede le modifiche al file:
* MAP_SHARED: se scrivi nell'array in memoria, il kernel aggiorna fisicamente il file sul disco.
* MAP_PRIVATE: innesca il Copy-On-Write; le scritture avvengono solo in RAM e non modificano il file originale su disco.
5. `fd`:l'intero restituito dalla chiamata a open().
6. `offset`: da quale punto del file iniziare a mappare. Nel 99% dei casi, e sicuramente per parsare un ELF, si passa 0 (l'inizio esatto del file). L'offset deve essere sempre un multiplo della page size del sistema,tipicamente 4096 bytes.

La funzione inversa è munmap(void *addr, size_t length). Distrugge la mappatura e rilascia le pagine al kernel. Operazione obbligatoria prima di chiudere il programma per un cleanup impeccabile.

### Il Valore di Ritorno

Se ha successo, mmap restituisce il puntatore generico void * all'inizio dell'area allocata in RAM.
In caso di fallimento, mmap NON restituisce NULL,ma imposta una macro specifica chiamata MAP_FAILED (che a livello macchina equivale a (void *)-1).
Se faccio error handling con if (ptr == NULL), il programma crasherà: c'e' bisogno rigorosamente di verificare if (ptr == MAP_FAILED).

0/0 = R
R(0) = R
R(n) = n / 0
R(n) * 0 --> n
n = a * b
R(n) = R(a) * R(b)
7 = 7R * 0
6 = 6R * 0
6 = 2R * 0 * 3R * 0