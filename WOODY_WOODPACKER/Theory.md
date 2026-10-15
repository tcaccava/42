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
Il formato ELF (Executable and Linkable Format) è lo standard architetturale de facto per i file binari nei sistemi operativi UNIX-like (Linux, *BSD, Solaris). È definito nelle specifiche System V Application Binary Interface (ABI).

Non si tratta di un semplice contenitore di codice macchina, ma di un database relazionale strutturato che fornisce al sistema operativo e al linker dinamico tutte le topografie di memoria e i metadati necessari per caricare ed eseguire un processo, oppure per collegare moduli oggetto multipli.

Il concetto cardine per padroneggiare l'architettura ELF è il Dual View Paradigm. Un file ELF possiede due strutture sovrapposte, lette da attori diversi in momenti diversi:

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

Tutto in un file ELF a 64-bit fa riferimento ai tipi di base definiti in <elf.h>, come Elf64_Addr (8 byte), Elf64_Off (8 byte), Elf64_Word (4 byte), Elf64_Half (2 byte).

ELF Header (Elf64_Ehdr)
ELF Header (Elf64_Ehdr): rappresenta il punto di partenza per l'analisi statica della struttura del file. Campi chiave da gestire:
- e_entry: Indirizzo virtuale del punto di ingresso iniziale (OEP - Original Entry Point).   
- e_phoff: Offset nel file in cui inizia la Program Header Table.
- e_phnum: Numero di entry presenti nella Program Header Table.
- e_shoff e e_shnum: Offset e conteggio delle Section Headers.
situato sempre all'offset 0x0 del file. È la "stele di Rosetta" che descrive dove trovare il resto delle tabelle. Occupa 64 byte.

e_ident[16]: I primi 4 byte sono il Magic Number (\x7F E L F). I successivi indicano la classe (32 o 64 bit), l'endianness (LSB/MSB) e la versione dell'ABI.

e_type: Definisce il ruolo del file (ET_REL = 1, ET_EXEC = 2, ET_DYN = 3, ET_CORE = 4).

e_machine: L'architettura target (per x86-64 è EM_X86_64 = 0x3E).

e_entry: L'indirizzo virtuale (o l'offset nel caso di PIE) della prima istruzione da cui la CPU deve iniziare l'esecuzione (spesso _start, non main).

e_phoff / e_shoff: Offset fisici assoluti dal byte zero in cui iniziano rispettivamente la Program Header Table e la Section Header Table.

e_phnum / e_shnum: Il numero di entry contenute nelle suddette tabelle.




Program Headers (Elf64_Phdr): descrivono i segmenti da mappare in memoria fisica/virtuale tramite l'loader del kernel (fs/binfmt_elf.c).
- PT_LOAD: unico segmento caricato in memoria dal kernel.
- p_offset: offset del segmento all'interno del file su disco.
- p_vaddr: indirizzo virtuale a cui il segmento deve essere mappato in memoria.
- p_filesz: dimensione del segmento sul disco.
- p_memsz: dimensione del segmento occupata in memoria ram (se $p\_memsz > p\_filesz$, la differenza definisce la sezione .bss, inizializzata a zero).
- p_flags: permessi del segmento (PF_R, PF_W, PF_X)
L'Execution View. È un array di strutture (56 byte ciascuna) situate immediatamente dopo l'ELF Header. Descrive al kernel quali blocchi del file devono essere caricati in RAM e con quali permessi.

Program Header Table (Elf64_Phdr array)
I campi critici di un Elf64_Phdr sono:

p_type: Il tipo di segmento. Il più importante è PT_LOAD (1). Solo i segmenti PT_LOAD vengono effettivamente mappati in memoria dal kernel tramite system call mmap. Altri tipi critici sono PT_INTERP (contiene il percorso del linker dinamico, es. /lib64/ld-linux-x86-64.so.2) e PT_DYNAMIC (metadati per il linking a runtime).

p_offset: L'offset del segmento all'interno del file su disco.

p_vaddr: L'indirizzo virtuale a cui mappare il segmento.

p_filesz: I byte occupati dal segmento sul disco.

p_memsz: I byte da riservare in RAM. Se p_memsz > p_filesz, la differenza viene allocata ed azzerata dal kernel: è il meccanismo con cui nasce la sezione .bss (dati non inizializzati, che non occupano spazio su disco per non sprecare byte).

p_flags: Permessi della Memory Management Unit (MMU). PF_X (1), PF_W (2), PF_R (4).

p_align: Granularità di allineamento, solitamente la dimensione della pagina di memoria (es. 0x1000 per 4KB o 0x200000 per le Huge Pages). p_vaddr modulo p_align deve essere uguale a p_offset modulo p_align.



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

Linking Statico (ld): Il linker prende in pasto multipli ET_REL (e archivi .a). Unisce le sezioni omologhe (tutte le .text insieme, tutte le .data insieme), risolve i simboli incrociati e applica le formule matematiche definite nelle rilocazioni per assegnare gli indirizzi di memoria virtuale definitivi.

Generazione dell'Eseguibile: Il risultato è un ELF di tipo ET_EXEC (eseguibile ad indirizzo base fisso, deprecato per ragioni di sicurezza) o ET_DYN (Position Independent Executable - PIE, lo standard moderno). In un ET_DYN, gli indirizzi generati dal linker non sono assoluti, ma relativi al registro RIP, permettendo al kernel di caricare il binario in un punto casuale della RAM (ASLR - Address Space Layout Randomization)