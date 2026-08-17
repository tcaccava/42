/*
L'Assembly è una rappresentazione simbolica intellegibile dall'uomo del codice macchina ,cioe' di quella lunga sequenza 
di bytes binari ,detti opcode, che rappresentano istruzioni che la cpu e' in grado di eseguire direttamente.
Il  compilatore/assemblatore (NASM) e' responsabile della traduzione del codice Assembly in machine code.
Il NASM (Netwide Assembler) e' il programma che prende il testo Assembly (es. mov rax, 1) e lo traduce negli opcode binari 
(es. B8 01 00 00 00). Questo binario è proprio il codice macchina ISA (Instruction Set Architecture).
Il sorgente Assembly ha formato .s, dove s per Source: e' l'estensione storica per i file assembly scritti a mano, in contrapposizione 
ai .S (maiuscolo) che passano prima dal preprocessore C. Il file source contiene diverse sezioni:
- .text: contiene il codice eseguibile (read-only, executable).
- direttiva global <simbolo>: esporta l'etichetta affinché sia visibile dal linker C (es. global ft_strlen). Per es. global ft_strlen dice 
  all'assemblatore di segnare questo indirizzo in una tabella di esportazione nel file .o. Quando il compilatore C compila il main.c, trova 
  una chiamata a ft_strlen ma non ha il codice. La direttiva extern serve per far dire al file che quelo codice arrivera' da "fuori". Alla fine,
  il Linker (ld) prende tutti i file .o, collega le etichette esportate dal .s con la chiamata pendente nel .c) e genera l'eseguibile.
- direttiva extern <simbolo>: dichiarazione di un simbolo fornito esternamente, come malloc o il puntatore di sistema a errno. 
Il flusso completo di un programma C è questo:

1. Preprocessore C (cpp): legge i file .c e risolve tutte le direttive che iniziano per # (#include, #define). 
   I file .S (maiuscola) passano per questa fase (puoi usare #define in Assembly), i file .s (minuscola) no.
2. Compilatore (cc1): legge il C e genera codice Assembly intermedio. Perché passa per l'Assembly? Perché il compilatore 
   lavora su astrazioni :generare codice Assembly permette al compilatore di ottimizzare il flusso logico 
   del programma prima di tradurlo in crudo binario.
3. Assemblatore (as o nasm): prende l'Assembly e genera i file oggetto (.o). I file .o contengono codice macchina puro, 
   ma gli indirizzi di memoria delle funzioni esterne (es. printf) sono lasciati vuoti.
4. Linker (ld): prende tutti i file oggetto .o e le librerie esterne, risolve gli indirizzi vuoti e impacchetta tutto nel file 
   eseguibile finale (ELF su Linux).

Esiste una corrispondenza quasi 1:1 tra un'istruzione Assembly e la rispettiva istruzione binaria. A differenza di C, 
dove il compilatore gestisce l'allocazione delle variabili locali e lo stack frame, in Assembly si ha il controllo diretto 
sui registri della CPU e sugli indirizzi di memoria.  
La CPU preleva gli opcodes dalla RAM, ma più specificamente, dalla L1i-Cache (Instruction Cache) che specchia porzioni della RAM.
La decodifica avviene in hw nel backend della cpu: circuiti dedicati prendono l'opcode binario e lo scompongono in micro-operazioni (µops). 
A seconda dell'istruzione, l'hardware smista la µop all'unità competente per la sua esecuzione:
-ALU (Arithmetic Logic Unit) per matematica intera e bitwise.
-FPU / SIMD (Floating Point / Vector Unit) per virgola mobile o operazioni su vettori.
-AGU (Address Generation Unit) per calcolare indirizzi di memoria fisici lavorando con la MMU(Memory Managment Unit).

La CPU lavora eseguendo un ciclo continuo:
1)Fetch: il backend preleva l'istruzione corrente dalla memoria ram all'indirizzo contenuto nel registro RIP (Instruction Pointer).
2)Decode: decodifica l'istruzione hardware dividendola in micro-operazioni.
3)Execute: esegue l'operazione nell'ALU o nell'FPU o nell'MMU o nell'NPU.
4)Increment: aggiorna RIP affinché punti all'istruzione successiva.

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

1. RAX (Accumulator): obbligatorio per i valori di ritorno delle funzioni e usato implicitamente in divisioni e moltiplicazioni.
2. RBX (Base): registro generale callee-saved usato anticamente per offset di memoria.
3. RCX (Counter): usato nei cicli hardware(istruzione loop o rep). È il 4° argomento nelle funzioni C.
4. RDX (Data): estensione matematica per divisioni/moltiplicazioni e 3° argomento C.
5. RSI (Source Index) e RDI (Destination Index): nati per operazioni massive (spostare MB di dati) su stringhe. Per es. l'istruzione 
   rep movsb prende RCX come contatore, legge da RSI e scrive su RDI via hardware. Oggi si preferiscono funzioni ottimizzate SIMD, 
   ma si usano questi registri per passare il 1° e 2° argomento nelle chiamate a funzione C per pura convenzione ABI.
6. RBP (Frame Pointer) e RSP (Stack Pointer): gestione dello stack.
7. R8  fino a R15: aggiunti per avere più spazio. R8 e R9 sono il 5° e 6° argomento C. R12-R15 sono callee-saved.





 


* **Matematica Implicita in RAX/RDX**: Se fai la divisione Assembly `div rbx`, la CPU assume implicitamente che il dividendo si trovi incatenato nei registri `RDX` e `RAX` (per formare un numero a 128 bit), e piazza il quoziente in `RAX` e il resto in `RDX`.
* **Maschere Hardware e Partial Register Stall**: Non si usa una maschera software, sono cavi di silicio fisicamente collegati a una parte del registro. Se scrivi 8 bit in `AL`, i vecchi 56 bit di `RAX` restano intatti. Questo è un problema per l'Out-of-Order execution della CPU: se un'istruzione successiva usa l'intero `RAX`, la CPU deve fermarsi e "cucire" i nuovi 8 bit di `AL` con i vecchi 56. Scrivere in un registro a 32 bit (`EAX`) invece attiva un circuito che **azzera** (zero-extension) istantaneamente i 32 bit superiori, rompendo le dipendenze hardware ed eliminando il collo di bottiglia.
* **Caller-Saved vs Callee-Saved (Volatili e Non)**: Hai capito perfettamente. Se `main` (caller) ha un dato in `RAX` (volatile), e chiama `ft_strlen` (callee), sa che lo perderà (sovrascritto dal return). Se voleva salvarlo, doveva farlo lui prima. Se `main` ha un dato in `RBX` (non volatile), la convenzione garantisce che al ritorno della funzione `RBX` avrà lo stesso valore. Se `ft_strlen` vuole usare `RBX`, deve pusharlo, usarlo e popparlo prima di uscire.



La divisione in sottoregistri è pura architettura hardware,ottenuta attraverso il wiring dei registri. Si tratta in sostanza sempre 
della stessa porzione di silicio. AL per es. tocca solo gli 8 bit meno significativi (LSB) di RAX. Non si va sotto gli 8 bit perché il byte è 
la minima unità indirizzabile logicamente in ram.

Un importante effetto collaterale delle scritture nei sottoregistri a 32-bit e' che in x86-64, esse azzerano automaticamente i 32 bit superiori(MSB) del 
corrispondente registro a 64-bit. Le scritture su sotto-registri a 8 o 16 bit ,invece, preservano intatti i bit rimanenti del registro a 64 bit.
Le CPU moderne eseguono le istruzioni Out-of-Order (fuori ordine),cioe' non necessariamente nell'ordine in cui vengono fetchate dal backend e sottoposte al 
frontend. Se si scrive in AL, la CPU non sa se la successiva istruzione userà l'intero registro RAX. Questo crea una Partial Register Stall: 
la CPU deve fermare l'esecuzione per unire i nuovi 8 bit con i vecchi 56 bit (falsa dipendenza).
Per x86-64, AMD ha deciso che le operazioni a 32 bit sono le più frequenti in C (int). Per ottimizzare il calcolo e rompere la dipendenza dal passato 
del registro, l'hardware è cablato per cancellare istantaneamente con degli zeri i 32 bit superiori, azzerando le dipendenze e accelerando l'esecuzione.

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
   l'indirizzo di memoria virtuale della prossima istruzione macchina da eseguire. Quando il sistema operativo (tramite la syscall execve) carica in RAM
   l'eseguibile, la CPU inizializza RIP all'indirizzo del punto di ingresso (_start). Il main in C non è la prima cosa eseguita: il file ELF specifica un 
   indirizzo hardware di partenza chiamato tipicamente _start (fornito dalla libreria C) che prepara gli argomenti e poi chiama il main. Da lì, la CPU incrementa 
   automaticamente RIP dopo aver fetchato ogni istruzione in base alla lunghezza in byte dell'istruzione stessa.
2. RSP (Stack Pointer): e' un general purpose,l'hardware lo usa implicitamente ogni volta che usi le istruzioni push, pop, call o ret,che si interfacciano 
   con la memoria stack. Contiene l'indirizzo dell'ultimo(msb) byte occupato in cima allo stack.
3. RBP (Base Pointer) : e' storicamente usato per salvare una copia statica di RSP all'inizio di una funzione. Essendo fisso, permette di trovare facilmente 
   le variabili locali e gli argomenti sullo stack a offset costanti (es. [rbp - 8]), anche se RSP cambia durante la funzione.

La ram e' divisa in diverse sezioni logiche mappate dal kernel:

-.text: codice eseguibile (permessi di sola lettura ed esecuzione).
-.data / .bss*`: variabili globali.
- heap: memoria allocata dinamicamente (es. malloc). Parte da un indirizzo basso (sinistra) e cresce verso destra (indirizzi crescenti).
- stack : parte da un indirizzo altissimo (destra estrema) e cresce verso sinistra (indirizzi decrescenti).

In un architettura x86-64, la memoria dello stack,se immmaginiamo la ram come un enorme riga di celle da un msb 0x0 posto a sinistra ad un lsb 0xFFFFFFFFFFFFFFFF
posto a destra(architettura little endian), cresce verso sinistra, cioe' verso indirizzi decrescenti.
Le istruzioni PUSH,POP,CALL e RET sono istruzioni ISA fisiche che manipolano automaticamente RSP e la memoria puntata da esso:
-l'istruzione PUSH SRC(es. push rax) decrementa RSP di 8 byte (RSP = RSP - 8),cioe' lo sposta a sinistra e scrive gli 8 bytes contenuti in src all'indirizzo [RSP].
-l'istruzione POP DEST(es. pop rbx) legge gli 8 bytes all'indirizzo puntato  da [RSP], li mette nel registro di destinazione dest e incrementa RSP di 8 byte (RSP = RSP + 8),
 cioe' lo fa ritrarre verso destra.
-l'istruzione CALL <LABEL>: una label in assembly (es. ft_strlen) è solo un segnaposto testuale che il compilatore rimpiazza con un vero indirizzo di memoria. 
 Spinge implicitamente(push rip) l'indirizzo dell'istruzione successiva (RIP, 8 byte) sullo stack e salta a label.Mentre la CPU esegue call, RIP è già stato incrementato 
 e punta all'istruzione successiva al call. Il call esegue segretamente un push rip (salva l'indirizzo a cui 
 tornare sullo stack) e poi mette l'indirizzo della label dentro RIP.
-l'istruzione RET esegue implicitamente un pop rip, quindi preleva gli 8 byte(l'indirizzo di memoria)che call aveva lasciato in cima allo stack ponendoli in RIP per tornare 
 al chiamante. L'esecuzione riprendera' da dove si era interrotta.
 
 

Quando una funzione scritta in C ne chiama un'altra (o quando chiamiamo una funzione di libreria da Assembly), entrambe devono rispettare una convenzione binaria 
universale o calling convention(ABI- Application Binary Interface). Sui sistemi UNIX/Linux/macOS x86-64 si applica la System V AMD64 ABI. 
La System V ABI stabilisce che, al momento dell'esecuzione dell'istruzione call verso un'altra funzione, il registro RSP deve essere allineato a un multiplo di 16 byte (RSP % 16 == 0).
Prima che il chiamante esegua call, lo stack è allineato a 16 byte.L'istruzione call spinge il return address (8 byte) sullo stack.
All'ingresso della funzione, RSP NON è più allineato a 16 byte (risulta sfalsato di 8 byte: RSP % 16 == 8).Pertanto, prima di eseguire  una sotto-chiamata (call malloc, call write, ecc.),
si deve ripristinare l'allineamento a 16 byte nello stack riservando memoria (es. sottraendo byte da RSP o effettuando un numero dispari di push). Un disallineamento provocherà Segmentation Fault 
imprevedibili  all'interno di funzioni di libreria C che usano istruzioni SIMD/SSE. La libreria standard C (glibc) per funzioni come malloc o printf usa pesantemente estensioni SIMD/Vectoriali per 
elaborazioni velocissime sui blocchi. Le istruzioni SIMD dell'ISA (come movaps) esigono che i dati in RAM si trovino ad indirizzi multipli di 16 (indirizzi che finiscono in 0). Se la memoria non è 
allineata a 16 byte, la CPU solleva un hardware fault (Segmentation Fault).Quando il codice esegue call malloc, la call fa implicitamente un push di 8 byte (RIP). Questo significa che dentro malloc, 
lo stack non è più un multiplo di 16, ma è sfalsato di 8. Per evitare che malloc crashi quando usa istruzioni SIMD, l'ABI impone che, prima di chiamare una qualsiasi funzione C, si debba assicurare che 
l' RSP sottratto di 8 (quello che avverrà con la call) risulti essere un multiplo di 16. Questo si fa tipicamente eseguendo una push fittizia o facendo sub rsp, 8 prima della call.


A differenza della sintassi AT&T, la sintassi Intel compilata tramite NASM esprime sempre le istruzioni nella forma:

--Istruzione  --Destinazione,--Sorgente
Le parentesi quadre [...] indicano un accesso alla memoria puntata(dereferenziazione) dall'indirizzo contenuto all'interno:
ESEMPIO: mov rax, rbx ---> copia il valore del registro RBX nel registro RAX.
         mov rax, [rbx] ---> legge 8 byte dalla memoria situata all'indirizzo contenuto in RBX e li scrive in RAX.

In C se fai *ptr = 5, il compilatore sa di quanti byte è il puntatore (char* o int*), in genere 64 bit su architetture a 64 bit.
In Assembly, se fai mov [rax], 5, la CPU ha in mano solo un indirizzo di RAM: non sa se scriverci il 5 in formato 1 byte (0x05), 
2 byte (0x0005), o 4 byte (0x00000005). Ecco perché va specificato esplicitamente: mov byte [rax], 5 oppure mov dword [rax], 5 .
Se la dimensione dell'operazione è ambigua (es. quando si lavora con costanti immediate), occorre specificare l'operatore di 
dimensione,cioe' l'ampiezza della memoria manipolata:
-byte (1 byte / 8 bit)
-word (2 byte / 16 bit)
-dword (4 byte / 32 bit)
-qword (8 byte / 64 bit)



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




Il Chiamante (Caller) e' la funzione che sta eseguendo, per esempio il main. Il Chiamato (Callee) e' la funzione che viene invocata, ad esempio una chiamata ad ft_strlen nel main 
o in un altra funzione. In virtu' di questa distinzione, i registri sono divisi in due categorie operative: 
-Callee-Saved Registers (preservati dal chiamato / non-volatili): RBX, RBP, R12, R13, R14, R15.
 Se una funzione intende modificare uno di questi registri, ha l'obbligo di salvarne il valore originale sullo stack all'inizio della funzione (prologo) e ripristinarlo 
 prima di ritornare al chiamante (epilogo). Esempio: se il main ha un dato in RBX, e chiama ft_strlen, la convenzione ABI (System V) stabilisce che ft_strlen (il chiamato) non può 
 alterarlo in modo permanente. Se ft_strlen vuole usare RBX per i suoi scopi, deve pusharlo sullo stack, usarlo, e popparlo prima di fare ret.

-Caller-saved registers (preservati dal chiamante / volatili): RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11.
 Una funzione può sovrascrivere liberamente questi registri. Una qualsiasi chiamata di funzione esterna (es. call malloc o call write) potrebbe sovrascrivere indisturbata 
 tutti questi registri! Esempio: se il main ha un valore importante in RAX, e chiamo ft_strlen, quando ft_strlen ritorna, RAX sarà stato sovrascritto dal suo valore di ritorno. 
 Per salvare il vecchio valore era responsabilità del chiamante salvarlo nello stack prima del call.


Differenza tra funzioni C e syscall kernel
Mentre le chiamate a funzioni C impiegano l'istruzione call, le chiamate di sistema(System Calls) dirette al kernel Linux (come read e write) impiegano l'istruzione hardware speciale ISA syscall. 
L' utente (Ring 3) non puo' interagire con l'hardware del PC (scrivere su schermo, aprire un file, mappare memoria). Lo vietano i circuiti della CPU. Solo l'OS (Linux, Ring 0, modalità privilegiata) può farlo.
Per scavalcare questo limite, l'ISA possiede l'istruzione syscall. Questa istruzione genera un interrupt hardware (trap): blocca la tua esecuzione, concede poteri assoluti al kernel Linux, il quale esegue la 
richiesta in base al numero che viene caricato nel registro RAX (es. 0 = sys_read, 1 = sys_write, 9 = mmap... ce ne sono circa 350) ,cioe' l'identificatore numerico della syscall (registry ID syscall) .
I registri in cui sono salvati gli argomenti delle syscall sono leggermente differenti da quelli delle funzioni di libreria:
-1° Argomento: RDI
-2° Argomento: RSI
-3° Argomento: RDX
-4° Argomento: R10 (DIVERSO: le funzioni C usano RCX, ma l'istruzione syscall usa R10). Syscall necessita di usare RCX internamente per salvare temporaneamente l'indirizzo RIP aaaaaaaaaaaaaaaaaaaa cui tornare. Siccome `RCX` viene distrutto dall'istruzione, il kernel Linux ha deciso che il 4° argomento della syscall deve risiedere in `R10`, a differenza delle funzioni C che usano `RCX`.
-5° Argomento: R8
-6° Argomento: R9
Effetti collaterali hardware: l'istruzione syscall sovrascrive internamente i registri RCX e R11.


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
un indirizzo tra -1 e -4095? Perché lo spazio di indirizzamento della memoria virtuale di Linux riserva sempre gli ultimissimi kilobyte altissimi all'apice della memoria, vietando allocazioni 
lì in alto. Pertanto, qualsiasi valore in quel piccolissimo range finale (che castato a signed è -1 / -4095) è matematicamente inequivocabile: è un errore.
In C standard, le funzioni di libreria C per essere comode avvolgono (wrappano) l'istruzione syscall hardware (es. chiamando write() della unistd in C si invoca un wrapper che chiama l'istruzione 
hardware syscall numero 1). Questo wrapper prende il valore negativo restituito dal kernel, lo fa diventare positivo e lo scrive dentro una variabile globale chiamata errno, restituendo -1 al chiamante
cosi' che lo sviluppatore possa guardare in errno. Tuttavia, errno non può essere una singola variabile globale in memoria (.bss): se il processo ha due Thread logici che fanno syscall contemporanee ed 
entrambe falliscono, una sovrascriverebbe l'errore dell'altra, generando una race condition. Si chiama Thread-Local Storage (TLS): ogni thread in esecuzione possiede una sua copia isolata privata di errno.
Per accedere a errno in C si usa la libreria errno. In Assembly non si puo' fare mov [errno], rax perché non è una vera variabile globale, è una macro complessa legata al segmento del thread! 
Per questo si deve invocare la funzione della libc (il cui nome reale è formattato con i doppi underscore, __errno_location, usati storicamente nello standard C per nascondere simboli di sistema che non 
devono scontrarsi con i nomi usati dall'utente). Questa funzione calcola al volo l'indirizzo di memoria della errno del thread corrente e lo restituisce in RAX. Il chiamante ha il compito di scrivere
alla memoria puntata da quell' indirizzo l'errore convertito in numero positivo.




































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






### 2. Memoria: Fetch, Cache, MMU e Indirizzi

* **Caricamento e Demand Paging**: Quando il kernel esegue la syscall `execve` per lanciare il programma, **NON** carica tutto l'eseguibile in RAM. Crea solo la mappa della memoria (Virtual Memory). Il codice viene caricato in RAM "on-demand" (a blocchi di 4096 byte, o *Page*) solo quando la CPU tenta di eseguirlo.
* **La Cache L1i**: Esiste eccome. La CPU ha una Cache L1d (Dati) e una L1i (Istruzioni). La CPU non legge la RAM byte per byte; la RAM è lentissima. Il controller di memoria trasferisce blocchi da 64 byte alla volta (chiamati *Cache Line*) dalla RAM alla L1i. La CPU poi fa il *fetch* degli opcode direttamente dalla L1i.
* **Quanto è grande un opcode?** In x86-64, le istruzioni hanno lunghezza variabile. `ret` è 1 byte (`0xC3`). Altre istruzioni possono arrivare fino a 15 byte. La CPU decodifica e capisce da sola quanti byte deve consumare prima di incrementare `RIP`.
* **Fisico vs Logico e la MMU**: Gli indirizzi che usi tu (puntatori C) sono **Logici (o Virtuali)**. Sono un'illusione. Il tuo programma crede di avere la RAM tutta per sé. La **MMU (Memory Management Unit)** è un pezzo di hardware nella CPU che usa una tabella del Kernel (Page Table) per tradurre istantaneamente il tuo indirizzo logico `0x4000` in un indirizzo **Fisico** (il vero condensatore sul banco di RAM installato sulla scheda madre).

### 3. Execution Units: ALU, FPU, SIMD e AGU

* **FPU vs SIMD**: L'FPU (Floating Point Unit) calcola numeri con la virgola mobile classici (scalari, uno alla volta). **SIMD** (Single Instruction Multiple Data) usa registri vettoriali enormi (es. `XMM` a 128 bit, `YMM` a 256 bit, `ZMM` a 512 bit). Un "vettore" qui non è il `std::vector` del C++, ma un array hardware. In un registro da 256 bit puoi farci stare otto `float` da 32 bit e sommarli tutti a un altro registro in un singolo ciclo di clock.
* **AGU (Address Generation Unit)**: È il circuito che calcola matematicamente l'indirizzo effettivo prima di accedere alla memoria. Se tu scrivi `[rbx + rcx*4 + 8]`, l'AGU esegue la moltiplicazione e le somme via hardware per trovare l'indirizzo da passare alla MMU.


### 5. Lo Stack Frame e l'Allineamento

* **Push, Pop, Call, Ret**: Sono istruzioni mnemoniche Assembly e corrispondono a istruzioni opcode ISA dirette. Usano implicitamente il registro `RSP`.
* **Meccanica PUSH/POP**: Lo stack (di solito 8 MB limite) cresce verso il basso (da indirizzi alti come `0x7FFF...` verso indirizzi più bassi).
* `push rax`: La CPU decrementa `RSP` di 8, poi copia gli 8 byte di `RAX` all'indirizzo contenuto in `RSP`. Essendo architettura *Little Endian*, `RSP` punta al bit meno significativo (LSB) dell'intero dato salvato.
* `pop rbx`: Legge 8 byte dall'indirizzo a cui punta `RSP`, li mette in `RBX`, e incrementa `RSP` di 8 ("svuotando" lo spazio logico, anche se i dati restano fisicamente in RAM finché non sovrascritti).


* **Il ruolo di RBP (Base Pointer)**: All'inizio di una funzione si fa `push rbp`, poi `mov rbp, rsp`. `RSP` cambia in continuazione durante la funzione se fai dei `push`. `RBP` invece rimane inchiodato all'indirizzo iniziale. Questo permette al compilatore di trovare sempre la prima variabile locale a `[rbp - 8]`, la seconda a `[rbp - 16]`, indipendentemente da cosa fa `RSP`.
* **Call e Ret**: `call` fa `push rip`, e poi `jmp label`. `ret` fa `pop rip`. Il `rip` salvato dalla `call` è esattamente l'istruzione successiva alla `call` stessa nel chiamante.
* **Allineamento e Zeri Esadecimali**: Un indirizzo allineato a 16 byte significa che è un multiplo intero di 16. Siccome $16 = 2^4$, in binario gli ultimi 4 bit devono essere zero. In notazione esadecimale (base 16), ogni carattere rappresenta esattamente 4 bit. Quindi, qualsiasi indirizzo multiplo di 16 termina col carattere `0` (es. `0x7ffd9b8a0`).

### 6. Linking, PIE e Librerie Dinamiche (PLT/GOT)

* **La Direttiva Global / Extern**: Il linker fonde assieme C e Assembly. `global` scrive l'indirizzo in una tabella nel file `.o`. `extern` lascia un buco vuoto segnalando il nome della funzione da cercare. Il Linker incrocia i due dati.
* **PIE, PLT e GOT**: Un programma PIE viene caricato in RAM ad un indirizzo base casuale ad ogni esecuzione. Come fa il tuo programma a sapere dov'è la funzione `malloc` della libreria del sistema operativo (caricata altrove a caso)? Tramite la **PLT (Procedure Linkage Table)** (una sezione del tuo `.text`) e la **GOT (Global Offset Table)** (una sezione `.data`). La `call malloc` non chiama la vera `malloc`, chiama uno "stub" (un mini-blocco di 3 istruzioni assembly nella tua PLT) che legge un indirizzo contenuto nella GOT e ci salta. Al primo caricamento del programma, il *Dynamic Linker* del sistema operativo (es. `ld.so`) calcola dove si trova `malloc` e scrive l'indirizzo reale nella tua GOT. Dalla seconda volta in poi, è istantaneo.
 **PIE, PLT, GOT e `-no-pie**`: Per difendersi dagli hacker, i sistemi operativi caricano i programmi in memoria ad indirizzi casuali ogni volta che li apri (ASLR - Address Space Layout Randomization). Per fare questo, il codice deve essere compilato come **Position Independent Executable (PIE)**. Non potendo conoscere gli indirizzi assoluti in anticipo, il codice accede alle funzioni esterne come `malloc` tramite la **PLT (Procedure Linkage Table)** e la **GOT (Global Offset Table)**. Sono tabelle create dinamicamente: la tua `call malloc` in realtà salta a un piccolo stub (PLT) che legge l'indirizzo reale di `malloc` calcolato al momento del lancio del programma (runtime) dalla GOT. Il flag `-no-pie` vietava questo comportamento, forzando indirizzi statici vecchi di 20 anni.
### 7. Il Mistero degli Indirizzi Negativi e di Errno

* **Indirizzi Negativi**: Gli indirizzi di memoria su x86-64 sono numeri a 64 bit *unsigned* compresi tra $0$ e $2^{64}-1$. Tuttavia, i valori di ritorno del Kernel in `RAX` vengono interpretati dal wrapper C in complemento a due (cioè come numeri *signed*). Un errore del kernel è un numero tra `-1` e `-4095`. In binario a 64 bit, il numero `-2` è `0xFFFFFFFFFFFFFFFE`. Se un programma allocasse memoria a quell'indirizzo astronomico, il wrapper crederebbe che sia l'errore `-2`. Per evitare questo, il Kernel Linux per convenzione non mappa mai l'ultimo frammento della memoria virtuale (quello che corrisponde ai numeri da `-1` a `-4095` in signed). Quindi, se `RAX` contiene una roba tipo `0xFFFFFFFFFFFFFFFE`, il sistema sa al 100% che è un errore e non un puntatore `malloc`.
* **Abs di Errno**: Il wrapper prende l'errore kernel (es. `-2`), usa l'istruzione `neg rax` (che inverte il segno trasformandolo nel numero positivo `2`) e lo salva nella variabile globale `errno`.
* **Threading ed Errno**: Le variabili globali normali (`int x = 5;`) sono condivise tra l'intero processo (tutti i thread). Questo generava race condition clamorose con gli errori. Pertanto `errno` è definita nello standard POSIX come **Thread-Local Storage (TLS)**. Il sistema operativo assegna a ogni singolo thread un piccolo segmento di memoria privata.
* **Variabile vs Macro**: In C moderno, se scrivi `errno = 5;`, non stai scrivendo su una variabile chiamata `errno`. Il compilatore pre-processa `errno` come una macro: `(*__errno_location()) = 5;`. L'assembly non ha macro magiche implicite di questo tipo, devi fare tu a mano la `call __errno_location` e dereferenziare il puntatore che ti restituisce.

### 8. L'Illusione del Rapporto 1:1 Assembly / ISA

Non è perfettamente 1:1. L'Assembly è un'astrazione debole.
Esistono circa un migliaio di mnemonici Assembly, ma le codifiche ISA binarie sottostanti sono decine di migliaia.
Se scrivi `mov rax, rbx`, NASM la traduce in un opcode specifico. Se scrivi `mov rax, 5`, NASM usa una codifica binaria completamente diversa (Immediate encoding). Inoltre, NASM implementa delle "pseudo-istruzioni" a vantaggio del programmatore, che vengono espanse in più istruzioni macchina o convertite (ad esempio per caricare variabili sfruttando il `RIP`-relative addressing in modalità PIE).

Scendiamo nel dettaglio. Questi due concetti sono i pilastri che separano il testo che tu scrivi su uno schermo dai circuiti di silicio che eseguono effettivamente il calcolo.

### 1. Il Divario tra Variabili e Macro

Per capire la differenza, devi smettere di pensare al codice come a un'entità unica e iniziare a vederlo come una **catena di montaggio** con operai diversi (Preprocessore e Compilatore).

* **Cos'è una Variabile?**
Una variabile è un costrutto semantico del linguaggio C destinato al **Compilatore**. Quando scrivi `int x = 5;`, stai dicendo al compilatore: *"Riservami 4 byte di memoria fisica (nello stack o nella sezione `.data`), tieni traccia del suo indirizzo di memoria, ricordati che contiene dati formattati come interi con segno, e scrivici dentro il valore 5"*. La variabile esiste nello spazio e nel tempo durante l'esecuzione del programma.
* **Cos'è una Macro?**
Una macro è pura, stupida e brutale **sostituzione di testo** destinata al **Preprocessore**. Il preprocessore è un programma che viene eseguito *prima* del compilatore. Non sa nulla di memoria, di stack, di registri o di logica C. Fa letteralmente un "Trova e Sostituisci" sul testo del tuo file sorgente.

#### Il Trucco di `errno` in C

In C, quando includi `<errno.h>` e scrivi nel tuo codice:
`errno = 2;`

Tu pensi di scrivere in una variabile globale, ma il Preprocessore, leggendo il file `<errno.h>`, trova questa direttiva:
`#define errno (*__errno_location())`

Prima ancora che il Compilatore veda il tuo codice, il Preprocessore scansiona il testo, trova la parola `errno` e la cancella, sostituendola col lato destro della direttiva. Il codice che viene *effettivamente* consegnato al Compilatore è:
`(*__errno_location()) = 2;`

Il Compilatore non ha **mai** visto la parola "errno". Ha visto solo una chiamata di funzione che restituisce un puntatore, il quale viene dereferenziato (`*`) e a cui viene assegnato il valore `2`.

#### L'Assembly ha le Macro?

Sì, NASM ha un sistema di macro potentissimo (che usa una sintassi diversa, come `%macro` e `%endmacro`), che ti permette di creare alias di testo complessi per blocchi di istruzioni Assembly.
**Ma ecco il punto fondamentale:** NASM non legge i file header `.h` del C. Non ha idea di cosa sia la macro C `#define errno`. Quindi, se in Assembly vuoi modificare l'errore del thread, non puoi scrivere magicamente `mov [errno], 2`, perché per NASM la parola `errno` non significa assolutamente nulla. Devi bypassare l'illusione del C e scrivere manualmente ciò che la macro nasconde: chiamare la funzione `__errno_location` e scrivere all'indirizzo che ti restituisce.

---

### 2. Il Punto 8: L'Illusione dell'1:1 e la Molteplicità dell'ISA

Quando studi Assembly, tu studi dei **Mnemonici**.
Un *mnemonico* (dal greco, "che aiuta la memoria") è una parola in inglese leggibile dall'uomo che raggruppa un concetto operativo. Esempi di mnemonici sono `MOV`, `ADD`, `PUSH`, `XOR`. Nel linguaggio Assembly x86-64 esistono circa un migliaio di mnemonici in totale (includendo le estensioni SIMD/AVX).

Ma la CPU (l'hardware, il silicio) non sa cosa sia un "mnemonico". La CPU accetta solo byte binari, definiti dall'**ISA (Instruction Set Architecture)**. E qui avviene la magia e la complessità.

L'ISA x86-64 non ha una singola istruzione hardware per il concetto di "muovi i dati" (`MOV`). Ha decine di istruzioni hardware completamente diverse a seconda di *cosa* stai muovendo e *dove* lo stai muovendo.

Guarda cosa succede dietro le quinte quando tu scrivi sempre e solo il mnemonico `mov`:

1. Scrivi: `mov rax, rbx` (Copia da un registro 64-bit a un registro 64-bit)
* L'Assemblatore NASM analizza gli operandi e genera l'opcode binario esatto: **`48 89 D8`**.


2. Scrivi: `mov eax, ebx` (Copia da registro 32-bit a registro 32-bit)
* NASM traduce in un opcode hardware completamente diverso: **`89 D8`**.


3. Scrivi: `mov rax, 5` (Copia un numero immediato/costante in un registro)
* NASM traduce nell'opcode hardware: **`48 C7 C0 05 00 00 00`**.


4. Scrivi: `mov [rax], rbx` (Copia un registro nella RAM puntata da un altro registro)
* NASM traduce nell'opcode hardware: **`48 89 18`**.



#### Cosa significa questo per te?

Tu, programmatore Assembly, hai l'illusione di conoscere un linguaggio semplice e diretto (usi sempre la parola `mov`). Ma in realtà ti stai interfacciando con un mostro hardware di una complessità aberrante (il manuale ISA della Intel è lungo quasi 5000 pagine).
Esistono decine di migliaia di combinazioni binarie valide nell'ISA. L'assemblatore (NASM) fa da ponte: tu gli dai un'astrazione umana (il mnemonico e gli operandi), e lui cerca nel suo dizionario interno qual è l'esatto e univoco codice a barre binario (opcode) che i circuiti stampati della CPU si aspettano per eseguire quel microscopico, specifico movimento di elettroni.

A livello di **Assemblatore (NASM)**, i mnemonici delle istruzioni e i nomi dei registri sono rigorosamente **case-insensitive** (insensibili alle maiuscole/minuscole).

Da un punto di vista strettamente tecnico, scrivere `MOV RAX, 5`, `mov rax, 5`, o perfino una mostruosità come `MoV rAx, 5` produce esattamente lo stesso risultato. L'assemblatore li interpreta tutti come la stessa astrazione e genera l'identico opcode binario hardware (`48 C7 C0 05 00 00 00`).

Tuttavia, bisogna fare una netta distinzione tra *possibilità tecnica*, *retaggio storico* e *convenzione moderna*.

### 1. Il Retaggio Storico e i Manuali Ufficiali (Maiuscolo)

Se apri il mastodontico *Intel 64 and IA-32 Architectures Software Developer's Manual* (la "bibbia" hardware della CPU), troverai ogni singolo mnemonico stampato in **MAIUSCOLO** (es. `MOV`, `PUSH`, `SYSCALL`).
Questa è un'eredità diretta dell'informatica degli anni '70 e '80. I primi terminali (Teletype), i vecchi mainframe e i primi assemblatori commerciali (come il MASM della Microsoft per MS-DOS) usavano il maiuscolo come standard, spesso perché i set di caratteri originari non supportavano le minuscole o lo facevano male.

### 2. La Convenzione Moderna e il Mondo UNIX/C (Minuscolo)

Oggi, specialmente su sistemi Linux, macOS e nei progetti accademici (incluso il tuo progetto della 42), **lo standard assoluto è scrivere mnemonici e registri in minuscolo** (`mov rax, rbx`).
Il motivo è l'influenza egemonica del linguaggio C e di UNIX. Il C è un linguaggio strettamente *case-sensitive* (sensibile alle maiuscole/minuscole) in cui le parole chiave sono tutte minuscole. Quando gli sviluppatori UNIX hanno iniziato a scrivere codice mista C/Assembly (usando toolchain come GNU GCC e l'assemblatore GAS), la convenzione del minuscolo è "strabordata" nel mondo Assembly per pura coerenza visiva ed estetica del codice sorgente.

### 3. L'Eccezione Critica: Etichette e Simboli (Case-Sensitive)

C'è una trappola mortale in NASM che devi avere ben chiara: se è vero che NASM ignora il case per le *istruzioni hardware* (mnemonici) e per l'hardware interno (registri), **le etichette (label), i nomi delle variabili e le chiamate di funzione sono strettamente CASE-SENSITIVE**.

* Se devi chiamare la funzione di allocazione del C, **devi** scrivere `call malloc`. Se scrivi `call MALLOC`, NASM lo compilerà senza fiatare, ma nella fase successiva il Linker (`ld`) esploderà con un errore di tipo `Undefined reference to 'MALLOC'`, perché per il Linker (che segue le regole del C) `malloc` e `MALLOC` sono due stringhe testuali distinte che puntano a indirizzi di memoria potenzialmente diversi.
* Lo stesso vale per le tue direttive di esportazione. Se scrivi `global ft_strlen`, il C potrà chiamare `ft_strlen()`. Se scrivi `global FT_STRLEN`, il C non troverà mai la tua funzione.

In sintesi: per non impazzire e per rispettare la conformità del codice, scrivi **tutto il file sorgente `.s` in minuscolo**.
*/