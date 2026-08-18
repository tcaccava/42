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
- .rodata (Read-Only Data): contiene costanti, come le stringhe letterali (es. "Hello\n").
- direttiva global <simbolo>: esporta l'etichetta affinché sia visibile dal linker C . Per es. global ft_strlen dice 
  all'assemblatore di scrivere questo indirizzo in una tabella di esportazione(Symbol Table) nel file .o. Quando il compilatore C compila il main.c, trova 
  una chiamata a ft_strlen ma non ha il codice della funzione. La direttiva extern serve per far dire al file che quel codice arrivera' da "fuori"(extern). 
  Alla fine, il Linker (ld) prende tutti i file .o, collega le etichette esportate dal .s con la chiamata pendente nel .c e genera l'eseguibile.
- direttiva extern <simbolo>: dichiarazione di un simbolo fornito esternamente, come malloc o il puntatore di sistema a errno. 
  Extern in sostanza lascia uno spazio vuoto(Relocation Entry) segnalando il nome della funzione esterna da cercare. Il Linker incrociera' le due direttive e
  riempira' quel buco ricalcolando gli offset.

---COMPILAZIONE, C E ASSEMBLY----------------------------------------------------------------------- 
Il flusso completo di un programma C è questo:
1. Preprocessore C (cpp): legge i file .c e risolve tutte le direttive che iniziano per # (#include, #define). 
   I file .S (maiuscola) passano per questa fase (puoi usare #define in Assembly), i file .s (minuscola) no.
2. Compilatore (cc1): legge il C e genera codice Assembly intermedio. Perché passa per l'Assembly? Perché il compilatore 
   lavora su astrazioni: generare codice Assembly permette al compilatore di ottimizzare il flusso logico 
   del programma prima di tradurlo in crudo binario.
3. Assemblatore (as o nasm): prende l'Assembly e genera i file oggetto (.o). I file .o contengono codice macchina puro, 
   ma mancano gli indirizzi di memoria di qualsiasi simbolo(funzioni o variabili globali) che non sia definito in quel file sorgente(es. printf).
4. Linker (ld): fonde assieme C e Assembly; prende tutti i file oggetto .o e le librerie esterne, risolve gli indirizzi vuoti e impacchetta tutto nel file 
   eseguibile finale (ELF su Linux). Il file a.out (Assembler OUTput) e' in formato ELF(Executable and Linkable Format).
A differenza di C, dove il compilatore gestisce l'allocazione delle variabili locali e lo stack frame, in Assembly si ha il controllo diretto sui registri 
della CPU e sugli indirizzi di memoria. 

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
In Assembly, se fai mov [rax], 5, la CPU ha in mano solo un indirizzo di memoria: non sa se scriverci il 5 in formato 1 byte (0x05), 
2 byte (0x0005), o 4 byte (0x00000005). Ecco perché va specificato esplicitamente: mov byte [rax], 5 oppure mov dword [rax], 5 .
Se la dimensione dell'operazione è ambigua (es. quando si lavora con costanti immediate), occorre specificare l'operatore di 
dimensione,cioe' l'ampiezza della memoria manipolata:
-byte (1 byte / 8 bit)
-word (2 byte / 16 bit)
-dword (4 byte / 32 bit)
-qword (8 byte / 64 bit)

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
 In un registro da 256 bit posso entrare fino a otto float da 32 bit e si puo' sommarli tutti a un altro registro in un singolo ciclo di clock.
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
   Se faccio la divisione Assembly div rbx, la CPU assume implicitamente che il dividendo si trovi nei registri RDX e RAX (per formare 
   un numero a 128 bit,il doppio del divisore), e piazza il risultato in RAX e il resto in RDX. Prima di una divisione con segno (idiv), se il divisore è a 64 bit, 
   si usa l'istruzione cqo (Convert Quadword to Octaword) per estendere il bit di segno di RAX su tutti i 64 bit di RDX.
2. RBX (Base): registro generale callee-saved usato anticamente per offset di memoria.
3. RCX (Counter): usato nei cicli hardware(istruzione loop o rep). È il 4° argomento nelle funzioni C. lOOP 
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
del registro, l'hardware è cablato per cancellare istantaneamente con degli zeri i 32 bit superiori(zero extension), azzerando le dipendenze hw e accelerando l'esecuzione.

---REGISTRI SPECIALI---------------------------------------------------------------------------------
Registri Speciali, non general purpose:
-RSP (stack pointer): mantiene l'indirizzo dell'ultimo elemento inserito in cima allo stack.
-RBP (base/frame pointer): utilizzato storicamente per ancorare il frame corrente della funzione sullo stack.
-RIP (instruction pointer): mantiene l'indirizzo di memoria dell'istruzione corrente in fase di esecuzione.
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
   di ingresso (_start). Quando il kernel esegue la syscall execve per lanciare il programma, NON carica tutto l'eseguibile in RAM,ma crea solo la mappa della memoria (Virtual Memory).
   Il codice viene caricato in RAM "on-demand" (a blocchi di 4096 byte, o Page) solo quando la CPU tenta di eseguirlo(demand caching). 
   Il main in C non è la prima cosa eseguita: il file ELF specifica un indirizzo hardware di partenza chiamato tipicamente _start (fornito
   dalla libreria C) che prepara gli argomenti e poi chiama il main. Da lì, la CPU incrementa automaticamente RIP dopo aver fetchato ogni istruzione in base 
   alla lunghezza in byte dell'istruzione stessa.
2. RSP (Stack Pointer): e' un general purpose,l'hardware lo usa implicitamente ogni volta che usi le istruzioni push, pop, call o ret,che si interfacciano 
   con la memoria stack. Contiene l'indirizzo dell'ultimo(msb) byte occupato in cima allo stack.
3. RBP (Base Pointer) : e' storicamente usato per salvare una copia statica di RSP all'inizio di una funzione.In sostanza all' inizio di una funzione si fa push rbp, 
   poi mov rbp, rsp. Essendo RBP fisso, permette di trovare facilmente le variabili locali e gli argomenti sullo stack a offset costanti (es. [rbp - 8] per la prima 
   variabile locale, - 16 per la seconda e cosi' via), anche se RSP cambia durante la funzione.

---STACK E ISTRUZIONI RSP---------------------------------------------------------------------------
La ram e' divisa in diverse sezioni logiche mappate dal kernel:
-.text: codice eseguibile (permessi di sola lettura ed esecuzione).
-.data / .bss*`: variabili globali.
- heap: memoria allocata dinamicamente (es. malloc). Parte da un indirizzo basso (sinistra) e cresce verso destra (indirizzi crescenti).
- stack : parte da un indirizzo altissimo (destra estrema) e cresce verso sinistra (indirizzi decrescenti).

In un architettura x86-64, la memoria dello stack,se immmaginiamo la ram come un enorme riga di celle da un msb 0x0 posto a sinistra ad un lsb 0xFFFFFFFFFFFFFFFF
posto a destra(architettura little endian), cresce verso sinistra, cioe' verso indirizzi decrescenti.
Le istruzioni PUSH,POP,CALL e RET sono mnemonici Assembly a cui corrispondono istruzioni opcode ISA dirette che manipolano automaticamente RSP e la memoria puntata da esso:
-l'istruzione PUSH SRC(es. push rax) decrementa RSP di 8 byte (RSP = RSP - 8),cioe' lo sposta a sinistra e scrive gli 8 bytes contenuti in src all'indirizzo [RSP].
-l'istruzione POP DEST(es. pop rbx) legge gli 8 bytes all'indirizzo puntato  da [RSP], li mette nel registro di destinazione dest e incrementa RSP di 8 byte (RSP = RSP + 8),
 cioe' lo fa ritrarre verso destra.
-l'istruzione CALL <LABEL>: una label in assembly (es. ft_strlen) è solo un segnaposto testuale che il compilatore rimpiazza con un vero indirizzo di memoria. 
 Spinge implicitamente(push rip) l'indirizzo dell'istruzione successiva (RIP, 8 byte) sullo stack e salta a label(jmp label).Mentre la CPU esegue call, RIP è già stato incrementato 
 e punta all'istruzione successiva al call. Il call esegue segretamente un push rip (salva l'indirizzo a cui tornare sullo stack) e poi mette l'indirizzo della label dentro RIP.
-l'istruzione RET esegue implicitamente un pop rip, quindi preleva gli 8 byte(l'indirizzo di memoria)che call aveva lasciato in cima allo stack ponendoli in RIP per tornare 
 al chiamante. L'esecuzione riprendera' da dove si era interrotta.
 
---MEMORY ALIGNMENT E CHIAMATE A FUNZIONE-----------------------------------------------------------
Quando una funzione scritta in C ne chiama un'altra (o quando chiamiamo una funzione di libreria da Assembly), entrambe devono rispettare una convenzione binaria 
universale o calling convention(ABI- Application Binary Interface). Sui sistemi UNIX/Linux/macOS x86-64 si applica la System V AMD64 ABI. 
La System V ABI stabilisce che, al momento dell'esecuzione dell'istruzione call verso un'altra funzione, il registro RSP deve essere allineato a un multiplo di 16 byte (RSP % 16 == 0).
Un indirizzo allineato a 16 byte significa che è un multiplo intero di 16,quindi in binario gli ultimi 4 bit devono essere zero(16 & 15 == 0). In notazione esadecimale ogni carattere rappresenta 
esattamente un nibble di 4 bit. Quindi, qualsiasi indirizzo multiplo di 16 termina col carattere 0 (es. 0x7ffd9b8a0).
Prima che il chiamante esegua call, lo stack è allineato a 16 byte. L'istruzione call spinge il return address (8 byte) sullo stack.
All'ingresso della funzione, RSP NON è più allineato a 16 byte (risulta sfalsato di 8 byte: RSP % 16 == 8).Pertanto, prima di eseguire  una sotto-chiamata (call malloc, call write, ecc.),
si deve ripristinare l'allineamento a 16 byte nello stack riservando memoria (es. sottraendo byte da RSP o effettuando un numero dispari di push). Un disallineamento provocherà Segmentation Fault 
imprevedibili  all'interno di funzioni di libreria C che usano istruzioni SIMD/SSE. La libreria standard C (glibc) per funzioni come malloc o printf usa pesantemente estensioni SIMD/Vectoriali per 
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

---CALLER, CALLEE E REGISTRI------------------------------------------------------------------------
Il Chiamante (Caller) e' la funzione che sta eseguendo, per esempio il main. Il Chiamato (Callee) e' la funzione che viene invocata, ad esempio una chiamata ad ft_strlen nel main 
o in un altra funzione. In virtu' di questa distinzione, i registri sono divisi in due categorie operative: 
-Callee-Saved Registers (preservati dal chiamato / non-volatili): RBX, RBP, R12, R13, R14, R15.
 Se una funzione intende modificare uno di questi registri, ha l'obbligo di salvarne il valore originale sullo stack all'inizio della funzione (prologo) e ripristinarlo 
 prima di ritornare al chiamante (epilogo). Esempio: se il main ha un dato in RBX, e chiama ft_strlen, la convenzione ABI (System V) stabilisce che ft_strlen (il chiamato) non può 
 alterarlo in modo permanente. Se ft_strlen vuole usare RBX per i suoi scopi, deve pusharlo sullo stack, usarlo, e popparlo prima di fare ret.
-Caller-saved registers (preservati dal chiamante / volatili): RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11.
 Una funzione può sovrascrivere liberamente questi registri. Una qualsiasi chiamata di funzione esterna (es. call malloc o call write) potrebbe sovrascrivere indisturbata 
 tutti questi registri! Esempio: se il main ha un valore importante in RAX, e chiamo ft_strlen, quando ft_strlen ritorna, RAX sarà stato sovrascritto dal suo valore di ritorno. 
 E' responsabilità del chiamante salvarlo nello stack prima del call.

---DIFFERENZA TRA FUNZIONI DI LIBRERIA E SYSCALL----------------------------------------------------
Mentre le chiamate a funzioni C impiegano l'istruzione call, le chiamate di sistema(System Calls) dirette al kernel Linux (come read e write) impiegano l'istruzione hardware speciale ISA syscall. 
L' utente (Ring 3) non puo' interagire con l'hardware del PC (scrivere su schermo, aprire un file, mappare memoria). Lo vietano i circuiti della CPU. Solo l'OS (Linux, Ring 0, modalità privilegiata) può farlo.
Per scavalcare questo limite, l'ISA possiede l'istruzione syscall. Questa istruzione genera un interrupt hardware (trap): blocca la  esecuzione del codice user space, concede poteri assoluti al kernel Linux, il quale esegue la 
richiesta in base al numero che viene caricato nel registro RAX (es. 0 = sys_read, 1 = sys_write, 9 = mmap... ce ne sono circa 350) ,cioe' l'identificatore numerico della syscall (registry ID syscall) .
I registri in cui sono salvati gli argomenti delle syscall sono leggermente differenti da quelli delle funzioni di libreria:
-1° Argomento: RDI
-2° Argomento: RSI
-3° Argomento: RDX
-4° Argomento: R10 (DIVERSO: le funzioni C usano RCX, ma l'istruzione syscall usa R10). Syscall necessita di usare RCX internamente per salvare temporaneamente l'indirizzo RIP aaaaaaaaaaaaaaaaaaaa cui tornare. Siccome `RCX` viene distrutto dall'istruzione, il kernel Linux ha deciso che il 4° argomento della syscall deve risiedere in `R10`, a differenza delle funzioni C che usano `RCX`.
-5° Argomento: R8
-6° Argomento: R9
Effetti collaterali hardware: l'istruzione syscall sovrascrive internamente i registri RCX e R11.

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


7. Analisi Architetturale dei Requisiti di Progetto
L'analisi seguente mappa le funzioni richieste dal soggetto sui rispettivi pattern algoritmici di livello registro.  
PDF

Parte Obbligatoria
1. ft_strlen (const char *s)
Argomenti: RDI contiene il puntatore alla stringa.

Logica Architetturale:

Inizializzare un registro indice/contatore a 0 (es. RAX).

Eseguire un ciclo di scansione della memoria: accedere al singolo byte situato a [RDI + RAX].

Confrontare il byte letto con il valore nullo (0x00).

Se è zero, interrompere il ciclo; altrimenti incrementare RAX e ripetere.

Ritorno: RAX contiene la lunghezza trovata.

2. ft_strcpy (char *dst, const char *src)
Argomenti: RDI puntatore a destinazione, RSI puntatore a sorgente.

Logica Architetturale:

Usare un registro d'appoggio per l'indice (es. RCX = 0).

Leggere un byte dalla sorgente ([RSI + RCX]) usando un registro a 8-bit (es. AL).

Scrivere il byte letto nella destinazione ([RDI + RCX]).

Confrontare il byte copiato (AL) con 0x00. Se diverso da zero, incrementare l'indice e continuare.

Ritorno: RAX deve contenere il puntatore originario alla destinazione (RDI).

3. ft_strcmp (const char *s1, const char *s2)
Argomenti: RDI puntatore a s1, RSI puntatore a s2.

Logica Architetturale:

Per garantire la conformità allo standard POSIX C, i caratteri devono essere trattati come unsigned char.

Leggere un byte da s1 e un byte da s2 in registri a 8-bit.

Effettuare la Zero-Extension dei byte a 32/64 bit (tramite istruzioni come movzx) per evitare interferenze dal segno durante la sottrazione.

Confrontare i byte. Se differiscono o se si incontra il byte nullo (0x00), calcolare la differenza aritmetica (s1_char - s2_char) ponendola in RAX.

4. ft_write (int fd, const void *buf, size_t count) e ft_read (int fd, void *buf, size_t count)
Argomenti: RDI (fd), RSI (buf), RDX (count).

Logica Architetturale:

Caricare in RAX l'ID della syscall (1 per write, 0 per read su Linux).

Invocare l'istruzione hardware syscall.

Valutare se RAX indica errore (<0). In caso affermativo, instradare l'esecuzione verso la procedura di gestione d'errore errno descritta nel Capitolo 6.  
PDF

5. ft_strdup (const char *s)
Argomenti: RDI puntatore alla stringa sorgente.

Logica Architetturale:

Calcolare la lunghezza della stringa (tramite chiamata interna a ft_strlen).

Incrementare la lunghezza di 1 (per includere il terminatore nullo \0).

Preservare il puntatore sorgente RDI in un registro callee-saved (es. RBX) o sullo stack, per evitare che venga distrutto da chiamate successive.

Garantire l'allineamento dello stack a 16 byte e chiamare malloc (RDI deve contenere la dimensione calcolata).  
PDF

Se malloc restituisce NULL (RAX == 0), ritornare immediatamente 0.

Copiare la stringa originale nella nuova memoria allocata restituita da malloc e ritornare il puntatore in RAX.

Parte Bonus: Layout delle Strutture Dati e Liste Concatenate
Il soggetto definisce la struttura C della lista concatenata:  
PDF

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