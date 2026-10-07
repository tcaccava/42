/*
---------EQUAZIONI E FUNZIONI-----------------------------------------------------------------------------------
Un' equazione descrive una condizione,cioe' una regola, di uguaglianza tra due espressioni algebriche, per la quale possono esistere specifiche soluzioni(gli zeri) che realizzano quella eguaglianza.E' quindi una condizione statica che non fa letteralmente nulla.
Una funzione è un operatore meccanico,una procedura algoritmica reale, un pezzo di codice o una trasformazione matematica che prende un input(dominio) e ,obbedendo a quella regola, restituisce un output(codominio) che corrisponde alle soluzioni di quella equazione.
Una funzione parziale calcolabile e' una funzione matematica f che può essere fisicamente calcolata da una Macchina di Turing in un tempo finito. Si dice "parziale" perché non è garantito che sia definita per ogni input possibile nel suo dominio.
Se per un dato input la Macchina di Turing entra in un loop infinito,quindi non termina,o crasha(Undefined Behavior,Segfault,divisione per zero), la funzione per quell'input è semplicemente non definita. 
Se invece la macchina termina sempre e restituisce sempre un risultato per ogni input del suo dominio, la funzione diventa totale. 

---------TEOREMA DELLA FERMATA------------------------------------------------------------------------------------
Il teorema della fermata e' un limite insuperabile della logica dimostrato nel 1936: afferma che è matematicamente impossibile scrivere un programma che prenda in input il codice sorgente di un altro programma e decida, nel 100% dei casi,
se quel codice eseguito terminera' o meno,quindi se quel codice sia assimilabile ad una funzione parziale o totale calcolabile. Posso scriverlo per casi specifici, ma l'analizzatore statico universale che funziona per ogni programma possibile è matematicamente 
impossibile da creare. Non e' possibile per esempio scrivere un parser in C che garantisca a priori l'assenza di loop. Se fosse possibile, si creerebbero paradossi logici distruttivi (un programma che entra in loop solo se l'analizzatore dice che terminerà). 
Per questo il compilatore non può salvare alcun programmatore da un while(1) o da una ricorsione difettosa. Un linguaggio come C,ma vale per qualunque altro linguaggio Turing completo, mappa la classe delle funzioni parziali perche' i suoi costrutti consentono 
di fare cose come un ciclo while true infinito senza uscita o andare in segfault/ UB,ma questo non significa che non ci si possa scrivere anche una funzione completa.
Ogni funzione totale in un linguaggio Turing completo è matematicamente solo un caso specifico (un sottoinsieme) di una funzione parziale che per puro caso "è definita ovunque".

---------MACCHINA E LINGUAGGIO DI TURING-----------------------------------------------------------------------------------------
Una macchina di Turing e' sostanzialmente il modello matematico assoluto di un computer,cioe' la definizione matematica di una macchina capace di computare. E' composta da almeno tre componenti fisici (teorici):
1) un nastro di memoria infinito diviso in celle (l'equivalente della ram di un odierno pc).
2) una testina di lettura/scrittura che può scorrere a destra o sinistra sul nastro di memoria.
3) un registro di stato e una tabella di transizioni (equivalenti,rispettivamente ad una moderna cpu e all'Instruction Set della stessa).
Ad ogni step, la macchina legge il simbolo sul nastro, guarda il suo stato attuale, consulta la tabella, e decide cosa scrivere, dove spostarsi e in che stato passare. È il sistema più semplice possibile in grado di calcolare tutto ciò che è calcolabile 
in un tempo finito. Se un problema non può essere risolto da una TM, non può essere risolto da nessun computer esistente (Tesi di Church-Turing).

Un linguaggio si dice Turing completo se possiede i costrutti per simulare una macchina di Turing. In informatica teorica, diciamo che i linguaggi Turing-completi mappano la classe delle funzioni parziali. I linguaggi mainstream (C, C++, Python) mappano tutti 
solo il dominio delle funzioni parziali,e dunque sono tutti linguaggi Turing completi. Esistono linguaggi di nicchia (come Coq, Agda o Idris) ,chiamati linguaggi a programmazione funzionale totale, in cui il compilatore rifiuta letteralmente di produrre il 
binario se non riesce a dimostrare matematicamente (tramite il sistema di tipi) che la funzione terminerà per tutti gli input. Lì non e' possibile causare segfault o loop infiniti, ma programmarci è un inferno concettuale.In parole povere, se un linguaggio permette 
di fare salti condizionali (if/branching) e di manipolare la memoria iterativamente (while/puntatori), può computare qualsiasi cosa sia fisicamente computabile.
Qualsiasi programma in un linguaggio Turing completo compie nient'altro che il calcolo di una funzione parziale calcolabile.

---------PUNTO FISSO DI UNA FUNZIONE E QUINE------------------------------------------------------------------------------
Nel lambda calcolo e nella teoria della computazione, un punto fisso di una funzione è un valore che viene mappato su se stesso dalla funzione ,ovvero (f(x) = x). In sostanza un punto fisso x di una funzione f è semplicemente un valore tale per cui l'esecuzione 
della funzione su quel valore restituisce il valore stesso. Esempio : se f(x) = x^2, i punti fissi sono 0 (0^2 = 0) e 1 (1^2 = 1). Se F è l'operazione di invertire una stringa, le stringhe palindrome (es. "radar") sono i punti fissi. 
Un Quine è letteralmente il punto fisso di un ambiente di compilazione ed esecuzione,cioe' un costrutto informatico il cui output coincide esattamente con il proprio codice sorgente . Esempio : una funzione E(s) che prende una stringa s (il file .c), la compila, 
la esegue e indirizza l'output sullo standard output. Normalmente, se metto dentro s un "print(2+2)",la funzione eseguita mi restituisce 4 : E(s) = "4".Se s è un Quine, avviene questo: E(s) = s. La stringa sorgente in ingresso sopravvive intonsa al processo di compilazione
ed esecuzione, ritornando se stessa.
Se prendiamo un compilatore o un interprete, possiamo vederlo come una funzione E (Execution environment) che prende in input un codice sorgente S e produce in output un risultato R: E(S) = R.
Un Quine non è altro che un codice sorgente Q il cui output è esattamente sé stesso. Quindi E(Q) = Q.
Un Quine è letteralmente il punto fisso dell' interprete o compilatore. E il fatto che i quine esistano per qualunque linguaggio di programmazione Turing-completo è garantito al 100% proprio dal Secondo Teorema di Ricorsione.
il primo quine documentato è stato scritto nel 1953 (su schede perforate!) da Paul Bratley e Jean Millot su un computer EDSAC, ben prima che venisse coniato il termine "quine" (in onore del filosofo Willard Van Orman Quine, famoso per i suoi studi sull'autoreferenza logica).

---------TEOREMI DI RICORSIONE DI KLEEN E NUMERAZIONE DI GODEL----------------------------------------------------------------------------
I due teoremi di ricorsione di Kleen poggiano su un assioma fondamentale: la numerazione di Godel. Qualsiasi macchina di Turing puo' essere codificata nella forma di un intero o di una singola stringa univoca. Questo annulla la distinzione del codice sorgente di un 
programma in codice e dati: un programma puo' manipolare altri programmi trattandoli come numeri. La numerazione di Godel non e' un hash: le funzioni di hash hanno dimensione fissa,generano collisioni e non sono revertibili;non posso ricostruire il file originale 
dall'hash. La numerazione di Gödel è una codifica biunivoca (isomorfismo) senza alcuna perdita di informazione. Per esempio un sorgente .c e' un file di testo, quindi una sequenza di byte ASCII. Se prendo i byte in hex e li concateno, otterro' un singolo,
gigantesco numero intero. Da quel numero posso riottenere l'esatto codice sorgente applicando l'operazione inversa (decodifica). Questo concetto dimostra una cosa fondamentale in informatica teorica: codice e dati sono la stessa identica cosa. Un programma è solo un grosso intero 
che la macchina di Turing interpreta come istruzioni. Quindi basicamente la numerazione di Godel di un programma e' semplicemente la rappresentazione binaria del suo sorgente,convertita in un intero gigantesco a precisione arbitraria, la cui bitness dipende dalla dimensione del sorgente ,e che 
semplicemente contiene tutta l'informazione codificata in forma binaria.

--------PRIMO TEOREMA DI RICORSIONE----------------------------------------------------------
Il primo Teorema (Teorema del Minimo Punto Fisso) riguarda la semantica del codice,quindi la logica matematica a fondamento della informatica teorica che legittima la programmazione ricorsiva: stabilisce che ogni equazione che definisce una funzione in termini di se stessa(ricorsiva) possiede solo un minimo punto fisso,e quest'ultimo e' calcolabile. 
È ciò che garantisce che quando scrivo una funzione ricorsiva in C, matematicamente abbia un senso e calcoli qualcosa di specifico. 
In realta',per essere piu' precisi,il primo teorema,quando parla di punto fisso non si riferisce ad input o output numerici(quelli sono solo una semplificazione per rendere comprensibili teoremi cosi' complessi),ma alle funzioni stesse. Possiamo quindi immaginare un punto fisso 
come una funzione f che ,passata in input ad un altra funzione F,restituisce se stessa come output. Il parellelo concettuale piu' immediato e' una funzione C che accetta un puntatore a funzione come argomento e ne restituisce un altro,o ancora meglio un decoratore python,cioe' un wrapper che 
prende una funzione e la ritorna modificata, o ,a un livello più macroscopico, un compilatore,cioe' un programma che prende in input un file di testo (il codice sorgente di un altro programma) e sputa fuori un binario. 
In matematica (nello specifico nel Lambda Calcolo, che è la base teorica della computazione), i tipi primitivi non esistono: tutto è una funzione di ordine superiore. 
Perché questo ha a che fare con la ricorsione? Perché quando scrivo una funzione ricorsiva in C (es. f(x) = x * f(x-1)), sto in realta' scrivendo un'equazione in cui l'incognita da trovare è la funzione f. Sto cioe' definendo f usando f stessa. Il compilatore si trova davanti a un costrutto circolare. 
Il Primo Teorema mi salva garantendo che per un simile costrutto circolare esiste sempre almeno una vera funzione matematica che si comporta esattamente così. Quella funzione è il minimo punto fisso.
In logica formale e teoria degli insiemi,non posso scrivere una definizione circolare,cioe' definire un'entità menzionando il suo stesso nome prima che quell'entità sia stata completamente definita. È un errore sintattico e logico insormontabile. Per aggirare questo blocco e dimostrare che la ricorsione 
e' corretta sotto il profilo logico-formale, i matematici devono fare un giro di boa: creano un operatore esterno F (il generatore),dicono che F prende una funzione "grezza" o incompleta g e la espande di un gradino,quindi definiscono la vera funzione ricorsiva f come quel particolare oggetto che rappresenta 
il punto d'arresto dell'operatore, cioè F(f) = f.L'operatore F è il trucco formale necessario per strappare la ricorsione al paradosso logico della circolarità.
Quando scrivo una funzione ricorsiva in C, per es. int f(int n) { return n == 0 ? 1 : n * f(n-1); }, matematicamente sto scrivendo un'equazione in cui f compare sia a destra che a sinistra: f = F(f),ovvero quella funzione f deve soddisfare una regola(un equazione) in cui compare se stessa. Sto cercando quella funzione f tale che, se la do in pasto all'
operatore di trasformazione F, ti restituisce esattamente se stessa. Questa è la definizione algebrica di punto fisso. Il problema matematico è che un'equazione ricorsiva del genere ha quasi sempre infiniti punti fissi. Esistono cioe' infinite funzioni diverse che, se infilate dentro F, fanno quadrare l'equazione. 
Tra queste ci sono funzioni corrette, ma anche funzioni non corrette.Non corrette rispetto a quale rule set?Il "rule set" è la specifica formale del problema che voglio computare (ad esempio, le proprietà matematiche che definiscono univocamente il fattoriale o la serie di Fibonacci).Se scrivo un'equazione ricorsiva scritta male,
l'algebra formale potrebbe trovare dei punti fissi che soddisfano formalmente l'equazione F(f) = f ma che, rispetto al problema reale che volevo risolvere, sono spazzatura (es. restituiscono valori costanti, o non terminano dove dovrebbero). Tra tutti i punti fissi possibili che fanno quadrare l'equazione algebrica, solo il minimo punto fisso corrisponde esattamente alla semantica pulita 
e corretta del mio algoritmo, senza effetti collaterali o invenzioni arbitrarie.
Il Primo Teorema di Kleene dice che ogni equazione di questo tipo ha un solo minimo punto fisso, ovvero esiste una e una sola funzione matematica che è la "più piccola" (quella definita su meno input possibili) a soddisfare l'equazione. Per capire il concetto di "minimo", bisogna comprendere 
come i matematici classificano le funzioni. In questo contesto, una funzione è considerata "più piccola" o parziale di un'altra se è meno definita, ovvero se per un maggior numero di input non restituisce un bel niente . La funzione in assoluto più piccola di tutte è quella che non sa fare nulla: diverge per qualunque input.
Il Primo Teorema di Kleene in sostanza stabilisce quale, tra infinite funzioni possibili, è quella "vera" che descrive un calcolo effettivo,affermando che esiste sempre un "minimo punto fisso" per qualsiasi operatore ricorsivo monotonicamente crescente e che esso è l'unica funzione che fa solo ed esclusivamente ciò che le è richiesto dai casi base,
senza aggiungere comportamenti arbitrari o inventati.
Come si costruisce questo minimo? Il teorema non si limita a dire che esiste, ma spiega come ottenerlo:
-Passo 0 : si parte da una funzione teorica  che fallisce (diverge o va in loop) per qualsiasi input le venga passato. Questa è la funzione più piccola possibile nello spazio matematico(funzione vuota).
-Passo 1: la funzione vuota viene passata all'operatore F,che calcola il primo livello di logica e restituisce una funzione rudimentale che sa gestire solo il caso base (es. calcola correttamente solo quando l'input è 0).
-Passo 2: la funzione parziale viene passata ad F,che ne espande la logica, ottenendo una funzione che gestisce il caso base e il primo step di ricorsione (es. input 0 e 1).
-Il Limite: ripetendo questo processo all'infinito (o per un numero transfinito di volte), la sequenza di funzioni converge a un limite convergente che è il minimo punto fisso. È la funzione matematica completa e perfetta (ad esempio, la funzione fattoriale esatta) che soddisfa l'equazione ricorsiva senza inventarsi alcun comportamento spurio per gli input non previsti.

Quando scrivo la logica di una ricorsione, sto in realta' scrivendo un'equazione.
Esempio: se scrivo int f(int x) { return f(x); }, qual è la funzione matematica che esce fuori? È la funzione "vuota", cioè una funzione parziale che diverge (va in loop) su qualunque input. Quella funzione vuota è il minimo punto fisso di quell'equazione ricorsiva.

Esempio pratico del Primo Teorema:
Mettiamo il caso che scriva un fattoriale in C dimenticandomi il caso base:
int f(int x) { return x * f(x-1); }
Qual è la funzione matematica che soddisfa questa regola? È la funzione che per qualsiasi input crasha o va in loop infinito. Quella funzione (la "funzione ovunque indefinita",cioe' la funzione vuota) è il minimo punto fisso di questa ricorsione rotta.
Mettiamo invece che la abbia scritta correttamente:
int f(int x) { if (x == 0) return 1; else return x * f(x-1); }
Qual è la funzione che soddisfa questa regola? È la funzione matematica del fattoriale. Il fattoriale vero e proprio è il minimo punto fisso di questa definizione.
Perche' e' nata tutta questa costruzione teorica di fronte ad un problema che appare insignificante?
Quando in C scrivo una ricorsione, a livello x86-64 sto solo piazzando un'istruzione call che punta a un indirizzo di memoria (o a un'etichetta nel segmento .text) che si trova due righe più sopra. La CPU non vede il paradosso logico: vede un indirizzo, pusha il Base Pointer e 
l'Instruction Pointer sullo stack, allinea i registri e salta. Se salta all'infinito, sfonda lo stack e si va in segfault. La macchina fisica non si occupa della semantica.
Ma la matematica pura non ha lo stack, i registri o l'Assembly. In logica matematica pura, non posso definire un'entità usando sé stessa. È un paradosso logico, come lo sarebbe definire la parola "cane" scrivendo sul vocabolario "entità che si comporta da cane".
Per dare un senso logico (e matematico) a quella call verso se stessa, i teorici si sono inventati questo escamotage per dimostrare che la ricorsione ha una sua integrita' logico-formale:
1) Immaginiamo un "costruttore" di codice, una funzione di ordine superiore: F_Builder.
2) F_Builder prende in input una funzione fittizia qualsiasi e la "potenzia", iniettandoci dentro la logica operativa di un algoritmo (ad esempio, le regole di un fattoriale).
3) All'inizio, gli passo un input spazzatura: una funzione "vuota" che genera solo undefined behavior. F_Builder elabora questa spazzatura e restituisce una funzione che sa calcolare solo il caso base (es. x = 0).
4) Se prendo questa nuova funzione e la ripasso a F_Builder, ne restituisce una leggermente migliorata, che sa calcolare il caso base e lo step successivo (x = 1).
5) Iterando questo ciclo, la funzione si "costruisce" un passo alla volta. Arriva un punto in cui la funzione che esce è identica, byte per byte, logica per logica, a quella che è entrata.
Questo limite di saturazione, dove l'input (la funzione vecchia) equivale all'output (la funzione nuova) senza subire alterazioni, è il punto fisso. Ed è matematicamente l'esatta funzione ricorsiva completa, deterministica e funzionante.
Il Primo Teorema è una colossale pezza teorica: serve "solo" a garantire ai matematici e ai creatori di compilatori che permettere a una funzione di chiamare se stessa non innesca una fallacia circolare che invalida la logica matematica e quindi la computabilita', ma produce sempre un 
comportamento deterministico (che sia un calcolo corretto o un crash inevitabile).

--------SECONDO TEOREMA DI RICORSIONE------------------------------------------------------------------------------------------
Per comprendere formalmente il teorema bisogna separare rigidamente due livelli:
1. Il piano sintattico (x): il codice sorgente statico, ovvero il numero di Godel x che rappresenta i byte del programma fermi sul disco.
2. Il piano semantico (phi_x): il processo dinamico, ovvero la funzione calcolata quando l'interprete o la macchina (phi) esegue effettivamente quel codice (x).

Il Secondo Teorema afferma che, per qualsiasi funzione totale calcolabile f che agisce come trasformatore sintattico di codice (una macro, uno script o una logica che prende un sorgente e lo modifica), 
esiste sempre un particolare programma e tale per cui le funzioni calcolate sono equivalenti: phi_e == phi_f(e).
Questo non significa che i file sorgente siano identici, ma che l'esecuzione del programma originale e produce lo stesso identico comportamento dinamico dell'esecuzione del programma ottenuto dopo averlo fatto stravolgere da f.
Applicato all'architettura dei Quine: se f e' una funzione che trasforma un sorgente iniettandogli la logica per stampare il suo sorgente, il teorema garantisce che esiste un programma e il cui comportamento operativo (phi_e) coincide 
perfettamente con quella modifica, rendendolo capace di stampare in output il proprio codice sorgente senza alcuna lettura esterna o input illegale.

In un contesto architetturale applicato alla informatica, questo significa che un programma puo' contenere la specifica completa di se stesso senza il bisogno di accedere a file esterni del disco rigido o ad astrazioni esterne (come l'uso di argv/argc o la lettura del file sorgente,
operazioni rigorosamente classificate come "cheating" in questo progetto). 
L'autoriferimento è risolvibile matematicamente all'interno del codice stesso, creando un sistema chiuso e autosufficiente. Il secondo teorema dice una cosa brutale e controintuitiva: ogni programma può avere accesso al proprio codice sorgente durante l'esecuzione.
Immaginiamo che io voglia scrivere un programma in C senza input esterni che stampi in output il suo stesso sorgente.
Inizio così:
1) int main() { printf( ??? ); }
Cosa metto al posto dei punti interrogativi? Ci devo mettere il codice stesso. Quindi provo:
int main() { printf("int main() { printf( ??? ); }"); }
Ed ecco il problema: appena inserisco il codice nella printf, il file sorgente originario è cambiato, è diventato più grande,quindi la stringa dentro la printf è ora incompleta.Se provo ad aggiornarla mettendoci la nuova versione:
int main() { printf("int main() { printf(\"int main() { ... }\"); }"); }
...il file cresce ancora. È un regresso all'infinito. Più cerco di descrivere o "hardcodare" il programma dentro se stesso, più il programma si espande, e non riusciro' mai a scrivere l'ultima virgoletta.Il che non significa che il compito sia impossibile,ma solo che un
approccio additivo ,basato sulla concezione del sorgente come un blocco monolitico di codice e dati, e' totalmente fallimentare by design.
Il Secondo Teorema di ricorsione entra in gioco qui: dimostra matematicamente che esiste sempre una via di fuga a questo loop infinito,garantendo che è possibile separare la logica esecutiva del programma dai suoi dati (componente attiva e passiva) per aggirare il problema dell'espansione infinita,
che è il principio esatto su cui si basa l'architettura di un Quine. Esiste sempre un modo per un programma di calcolare il proprio numero di Gödel (il proprio codice sorgente) e passarlo come argomento a sé stesso.

----------STRUTTURA DI UN QUINE------------------------------------------------------------------------------------------------
Per costruire un Quine valido, la teoria di base (spesso associata al costruttore universale di von Neumann) richiede la scomposizione del sorgente in due entità ontologicamente distinte ma interdipendenti:
1) la componente attiva (codice o fenotipo): le istruzioni operative incaricate di formattare e stampare.
2) la componente passiva (dati o genotipo): una rappresentazione in memoria (generalmente una stringa o un buffer) che mappa esattamente le istruzioni della componente attiva.
Il parallelismo con un linguaggio a basso livello come Assembly ,ma anche con C, e' immediato: 
1) la componente attiva e' il .text segment,cioe' le istruzioni macchina vere e proprie.
2) la componente passiva e' l'insieme di .data/.bss/.ROData segments: cioe' variabili globali inizializzate e non,e variabili o buffer costanti.
La componente passiva "mappa" quella attiva non nel senso che ne copia la memoria a runtime. Significa piuttosto che la componente passiva contiene un isomorfismo strutturale pre-calcolato. È una rappresentazione codificata (sotto forma di stringa) della sintassi esatta 
che compone la parte attiva e il resto del programma. Non c'è nessuna "lettura" della memoria delle istruzioni, ma piu'banalmente una stringa hardcoded che contiene l'esatta traduzione ASCII del codice che la sta utilizzando, e di se stessa. È un trucco di auto-riferimento sintattico 
che consiste nel fatto che la stringa non contiene se stessa per intero in modo letterale, ma contiene il formato per stampare se stessa. Questo non e' un cheat: sto sfruttando l'isomorfismo strutturale tra dati e codice, la stessa identica base su cui operano i metaprogrammi e l'architettura 
dei malware di tipo dropper/replicanti.
Il parallelismo biologico è assoluto: la componente dati agisce esattamente come il DNA (che codifica l'informazione per la sintesi delle proteine in modo inerte), mentre la componente attiva agisce come i ribosomi e le polimerasi che trascrivono e traducono quell'informazione 
per ricostruire sia la cellula (il programma) che il filamento di DNA stesso (la stringa autogenerata). 
L'esecuzione consiste nel far operare la componente attiva sui dati per produrre una copia di entrambi.
Qui entra in gioco un ulteriore complicazione tipica di linguaggi come C : per stampare una stringa, devo racchiuderla tra virgolette doppie. Quindi la stringa passiva deve contenere il codice del programma. Ma il programma contiene la stringa passiva, che ha le virgolette.
Se provo a scriverlo, cadro' in questo ciclo di regressione infinita:
-devo stampare una stringa: "..."
-la stringa deve contenere la sua stessa definizione: " "..." "
-ma in C devo mettere i caratteri di escape per le virgolette interne perche' vengano interpretate come tali: " \"...\" "
-ma ora la stringa deve contenere i caratteri di escape \, quindi devo fare l'escape delle escape: " \\\"...\\\" "
-questo genera una ricorsione logica infinita in fase di scrittura del codice: non posso definire la stringa passiva includendo letteralmente le virgolette o i ritorni a capo (newline) necessari per formattarla, perché per farlo sarei costretto ad aggiungere altri caratteri di escape,
 alterando la stringa che stavo cercando di rappresentare.

Il legame matematico tra il punto fisso e il codice sorgente si riduce a questa identità: Compiler(Source) = Binary,e poi Execution(Binary) = Output. Nel momento in cui scrivo un Quine, cerco un codice sorgente S tale che Execution(Compiler(S)) = S. L'output dell'esecuzione deve essere bit a bit identico al 
sorgente S che è entrato nel compilatore. Questo è il punto fisso: lo spazio dei dati in cui l'operazione di stampa/compilazione non sposta né altera di un byte la struttura sintattica dell'oggetto di partenza.
Il punto centrale del secondo teorema di ricorsione di Kleene applicato al problema del Quine è la dimostrazione che un sistema formale puo' possedere la specifica strutturale di se stesso attraverso una relazione di ricorsione incrociata tra l'operatore e il suo operando.
La struttura teorica si regge su tre pilastri concettuali puri:
1) Isomorfismo di dominio: poiché codice e dati condividono lo stesso spazio numerico (la numerazione di Gödel), non esiste alcuna barriera ontologica tra un'istruzione che elabora e un dato che descrive. Una funzione può avere come proprio dominio la stringa di caratteri che la definisce.
2) La dualità funzionale (il doppio strato): per evitare il regresso all'infinito in cui un oggetto che descrive se stesso dovrebbe espandersi all'infinito, la teoria impone la separazione in due entità logiche simmetriche: un operatore di transizione (la logica attiva ) e un template strutturale (la componente passiva).
3) La risoluzione del punto fisso (F(x) = x): il teorema dimostra che esiste sempre una configurazione in cui l'operatore di transizione agisce sul template strutturale producendo in uscita esattamente lo stesso template e la stessa regola che lo ha generato. In termini logici, la struttura collassa
   in un'identità chiusa dove l'input descrittivo e l'output generato coincidono perfettamente.Questo è il motivo per cui la teoria esclude qualsiasi input esterno o lettura da file: l'autoriferimento non è un'operazione di I/O, ma una proprietà topologica dello spazio di computazione, garantita dal fatto che la 
   logica e la stringa descrittiva sono legate da un punto fisso matematico.

======================================================================================================================
   NOZIONI DI CODICE FONDAMENTALI PER AFFRONTARE IL PROGETTO
===================================================================================================================

--------SEQUENZE DI ESCAPE----------------------------------------------------
   In C, le sequenze di escape numeriche si esprimono in due modi:
   - Notazione Ottale (\ooo): usa cifre da 0 a 7 (fino a un massimo di 3 cifre). Il carattere delle virgolette doppie (") ha codice ASCII 34 in decimale. Convertito in base ottale, 34 diventa 42. Di conseguenza, la sequenza ottale corretta per le virgolette è \42 (o \042).
   - Notazione Esadecimale (\xhh): usa il prefisso \x seguito da cifre esadecimali. Il valore esadecimale del codice ASCII 34 è 22. La sequenza esadecimale corretta per le virgolette è quindi \x22.
   In Assembly non esiste un insieme universale di caratteri di escape a livello di ISA/linguaggio, poiché l'Assembly tratta la memoria come byte puri. Tuttavia, il modo in cui vengono interpretati i caratteri di escape dipende interamente dall' assembler utilizzato (nel mio caso NASM).
   In NASM esistono due modalità principali per gestire i caratteri speciali e i ritorni a capo nelle stringhe:
   1) La sintassi con i Backtick (``...``) — C-Style Escapes
      Se racchiudo una stringa tra backtick (l'accento grave `), NASM abilita l'interpretazione dei caratteri di escape in stile C.
      Principali caratteri di escape supportati nei backticks:
      | Sequenza | Significato | Valore ASCII (Dec / Hex) |
      | --- | --- | --- |
      | `\n` | Newline (Line Feed) | `10` (`0x0A`) |
      | `\r` | Carriage Return | `13` (`0x0D`) |
      | `\t` | Tabulazione orizzontale | `9` (`0x09`) |
      | `\0` | Byte nullo (Null terminator) | `0` (`0x00`) |
      | `\\` | Backslash | `92` (`0x5C`) |
      | `\'` | Apice singolo | `39` (`0x27`) |
      | `\"` | Virgoletta doppia | `34` (`0x22`) |
      | `\`` | Backtick | `96` (`0x60`) |
      | `\e` / `\E` | Carattere Escape (ANSI escape codes) | `27` (`0x1B`) |
      | `\xHH` | Byte esadecimale arbitrario (es. `\x0A`) | Specificato da `HH` |
      | `\000` | Byte ottale arbitrario (es. `\012`) | Specificato dal valore ottale |

   2) Le virgolette classiche ("..." o '...') — Nessun Escape
      Se uso le virgolette doppi o i singoli apici NASM NON interpreta alcun carattere di escape. Se scrivi '\n', l'assembler scriverà in memoria letteralmente un backslash (0x5C) seguito dal carattere n (0x6E).
      Per inserire caratteri speciali con la sintassi classica, si separano le stringhe con i valori numerici ASCII trascritti in decimale o esadecimale tramite virgola nella direttiva db.
      Esempio:

      s_classica: db "Linea 1", 10, "Linea 2 con ", 34, "virgolette", 34, 0


   I due approcci seguenti generano identici byte nella sezione dati del binario compiled:

   section .data
      ; Metodo 1: Backtick (C-style)
      str1: db `Hello\nWorld\0`

      ; Metodo 2: Separazione per virgola (Classic NASM)
      str2: db "Hello", 10, "World", 0


---------ARGOMENTI POSIZIONALI IN FUNZIONI DELLA FAMIGLIA PRINTF---------------------------------
   %1 e' ad esempio una specifica di argomento posizionale supportata dall'estensione POSIX della printf (presente nella libreria standard di sistemi come Linux/glibc). In una printf standard, gli argomenti vengono consumati in ordine sequenziale: il primo % prende il primo argomento dopo la stringa, il secondo % prende il secondo,
   e così via. Inserendo un numero seguito da un dollaro prima del modificatore (%1$c, %2$s), sto dicendo esplicitamente alla printf quale specifico argomento della va list prendere, scavalcando l'ordine sequenziale:
   - %1$c significa: prendi il primo argomento extra passato alla funzione e formattalo come carattere.
   - %2$s significa: prendi il secondo argomento extra e formattalo come stringa.
   Nei Quine questo è vitale perché permette di riutilizzare lo stesso identico valore (come il codice ASCII 34 o la stringa s stessa) decine di volte in punti diversi del testo senza dover impazzire a rispettare un ordine rigido di parametri nella chiamata. Variadicita' posizionale alla ennesima potenza.
   
----------DIFFERENZA TRA DEFINIZIONE MANUALE DELL'ENTRYPOINT _START(binario bare metal) E USO DELLA C RUNTIME------------------------
   Per capire perché esiste questa distinzione, bisogna guardare a come il kernel Linux e il linker (ld) gestiscono l'esecuzione di un binario ELF.
   1) L'approccio _start (Naked ELF / nasm + ld) :
      - Meccanismo del Kernel: quando il kernel esegue la syscall execve, carica l'eseguibile ELF in memoria, prepara lo stack (inserendovi argc, argv, envp) e passa il controllo direttamente all'indirizzo di memoria specificato nell'header ELF sotto il simbolo _start.
      - Assenza di Paracadute: non esiste alcun codice di inizializzazione prima della prima istruzione di _start. Lo stack non contiene un indirizzo di ritorno valido. Di conseguenza, terminare la routine con un'istruzione ret causa inevitabilmente un SegFault, perché lo stack pointer rsp punta a argc e non a un frame di chiamata.
        Per terminare un programma con _start è teoricamente obbligatorio invocare esplicitamente la syscall sys_exit.
   2) L'approccio main (C Runtime / gcc + nasm) :
      - Inizializzazione CRT (crt1.o): quando compili o linki tramite gcc, il compilatore inserisce automaticamente il proprio entry point _start fornito dalla libreria C standard.
      - Flusso di esecuzione: il kernel salta a _start della CRT. Questo codice di bootstrap inizializza i costruttori globali, allinea lo stack, estrae argc/argv dallo stack e invoca la funzione main come una normale chiamata call. All'ingresso di main, i registri contengono già i parametri della firma C classica(rdi == argc, rsi == argv, rdx == envp).
      - Ritorno Pulito: poiché main viene invocata via call, puoi terminare la funzione semplicemente con ret (restituendo il codice di uscita in rax), lasciando che la CRT gestisca la pulizia e la syscall di uscita. La C Runtime riprenderà il controllo dall'indirizzo di ritorno presente nello stack ed eseguirà la exit(rax) per mio conto. 
        Non serve alcuna syscall manuale di uscita (sys_exit).
      - Visibilità del simbolo: l'unica regola vincolante lato assembler è dichiarare il simbolo come globale (global main in NASM), in modo che il linker di gcc possa trovarlo e collegarlo alla CRT.
      - L'unica trappola: l'allineamento dello Stack. Trattare main come una normale funzione implica rispettare l'ABI di sistema quando entro ed esco. Quando la C Runtime esegue call main, spinge nello stack l'indirizzo di ritorno (8 byte). Di conseguenza, alla prima istruzione dentro main, lo stack pointer rsp si trova sfasato: rsp % 16 == 8.
        Se dentro main voglio chiamare un'altra funzione (come la sotto-routine richiesta da Colleen o la printf/write) l'istruzione call spingerà altri 8 byte nello stack. Se non correggo rsp prima della call, la funzione chiamata vedrà uno stack non allineato a 16 byte, causando un crash o comportamenti indefiniti.
        Per mantenere main aderente alle regole di una normale funzione C e poter chiamare sotto-routine in sicurezza, la struttura tipica in NASM prevede il canonico prologo/epilogo per riallineare lo stack:
         global main

         main:
            push rbp        ; Salva rbp (8 byte) -> ora rsp % 16 == 0!
            mov  rbp, rsp   ; Crea il frame pointer

            ; --- Logica di main / Chiamata a sotto-routine ---

            mov  rax, 0     ; Return value 0
            mov  rsp, rbp   ; Ripristina lo stack
            pop  rbp        ; Ripristina rbp -> ora rsp % 16 == 8
            ret             ; Torna alla C Runtime
            
      Se uso gcc, definisco global main nel codice NASM, compilo l'oggetto .o e lascio che gcc gestisca la fase di linking. Quando compilo e faccio il linking tramite gcc, per il toolchain main non è altro che una normalissima funzione definita come etichetta globale.
      Per la C Runtime (crt1.o) inserita automaticamente da gcc, il mio codice non è il punto di partenza dell'eseguibile, ma una sotto-routine che viene invocata con una classica istruzione call main.
      gcc riconosce nativamente l'estensione .s delegando il lavoro a as (GNU Assembler/GAS). Tuttavia, GAS si aspetta di default la sintassi AT&T (oppure la direttiva .intel_syntax noprefix). Per assemblare codice scritto in sintassi NASM pulita è indispensabile chiamare nasm -f elf64.
      Sui sistemi Linux recenti, gcc compila di default in modalità PIE (Position Independent Executable). Se nel codice NASM faccio riferimenti ad indirizzi di memoria usando nomi di etichette senza indirizzamento relativo, il linker potrebbe generare un errore di rilocazione (relocation R_X86_64_32S against .data...).
      Due modi per gestire la cosa: 
      - Aggiungere la direttiva default rel in cima al file .s per istruire NASM a calcolare sempre offset relativi al registro RIP. Di default, su architettura a 64 bit, NASM tratta i riferimenti a etichette in memoria come indirizzi assoluti (abs).
        Nei sistemi moderni GCC compila di default come PIE (Position Independent Executable). Se cerco di caricare un indirizzo assoluto, il linker fallisce con un errore di rilocalizzazione (relocation R_X86_64_32S against .rodata cannot be used when making a PIE object).
        Per fare codice position-independent, x86-64 usa l'indirizzamento relativo all'Instruction Pointer (RIP-relative addressing). Senza direttiva, sarei costretto a specificare rel a mano a ogni singola istruzione.
        Inserendo la direttiva default rel in cima, dico a NASM che tutti gli accessi a label di memoria devono essere RIP-relative di default. In questo modo posso scrivere semplicemente lea rdi, [msg] e NASM genererà automaticamente il codice macchina relativo a RIP.
      - Disabilitare il PIE in fase di linking passando la flag -no-pie a GCC:                 

-----------LE TRAPPOLA MORTALI DELLE FUNZIONI VARIADICHE IN ASSEMBLY----------------------------------
   Le funzioni C che utilizzano le istruzioni SIMD/SSE (come printf, che impiega registri XMM per argomenti variadici) effettuano operazioni di memoria vettoriale che generano un General Protection Fault se lo stack non è perfettamente allineato a 16 byte al momento della chiamata.
   Inoltre per la System V AMD64 ABI (il sistema standard su Linux x86-64), i primi 6 argomenti interi o puntatori non si spingono nello stack, ma vanno caricati nei registri dedicati secondo questo ordine tassativo:

   1° parametro | rdi | Puntatore alla stringa di formato (format string) |
   2° parametro | rsi | Primo valore da sostituire nel formato |
   3° parametro | rdx | Secondo valore da sostituire |
   4° parametro | rcx | Terzo valore da sostituire |
   5° parametro | r8 | Quarto valore da sostituire |
   6° parametro | r9 | Quinto valore da sostituire |

   Per passare l'indirizzo della stringa al primo parametro (in rdi) o a un parametro successivo, l'istruzione corretta è l'uso di lea (Load Effective Address) combinato con l'indirizzamento RIP-relative:
   lea rdi, [rel s]    ; Carica l'indirizzo effettivo della label s in rdi (PIE-compliant)
   Perché lea e non mov? mov rdi, s tenta di inserire un indirizzo assoluto a 32/64 bit hardcodato. Se compili in modalità PIE (Position Independent Executable, il default di gcc), il linker fallirà o genererà errori di rilocazione. lea rdi, [rel s] calcola l'indirizzo calcolando l'offset relativo al registro RIP corrente a runtime.
   Non c'è alcuna dereferenziazione in quell'istruzione: lea NON legge né tocca mai la memoria. La confusione nasce dalla sintassi di NASM, dove le parentesi quadre [...] hanno due significati diversi a seconda dell'istruzione che le usa.
   1) Con mov (Dereferenziazione vera): mov rax, [rel s]
      Le quadre dicono alla CPU di calcolare l'indirizzo di s e andare a leggere i byte in RAM presenti a quell'indirizzo per copiarli in rax. Questa è una dereferenziazione (come fare *ptr in C).
   2) Con lea ((Nessuna dereferenziazione): lea rax, [rel s]
      Le quadre servono solo per definire la formula di calcolo dell'indirizzo. lea ignora la RAM e dice alla CPU di eseguiew solo l'aritmetica (RIP + offset) e mettere il risultato del calcolo (l'indirizzo) in rax. È l'equivalente esatto dell'operatore & in C.
      È una pura regola sintattica dell'architettura x86: l'istruzione lea richiede tassativamente come operando di origine un operando di memoria (scritto tra quadre). Le quadre servono perché lea nasce per fare aritmetica sui puntatori in un singolo ciclo di clock.

   In sintassi x86/x86-64 (e in NASM in particolare), lea richiede SEMPRE le parentesi quadre. Senza le quadre l'assembler rifiuta proprio di compilare, generando un errore di sintassi.
   Il motivo è strutturale a livello di CPU: l'istruzione lea  nasce per calcolare un indirizzo di memoria, e in NASM l'espressione di un indirizzo di memoria è rappresentata tassativamente dalla notazione con le quadre.
   mov rax, [rbx] | Calcola l'indirizzo rbx e legge il valore in RAM | rax = *rbx; |
   lea rax, [rbx]`| Calcola l'indirizzo rbx e salva l'indirizzo stesso | rax = rbx; (oppure &(*rbx)) |

   In C o in Assembly a 32-bit ero abituato a scrivere semplicemente s per indicare l'indirizzo. In x86-64 con PIE (Position Independent Executable)  abilitato da gcc, questo non funziona più per due motivi:
   1) ASLR (Address Space Layout Randomization): il sistema operativo carica l' eseguibile a un indirizzo di memoria casuale ogni volta che lo avvii. L'indirizzo assoluto di s non è noto a tempo di compilazione.
   2) Limitazione delle istruzioni x86-64: non esiste un'istruzione mov rdi, <imm64> che accetti un offset relativo a 64-bit in modo efficiente per i registri dati senza generare rilocazioni complesse per il linker.
   Aggiungendo rel all'interno delle quadre [rel s] dico a NASM di calcolare la distanza in byte (offset) tra l'istruzione corrente RIP e la label s. A runtime, la CPU eseguirà RIP + offset: siccome la distanza tra il codice e la sezione dati è fissa nel binario, l'indirizzo calcolato sarà sempre corretto, indipendentemente da dove il kernel 
   ha caricato il programma in RAM.

   In C, printf è una funzione variadica. L'ABI x86-64 impone una regola precisa quando si invoca una funzione variadica: il registro rax (o meglio la sua porzione al) deve contenere il numero di registri vettoriali (XMM0–XMM7),da 1 ad 8, utilizzati per passare argomenti floating-point(float o double).
   Poiché per il Quine passo solo numeri interi o puntatori (e zero numeri in virgola mobile) e' necessario resettare rax a 0 prima di chiamare printf
   xor rax, rax
   call printf
   Se dimentico xor rax, rax, printf andrà a leggere un valore casuale rimasto in rax. Se quel valore è diverso da 0, printf proverà a salvare i registri XMM nello stack, generando un Segmentation Fault immediato.

--------MASCHERA DEI PERMESSI PER LE FUNZIONI DI IO-----------------------------------------------------
   La maschera dei permessi per le chiamate di creazione file  si esprime in ottale e segue lo schema standard POSIX diviso su 3 cifre: Proprietario (User), Gruppo (Group), Altri (Others).
   Ogni cifra è la somma di tre bit fondamentali: 4 = Lettura (r - read),2 = Scrittura (w - write),1 = Esecuzione (x - execute). Il valore standard 0644(rw-r--r--) e' la combinazione ideale per file creati da un programma (il proprietario legge e scrive, gli altri possono solo leggere):
   Altre maschere comuni: 0600(rw-------, File privato/sensibile (lettura e scrittura solo per l'user), 0755 (rwxr-xr-x ,Eseguibile o cartella standard (esecuzione/accesso per tutti).
   In C scrivo semplicemente 0644 (il prefisso 0 indica l'ottale al compilatore). In **NASM**, se scrivo 644 o 0644 viene interpretato come decimale, corrompendo la maschera dei permessi inviata al kernel!
   In NASM e' necessario specificare l'ottale o l'esadecimale in modo esplicito: 0o644 oppure 644q per l'ottale, 0x1A4 per l'hex. NASM adotta lo stesso standard di linguaggi moderni (come Python o Rust) per i prefissi numerici:
   0x per l'esadecimale ,0b per il binario e 0o per l'ottale.In alternativa al prefisso 0o, NASM accetta anche il suffisso q (da quaternary/octal): 644q.

--------DIFFERENZA TRA FLAG E PERMESSI NELLA SYSCALL OPEN-----------------------------------------------
   La differenza fondamentale tra flags e permessi (mode) nella system call open() è la distinzione tra comportamento a runtime del descrittore e metadati di sistema del file.
   FLAGS (2° argomento): come il processo usa il file
   I flags dicono al kernel in che modo il tuo processo intende interagire con il file descriptor durante questa specifica apertura. È una bitmask creata combinando costanti `O_*` tramite l'operatore bitwise OR .
   I flags si dividono in due categorie principali:
   1) Modalità di Accesso (Mutuamente esclusive) : bisogna specificarne obbligatoriamente una e una sola (i primi 2 bit del valore).
      O_RDONLY (valore decimale 0, hex 0x0): apertura in sola lettura.
      O_WRONLY (valore decimale 1,hex 0x1): apertura in sola scrittura.
      O_RDWR (valore decimale 2, hex 0x2): apertura in lettura e scrittura.
   2) Modificatori di Controllo e Creazione (Combinabili via OR)
      O_CREAT(valore decimale 64,hex 0x40): se il file non esiste sul filesystem, lo crea. Richiede l'invocazione del 3° argomento (mode`).
      O_TRUNC(valore decimale 512,hex 0x200): se il file esiste già ed è aperto in scrittura (O_WRONLY o O_RDWR), ne azzera la lunghezza a 0 byte (lo svuota).
      O_APPEND(valore decimale 1024,hex 0x400): ogni operazione di scrittura (write) sposta automaticamente l'offset alla fine del file (EOF) prima di scrivere.
      O_EXCL: usato esclusivamente insieme a O_CREAT. Se il file esiste già, open() fallisce e restituisce -1 settando errno a EEXIST. Garantisce la creazione atomica del file senza race conditions.

   MODE / Permessi (3° argomento): chi può fare cosa sul filesystem,espressi in ottale(04 read,02 write,01 exec; in Assembly l'ottale si esprime con 0o)
   I permessi definiscono i diritti di accesso POSIX (rwx) scritti nell'inode del file sul filesystem. Vengono presi in considerazione dal kernel esclusivamente se flags contiene O_CREAT (o O_TMPFILE). Se il file esiste già, il 3° argomento viene totalmente ignorato dal kernel.
   È una rappresentazione ottale a 3 cifre che definisce i bit di accesso per Proprietario (User), Gruppo (Group) e Altri (Others).

   L'effetto della umask:i permessi reali scritti sull'inode non sono mai identici al parametro mode passato a open(), ma vengono filtrati tramite la umask del processo: Permessi Effettivi = mode & ~ umask
   Esempio: se chiedi 0666 e la umask di sistema è 0022, il file verrà creato con permessi 0644).

   Nel rispetto della System V AMD64 ABI, il secondo argomento di qualsiasi funzione intera/puntatore si passa nel registro RSI.
   In Assembly, però, non ci sono a disposizione le costanti simboliche del C (O_WRONLY, O_CREAT, O_TRUNC dell'header <fcntl.h>): si deve passare direttamente il valore numerico della maschera bitwise: O_WRONLY = 0x01 (decimale 1), O_CREAT = 0x40 (decimale 64, ottale 0100), O_TRUNC = 0x200 (decimale 512, ottale 01000).
   Eseguendo il bitwise OR tra i tre valori: 0x01 | 0x40 | 0x200 = 0x241 (decimale 577)


---------MACRO IN C E ASSEMBLY---------------------------------------------------------------------
   Nello standard C, le direttive del preprocessore (come #define) terminano tassativamente alla fine della riga fisica (al primo carattere \n). Il carattere backslash \ posto immediatamente prima di un a capo attiva la cosiddetta splicing phase (fase 2 della compilazione C):
   dice al preprocessore di ignorare la newline e considerare la riga successiva come la continuazione logica della stessa direttiva. Permette di scrivere una macro su più righe leggibili anziché condensare tutto su un'unica riga orizzontale illeggibile di 200 caratteri.

   NASM ha due tipi di macro nel suo preprocessore:
   1) Macro a riga singola: %define .È l'equivalente diretto del #define del C. Sostituisce testualmente identificatori o costanti:

   Snippet di codice
   %define FILENAME "Grace_kid.s"
   %define FLAGS    0x241            ; O_WRONLY (0x1) | O_CREAT (0x40) | O_TRUNC (0x200)
   %define MODE     0o644            ; 0644 ottale (oppure 0x1A4)

   2) Macro multi-linea: %macro / %endmacro . Permette di definire blocchi interi di istruzioni Assembly. La sintassi richiede il nome della macro e il numero di parametri attesi (se non accetta parametri, si mette 0):

   Snippet di codice
   %macro NOME_MACRO numero_argomenti
      ; istruzioni assembly
   %endmacro
   Per invocarla nel codice, scrivo semplicemente il suo nome (senza prefisso % e senza parentesi):

   A livello di sistema operativo, la trasformazione di un file di testo appena scritto su disco in un nuovo processo attivo nello scheduler della CPU richiede la cooperazione di tre sottosistemi del kernel Linux: il **Virtual Memory Manager**, il sottosistema dei **Processi (task management)** e l'**ELF Loader**.

--------STRINGIFICAZIONE-----------------------------------------------------------------------
La stringificazione (stringification) è un trucco del preprocessore C che usa l'operatore # per trasformare automaticamente un blocco di codice in una stringa letterale racchiusa tra doppie virgolette, senza dover impazzire con i codici ASCII per i ritorni a capo o le virgolette stesse. 
Se scrivo #define QUINE(code) char *s = #code, tutto quello che passo a QUINE diventa una stringa s formattata. Un Quine puo' quindi essere scritto banalmente senza usare le formattazioni complesse richieste da printf. 
Esempio concettuale: 
#include <stdio.h>

#define QUINE(code) int main() { char *s = #code; printf("#include <stdio.h>\n\n#define QUINE(code) int main() { char *s = #code; %s\nQUINE(%s)\n", code, s); }

QUINE(
    printf("#include <stdio.h>\n\n#define QUINE(code) int main() { char *s = #code; %s\nQUINE(%s)\n", code, s);
)

--------LA MACRO __FILE__ ---------------------------------------------------------------------
È una macro standard predefinita del preprocessore C ANSI (come __LINE__, __DATE__ o __TIME__).Non è una variabile a runtime, ma un costrutto a tempo di compilazione. Quando lanciO gcc, durante la fase di preprocessing, il compilatore cerca ogni occorrenza di __FILE__ nel  codice e la sostituisce brutalmente con una stringa letterale 
contenente il nome del file che sta elaborando in quel momento Quando usO __FILE__, il compilatore si limita ad allocare la stringa "Sully_X.c" nella sezione .rodata (Read-Only Data) del binario ELF e piazza un puntatore a quell'indirizzo di memoria nel punto in cui ho usato la macro.
Questo significa che l'identità del programma è scolpita nel binario stesso nel momento esatto in cui faccio la execve di gcc. Il programma diventa autoconsapevole del proprio "DNA sorgente".
Nel file originario, __FILE__ viene espanso in "Sully.c". Quando il programma genera e compila Sully_5.c, dentro il nuovo binario __FILE__ diventerà "Sully_5.c".
In questo progetto, questa macro viene spesso usata da chi sceglie di lanciare effettivamente i binari generati tramite execve al posto della ricorsione del main. Siccome un nuovo processo lanciato da execve non condivide la memoria col padre, perde il valore della variabile static. 
Usando __FILE__, il programma legge il proprio DNA per capire dove si trova nella catena: "Il mio file sorgente era Sully.c? Allora sono il padre, imposto a 5 e creo il primo figlio. Era Sully_X.c? Allora estraggo il numero dal mio nome e decremento."
NASM ha una macro equivalente che si chiama %__FILE__ che espande al nome del file sorgente corrente in fase di assemblaggio. Per inserirla nel binario come stringa terminata da null, la si definisce così:

section .rodata
    file_name: db %__FILE__, 0

--------PIPELINE DI SULLY : FORK, EXECVE, WAIT----------------------------------------------------
   Per orchestrare la compilazione e l'esecuzione del figlio dall'interno del programma, esistono due paradigmi teorici: la pipeline a basso livello basata su chiamate di sistema native (fork + execve\ + waitpid) e l'astrazione ad alto livello fornita dalla funzione di libreria system().

   1) La Pipeline Nativa: fork, execve e waitpid
      Nel modello UNIX puro, la creazione di un programma non avviene in un unico passaggio, ma scindendo la duplicazione del contesto d'esecuzione dalla sostituzione dell'immagine binaria.
      Quando il processo genitore decide di compilare o eseguire, invoca la syscall fork:
      Il kernel non copia fisicamente l'intera memoria RAM occupata dal genitore. Duplica soltanto la tabella delle pagine (Page Table) e marca tutte le pagine di memoria fisica come Copy-on-Write (COW) a sola lettura.
      Viene allocato un nuovo task struct con un PID univoco. Da questo istante esistono due flussi d'istruzione identici che riprendono l'esecuzione dall'istruzione immediatamente successiva alla syscall: il genitore riceve come valore di ritorno il PID del figlio, mentre il figlio riceve 0.
      Il processo figlio invoca execve specificando il percorso dell'eseguibile (prima il compilatore, poi il binario del quine figlio). Il kernel distrugge l'intero spazio d'indirizzamento virtuale del processo invocante: lo stack, l'heap, il segmento .data e il segmento .text vengono spazzati via e deallocati.
      L'ELF Loader del kernel mappa in memoria i segmenti del nuovo file binario (PT_LOAD), prepara il nuovo stack iniettando argomenti (argv) e variabili d'ambiente (envp), e reimposta l'Instruction Pointer (RIP) all'entry point del nuovo eseguibile (_start).
      Nota critica sui file descriptor: a differenza della memoria, i file descriptor aperti rimangono aperti e condivisi attraverso execve, a meno che non siano stati marcati preventivamente con il flag FD_CLOEXEC.
      Non è possibile avviare il binario figlio prima che il compilatore abbia terminato di scriverlo su disco e chiuso il suo file descriptor. Se tentassi di eseguire un binario parzialmente scritto, il kernel rifiuterebbe l'esecuzione con l'errore ETXTBSY (Text file busy) o l'interprete ELF fallirebbe nel caricamento delle intestazioni corrotte.
      Il genitore deve sospendere la propria esecuzione tramite waitpid, cedendo la CPU fino a quando il processo del compilatore non transita nello stato di terminazione (zombie) restituendo il proprio codice di stato (exit status). Solo se il compilatore è uscito con stato 0, il genitore è autorizzato a generare un secondo processo per eseguire il binario appena prodotto.

   2) L'Astrazione di Libreria: system()
      La funzione di libreria system(const char *cmd) incapsula internamente l'intera sequenza fork --> execve --> waitpid, ma introduce un intermediario fondamentale: la shell di sistema (/bin/sh).
      Quando si invoca system: la C runtime esegue una fork(),poi il processo figlio esegue execve puntando a /bin/sh passando come argomenti i flag -c e la stringa del comando,infine il processo genitore si blocca in una chiamata waitpid() mascherando temporaneamente i segnali SIGINT e SIGQUIT e bloccando SIGCHLD.
      L'utilizzo di un interprete di comando permette di sfruttare l'operatore booleano di sequenziamento && : Compilazione && Esecuzione.
      La shell garantisce a livello sintattico e temporale la serializzazione deterministica: il secondo comando viene invocato soltanto se il primo si conclude con exit code 0. La sincronizzazione è implicita: la shell attende la chiusura dei descrittori del compilatore prima di passare il controllo al loader per il nuovo processo.

   Un processo in ambiente UNIX non è semplicemente un file binario in esecuzione, ma un'istanza viva gestita dal kernel, composta da due elementi fondamentali: uno spazio d'indirizzamento virtuale isolato (gestito dalla MMU tramite tabelle delle pagine) e un contesto di esecuzione nel kernel (rappresentato in Linux dalla struttura task_struct).
   Il task_struct contiene tutte le informazioni di stato: identificativo del processo (PID), identificativo del genitore (PPID), registri CPU correnti, credenziali utente, maschera dei segnali e la File Descriptor Table (la tabella dei descrittori di file aperti).
   Ogni processo ha la sua Page table : la propria gerarchia di tabelle delle pagine a 4 livelli (PML4, PDPT, PD, PT su x86-64). L'indirizzo fisico in RAM della radice della tabella delle pagine del processo (la PML4) viene caricato dal kernel all'interno del registro di controllo della CPU CR3 a ogni context switch.
   Questo è il motivo per cui due processi distinti possono puntare entrambi allo stesso indirizzo virtuale (es. 0x400000), ma la MMU della CPU risolverà quell'indirizzo virtuale traducendolo in frame di memoria RAM fisica completamente distinti.
   Quando il sistema operativo decide di togliere la CPU al tuo processo per darla a un altro (Context Switch): il kernel salva una copia esatta del valore di ogni singolo registro della CPU all'interno di una sotto-struttura del task_struct (chiamata thread_struct) o sullo stack kernel del processo.
   Quando il processo viene ricaricato sulla CPU dallo scheduler, il kernel rilegge quei valori dalla memoria e li ricarica fisicamente nei registri della CPU. Il registro RIP (Instruction Pointer) riprenderà esattamente dall'istruzione in cui era stato interrotto, rendendo la sospensione invisibile al codice.
   POSIX specifica che con la fork() il figlio parte con i contatori di tempo azzerati: statistiche di CPU come i contatori di tempo speso in user-space e kernel-space (tms_utime, tms_stime) tornano a 0.Lo stesso per i timer di allarme: se il padre aveva programmato un allarme o un timer periodico (tramite le chiamate alarm(), setitimer() o timer_create()), 
   questi timer pendenti non vengono ereditati dal figlio; per il figlio vengono cancellati.
   In UNIX, ogni processo (ad eccezione di PID 1, tipicamente systemd o init) nasce da un altro processo mediante un rapporto gerarchico asimmetrico: 
   - il Padre e' il processo generatore, mantiene il controllo sul ciclo di vita del figlio; ha l'obbligo contrattuale nei confronti del sistema operativo di raccoglierne lo stato di terminazione.
   - il Figlio e' il processo clonato, nasce con una copia quasi esatta dello stato del padre, ma con un'identità autonoma (nuovo PID, proprio spazio di indirizzamento, timer azzerati).
   La chiamata di sistema pid_t fork(void) duplica il processo chiamante. La sua caratteristica unica è che viene chiamata una volta sola, ma ritorna due volte in due spazi di memoria ora distinti.
   Cioe' la fork() non è una chiamata a funzione che restituisce due valori nello stesso processo, ma è una clonazione istantanea che genera due flussi di esecuzione indipendenti. Nel momento esatto in cui viene eseguita la fork():
   il kernel crea il clone,e da quel momento due processi separati (Padre e Figlio) stanno eseguendo esattamente le stesse istruzioni a partire dalla riga successiva alla fork(). L'unica differenza risiede nel valore della variabile pid in cui è stato salvato il ritorno di fork():
   nel Padre, pid contiene un numero positivo (il PID del figlio),mntre nel Figlio, pid contiene 0.
   L'istruzione if pid == 0/ else if pid > 0 sfrutta questa asimmetria per separare i destini dei due processi:quello del padre entrera' nel ramo pid > 0,il figlio nel ramo pid == 0.
   La CPU riprende l'esecuzione esattamente dall'istruzione macchina successiva alla syscall fork. Il kernel inserisce nel registro RAX dei due processi due valori differenti per permettere al codice di discriminare il proprio ruolo:
   - nel processo Padre  fork() restituisce il PID del figlio (un intero positivo > 0). Il padre deve memorizzare questo PID per poter identificare quel figlio specifico in seguito.
   - nel processo Figlio fork() restituisce 0 : il valore 0 non indica un PID reale, ma è un marcatore convenzionale per identificare il processo appena nato. Per conoscere il proprio vero PID, il figlio deve invocare getpid().
   In caso di errore fork restituisce -1 nel contesto del padre (es. limite massimo di processi di sistema raggiunto, EAGAIN/ENOMEM); nessun processo figlio viene creato.
   In passato, fork() copiava fisicamente l'intero contenuto della RAM del padre nel figlio, un'operazione lentissima e vorace di risorse. I moderni kernel x86-64 usano il Copy-on-Write: alla fork(), il kernel si limita a duplicare la Page Table del padre per assegnarla al figlio.
   Tutte le pagine di memoria virtuale di entrambi i processi vengono marcate con il flag hardware di sola lettura (Read-Only) ed entrambi i processi leggono dalle medesime pagine di RAM fisica. Non appena uno dei due (padre o figlio) tenta di modificare una variabile (scrivere in memoria), la MMU della CPU genera un Page Fault.
   Il kernel intercetta il fault, alloca una nuova pagina fisica di RAM, vi copia i dati originali, assegna la nuova pagina al processo che voleva scrivere e rimarca la pagina come scrivibile.
   La fork() clona anche la tabella dei descrittori di file: vengono duplicati gli indici della tabella del processo (0, 1, 2, fd...), ma le voci duplicate puntano alle medesime strutture di file aperto nel kernel (Open File Description). Ciò significa che padre e figlio condividono il file offset: se il figlio sposta il puntatore 
   di lettura/scrittura con un'operazione di I/O o con lseek, il cursore si sposta contemporaneamente anche per il padre.
   Mentre fork() crea un clone che esegue lo stesso codice, execve trasforma il processo corrente caricando ed eseguendo un nuovo programma binario: int execve(const char *pathname, char *const argv[], char *const envp[]);
   Il kernel dealloca completamente il vecchio spazio d'indirizzamento virtuale (stack, heap, .data, .text). Il vecchio codice scompare dalla memoria. L'ELF Loader del kernel legge il file specificato in pathname, ne mappa i segmenti PT_LOAD nelle nuove pagine di memoria virtuale e alloca un nuovo stack pulito.
   Il kernel popola la cima del nuovo stack con i parametri passati:
   - argv: array di puntatori a stringhe che rappresentano gli argomenti a riga di comando. Deve essere tassativamente terminato da un puntatore NULL. Convenzione vuole che argv[0] contenga il nome del comando stesso.
   - envp: array di puntatori a stringhe contenenti le variabili d'ambiente (CHIAVE=VALORE), terminato anch'esso da NULL.
   Il registro Instruction Pointer RIP della CPU viene impostato all'entry point del nuovo eseguibile (_start).
   Se la chiamata execve ha successo, essa NON ritorna mai, perché il codice chiamante non esiste più: è stato rimpiazzato dal nuovo programma.Ritorna solo in caso di fallimento (valore -1), impostando errno (es. ENOENT se il file non esiste, EACCES se mancano i permessi di esecuzione). Se vedo un'istruzione eseguita subito dopo una execve, 
   significa matematicamente che la chiamata è fallita.
   Il PID e il PPID del processo che ha invocato execve sopravvivono alla chiamata rimanendo identici: per il kernel è sempre lo stesso processo. I File Descriptor aperti rimangono aperti e fruibili dal nuovo programma, a meno che al momento dell'apertura non sia stato impostato il flag O_CLOEXEC (o tramite fcntl con FD_CLOEXEC).
   Un processo non svanisce nel nulla quando termina con exit() o con un return da main. Il sistema operativo deve consentire al genitore di sapere come e perché il figlio è morto:  pid_t waitpid(pid_t pid, int *wstatus, int options);
   Quando un figlio invoca exit(code) :
   1. il kernel distrugge la sua memoria virtuale e chiude i suoi file descriptor (rilasciando la RAM).
   2. il kernel mantiene in vita la sua task_struct ,quella del figlio, nella Process Table del kernek, congelando: il PID, il codice di uscita e le statistiche sull'uso delle risorse.
   3. in questa fase il processo è uno Zombie (visibile con ps come <defunct>).
   4. lo zombie rimane nel kernel fino a quando il padre non esegue una chiamata della famiglia wait(). Nel momento in cui il padre raccoglie lo stato con waitpid(), lo zombie viene definitivamente rimosso dal sistema ("sepolto").
   L'exit code e' un valore numerico a 8 bit (intervallo da 0 a 255) che il processo uscente invia al sistema operativo per comunicare il proprio esito. Se in C scrivo exit(42) o return 42 il numero 42 viene salvato dal kernel nel task_struct del figlio. 
   Per convenzione UNIX: 0 significa successo, qualsiasi valore diverso da zero (1-255) indica un codice di errore personalizzato.Se passo numeri maggiori di 255 (es. exit(256)), il kernel considera solo il modulo a 8 bit (256 & 0xFF = 0).
   Se il padre muore prima del figlio senza attenderlo, il figlio diventa "orfano". Il kernel riassegna immediatamente il PPID del figlio a PID 1 (systemd/init), il quale chiama periodicamente wait() ripulendo automaticamente gli orfani terminati.
   PID 1 e' il primo processo utente avviato dal kernel al termine della fase di bootstrap della macchina. È la radice assoluta dell'albero dei processi. Ha un dovere istituzionale primario: l'adozione degli orfani.
   Quando un processo genitore muore prima del figlio, il kernel cambia il PPID (Parent PID) del figlio impostandolo a 1. systemd/init include un loop infinito che esegue chiamate wait() asincrone per raccogliere e cancellare gli stati zombie lasciati dai processi orfani, evitando il saturamento della memoria kernel.

   La barriera del Kernel Linux (Post-PwnKit / CVE-2021-4034):storicamente, se un programmatore passava NULL come puntatore argv a execve(path, NULL, envp), il kernel Linux lo tollerava creando uno stack anomalo con argc = 0.
   Nel 2021 questa discrepanza ha generato la celebre vulnerabilità PwnKit (CVE-2021-4034): programmi SUID come pkexec, scorrendo argv con un ciclo for (i = 1; i < argc; i++) o assumendo che argv[1] fosse adiacente ad argv[0], finivano per leggere e interpretare come argomenti le stringhe appartenenti all'array delle variabili d'ambiente (envp),
   consentendo l'escalation immediata a root. A partire da Linux 5.18, il kernel rifiuta formalmente o corregge questa anomalia: se si tenta di passare un argv vuoto o nullo, il kernel forza l'inserimento di una stringa vuota fittizia "" come argv[0] per impedire l'esecuzione con argc == 0.

   Parametri di waitpid
   - pid: 0 attende il figlio specifico avente quel determinato PID, -1 attende un qualunque figlio generato dal processo corrente (equivalente a wait()). wait() infatti sospende il processo padre finché uno qualsiasi dei suoi processi figli termina. Se ho fatto 5 fork e ho 5 figli in esecuzione, 
     il primo che muore (per qualsiasi motivo) sbloccherà la wait. Non si puo' scegliere quale aspettare. Curiosità di basso livello: a livello di system call Linux, wait(&status) in realtà non esiste come entità separata; è letteralmente implementata nella libc come un wrapper che chiama waitpid(-1, &status, 0).
   - wstatus: puntatore a un intero a 32 bit dove il kernel scriverà un bitfield codificato con il codice di uscita e il motivo della terminazione. Se passo &status (per es. con waitpid(pid, &exit_status, 0)) il kernel salva lì dentro i dati. Poi posso usare macro come WIFEXITED(status) e WEXITSTATUS(status) 
     per estrarre l'exit code. Se invece passo NULL (es. wait(NULL) o waitpid(pid, NULL, 0)) diro' al kernel di non essere interessato all'exit code ne' ai motivi della terminazione.Voglio solo sapere che quel processo e' terminato per poter procedere con altro. Il kernel butta via i dati di uscita.
   - options : flag di comportamento . 0 per attesa bloccante sincrona, lo scheduler del kernel mette il processo padre nello stato di riposo (TASK_INTERRUPTIBLE), rimuovendolo dalla coda dei processi eseguibili; il processo non consuma alcun ciclo di CPU e rimane congelato su quella riga finché il figlio specificato 
     non cambia stato (termina o crasha). Se invece passassi il flag WNOHANG, la chiamata diventerebbe asincrona/non bloccante: se il figlio è ancora in esecuzione, waitpid() non blocca il padre ma ritorna immediatamente 0 senza attendere, permettendo al genitore di fare altre operazioni,andando a controllare ciclicamente(polling).Questo wait() non puo' farlo,
     e' ua feature esclusiva di waitpid,il cd non blocking.
   Il valore scritto in wstatus non è semplicemente il codice passato a exit(), ma un bitfield che impacchetta informazioni sulla causa della morte (uscita pulita vs segnale di crash). Non si legge mai direttamente con operazioni aritmetiche grezze, ma tramite le apposite macro:
   | WIFEXITED(status) | Restituisce vero se il figlio è terminato volontariamente (return o exit()). |
   | WEXITSTATUS(status) | Valida solo se WIFEXITED è vero. Estrae gli 8 bit bassi dell'argomento di exit (0-255) con un (status >> 8) & 0xFF. |
   | WIFSIGNALED(status) | Se WIFEXITED e' false, restituisce vero se il figlio è stato terminato bruscamente da un segnale non intercettato (SIGSEGV, SIGKILL, SIGBUS ecc.). |
   | WTERMSIG(status) | Valida solo se WIFSIGNALED è vero. Restituisce il numero del segnale che ha ucciso il figlio. |

   Quale e' la differenza tra SIGSEGV e SIGKILL?
   Entrambi provocano la terminazione del processo se non intercettati, ma con meccaniche radicalmente diverse:
   - SIGSEGV (Signal 11 - Segmentation Violation): e' scatenato dall'hardware (la MMU segnala un accesso a memoria non autorizzato, pagina non presente, o scrittura su pagina RO).
     È intercettabile: il processo può definire un gestore di segnale personalizzato con sigaction() per tentare un salvataggio dei dati o stampare un log di debug prima di morire.
   - SIGKILL (Signal 9 - Unconditional Kill): e' una direttiva d'arresto immediata emessa dal sistema operativo o da un utente root/autorizzato (il classico kill -9).
     Non può essere intercettato, gestito né ignorato: il kernel non invia alcuna notifica al processo; distrugge direttamente il suo spazio di memoria virtuale e ne arresta l'esecuzione istantaneamente.

   Il pattern con cui una shell o un programma coordinatore manda in esecuzione un comando esterno (come la compilazione del sorgente figlio) si articola in tre momenti ordinati:

   [ Processo Genitore ]
         |
         +--- 1. fork() --------------------------------+
         |                                              |
         | (pid > 0)                                    | (pid == 0)
         v                                              v
      waitpid(pid, &status, 0)                  execve("/usr/bin/gcc", ...)
      [Sospeso nello scheduler]                           |
         |                                              | [Esecuzione binario esterno]
         |                                              v
         |                                         exit(0)
         |                                              |
         |<--- Notifica del Kernel (SIGCHLD) -----------+
         |     [Zombie eliminato]
         v
      Analisi macro:
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
         -> Successo: possiamo procedere alla fase successiva

   Che differenza c'e' tra exit e return?
   Nel punto terminale del main(), fare return code; ed eseguire exit(code); producono lo stesso effetto finale perché la funzione di avvio della libc (__libc_start_main) fa internamente: exit(main(argc, argv, envp));
   La differenza emerge quando sei fuori dal main (in funzioni secondarie):
   - return code : termina solo la funzione locale corrente, distruggendo il relativo stack frame e restituendo il valore alla funzione chiamante.
   - exit (code) : termina istantaneamente l'intero processo da qualsiasi punto dell'albero delle chiamate, invoca tutte le funzioni di cleanup registrate con atexit(), svuota i buffer di I/O dello user-space (fflush) e richiama la syscall exit_group del kernel.
   Esiste anche _exit() / _Exit(), che chiude il processo all'istante a livello kernel senza svuotare i buffer di I/O della libc).

   Perché l'istruzione dopo execve scatta solo in caso di errore? execve è il punto di non ritorno di un processo: se ha successo, il kernel distrugge completamente il codice, i dati, lo stack e l'heap del programma, rimpiazzandoli con quelli del nuovo binario.
   In quel preciso istante, le istruzioni successive alla execve nel codice sorgente cessano fisicamente di esistere nella memoria virtuale del processo. Di conseguenza, il flusso di esecuzione non potrà mai raggiungere la riga con _exit(1).
   Se invece execve fallisce , la chiamata restituisce immediatamente -1. Il codice è ancora lì in memoria e il flusso di esecuzione cade sulla riga successiva. A quel punto bisogna interrompere il figlio con _exit(1). Se non lo facessi, il figlio continuerebbe a eseguire 
   il resto del codice del main facendo disastri.

   Quando execve carica un programma (come gcc), quel programma ha un suo main interno scritto dai suoi sviluppatori, che termina con un return o una chiamata a exit().
   Quando quel programma termina,esso stesso invoca internamente exit(0) se e' andato a buon fine,o exit(1) o altro codice di errore in caso di errore.
   Il kernel intercetta quella chiamata di uscita del binario caricato, distrugge il processo figlio e trasforma il suo stato in Zombie, svegliando il padre bloccato sulla waitpid().
   Di conseguenza, il codice di Sully non deve preoccuparsi di come uscirà il figlio in caso di successo, perché il controllo passa interamente nelle mani del programma esterno, il cui exit code verrà catturato dal padre tramite WEXITSTATUS(status) nella waitpid().


----L'ANATOMIA ESATTA DELLA ESECUZIONE A CASCATA DI SULLY------------------------------------:
1) LanciO ./Sully dal terminale.
2) ./Sully crea Sully_5.c, lo compila in Sully_5, fa una fork e lancia ./Sully_5. Poi si mette in wait.
3) ./Sully_5 crea Sully_4.c, lo compila in Sully_4, fa una fork e lancia ./Sully_4. Poi si mette in wait.
4) Questa catena continua finché non nasce ./Sully_0.
5) ./Sully_0 si sveglia, vede che runs <= 0, fa spallucce ed esce subito con return 0 senza creare figli.
6) A questo punto parte il collasso della catena (l'effetto domino al contrario):
7) ./Sully_0 muore.
8) ./Sully_1 che era in wait vede morire il figlio, e muore a sua volta.
9) ./Sully_2 vede morire il 1, e muore.
10) ...fino al ./Sully originale, che muore e restituisce finalmente il prompt del terminale.
È un'architettura bellissima da vedere in azione con utility come htop o lanciando il programma sotto strace -f (che traccia anche i figli). Si vedono proprio i processi che nascono in cascata, si mettono in attesa, 
e poi collassano tutti insieme quando l'ultimo anello decide di fermarsi. Questo è il comportamento autentico di un software autoreplicante (worm) su architetture POSIX.

-------COME PRENDO LE VARIABILI D'AMBIENTE DEL PADRE E LE PASSO AL FIGLIO?-------------------------------
In C hai due modi standard e immediati per recuperare le variabili d'ambiente del processo corrente e passarle direttamente a execve tramite il suo terzo parametro (envp).

Metodo 1: La variabile globale environ
Non c'e' bisogno di dichiarare parametri nel main. La libreria standard e il kernel espongono una variabile globale extern chiamata environ:

extern char **environ;

environ punta già all'array di stringhe in formato CHIAVE=VALORE terminato da NULL che il kernel ha allocato sullo stack all'avvio del programma.

Metodo 2: il terzo argomento del main
Lo standard POSIX (e l'estensione GCC) consente di definire il main accettando tre argomenti anziché due:

int main(int argc, char **argv, char **envp)

A livello di esecuzione il risultato è identico a environ. Scegliere l'uno o l'altro dipende solo da dove e' più comodo recuperare il puntatore nel codice (nel main o dentro una funzione profonda tramite extern).
Per prendere invece una singola variabile d'ambiente e non tutto l'array si usa getenv(),che trova il valore di una singola variabile d'ambiente passatale come stringa (es. char *path = getenv("PATH");). 
Restituisce un singolo valore (char *), non un array.

------QUANTI SONO REALMENTE I REGISTRI DI UNA CPU MODERNA?----------------------------------------------

Esiste un solo set di registri architetturali (RAX, RBX, ecc.) per singolo core logico, ma padre e figlio non condividono mai quel registro fisico nello stesso istante.
La coesistenza di due valori diversi è resa possibile da due meccanismi: la multiplexazione temporale (schedulazione e context switch) su singolo core, e la separazione spaziale su architetture multi-core.

1. Il caso Single-Core: Multiplexazione Temporale (Time-Sharing)
   Se il computer avesse una CPU con un solo core fisico e un solo thread hardware, esisterebbe letteralmente un unico registro RAX di silicio. Padre e figlio non girano in parallelo nello stesso identico ciclo di clock, ma in sequenza temporale.
   Il trucco risiede nel modo in cui il kernel gestisce l'uscita dalla system call tramite lo stack kernel del processo:

   - Padre chiama fork, una syscall
   -> La CPU passa in Ring 0 (Kernel Space).
   -> Il kernel salva lo stato di TUTTI i registri della CPU nello stack kernel del padre
      all'interno di una struttura dati C chiamata struct pt_regs.

   - Il kernel alloca un nuovo task_struct e un nuovo kernel stack per il Figlio
   -> Copia la struct pt_regs del padre nello stack kernel del figlio.
   -> Ora ci sono DUE copie dei registri congelate in RAM.

   Il Kernel manipola i valori di ritorno in RAM:
   - Nel pt_regs del PADRE:  scrive regs->ax = child_pid;
   - Nel pt_regs del FIGLIO: scrive regs->ax = 0;

   - Ritorno in User Space (Ring 3):
   - Se lo scheduler decide di far ripartire il PADRE:
     Esegue l'istruzione di ritorno (sysretq o iretq). La CPU rilegge il frame
     pt_regs del padre e ripopola i registri hardware. Nel registro fisico RAX
     viene caricato child_pid.
   - Quando lo scheduler sospende il padre e cede la CPU al FIGLIO (Context Switch):
     Salva i registri del padre in RAM, carica nel registro fisico RAX il valore
     estratto dal pt_regs del figlio (0) ed esegue sysretq.

    Il registro fisico RAX è uno solo, ma in un istante temporale t_0 contiene il PID del figlio, e in un istante t_1 contiene 0.

2. Il caso Multi-Core e SMT (Hyper-Threading)
Sui processori moderni (multi-core o con Simultaneous Multithreading), la separazione è anche fisica:
Ogni singolo Core logico ha il proprio set dedicato di registri architetturali. Una CPU a 8 core ha fisicamente almeno 8 registri architetturali RAX indipendenti operativi in parallelo sul die di silicio.
Se lo scheduler assegna il padre al Core 0 e il figlio al Core 3, i due processi avanzano nel medesimo istante di tempo: il registro RAX del Core 0 conterrà il PID, mentre il registro RAX del Core 3 conterrà contemporaneamente 0.

3. La realtà microarchitetturale: Register Renaming
Se scendiamo all'estremo dettaglio hardware dei processori moderni (Out-of-Order Execution) anche all'interno di un singolo core fisico, un registro non è un singolo cassetto di memoria hardware, ma un'etichetta logica dell'Architectural Register File (ARF).
La CPU fisica contiene al suo interno un pool molto più ampio di registri fisici reali, il Physical Register File (PRF) (spesso 160–224 registri fisici o più).
Un blocco logico chiamato RAT (Register Alias Table) esegue il register renaming, mappando dinamicamente il nome architetturale (ad es. RAX) a uno specifico registro fisico temporaneo del chip.
Quando il processo cambia contesto, il kernel aggiorna i puntatori di stato e la CPU punta semplicemente a mapping di registri fisici differenti.
Il programmatore vede un'astrazione immutabile (il registro RAX), ma a garantire l'isolamento dei valori ci pensano la memoria RAM del kernel prima e la logica hardware della CPU dopo.

Quindi in sostanza e' fondamentale la distinzione tra il livello logico determinato dalla osservanza dell'ISA e quello microarchitetturale fisico della struttura microscopica del die.
Livello Architetturale (ISA x86-64): il contratto prevede 16 registri general-purpose (RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8–R15),che sono un'astrazione dell'instruction set.Ogni thread logico (hardware thread) deve esporre al software uno stato architetturale isolato 
e indipendente.In una CPU 8-core con SMT/Hyper-Threading (16 thread logici), il sistema operativo vede a tutti gli effetti 16 * 16 = 256 registri architetturali interi distribuiti su tutta la macchina.Dal punto di vista del compilatore e del programmatore assembly, ciascuno di quei 16 
thread logici possiede il proprio RAX privato.
Livello Microarchitetturale: all'interno del silicio reale del die di un singolo core fisico moderno (architetture Out-of-Order superscalari come AMD Zen o Intel Core), la realtà hardware è completamente svincolata da quei 16 nomi storici.
Sul silicio di ogni singolo core non esistono 16 registri, ma un pool unificato (PRF) di celle SRAM ad altissima velocità (ad esempio ~192 registri interi in AMD Zen 3, ~280 in Intel Golden Cove). Quando due thread logici girano sullo stesso core fisico, condividono lo stesso PRF unificato .
(gestito con partizionamento statico o allocazione dinamica a contesa). Ciascun thread logico ha però la propria RAT (Register Alias Table) dedicata, che tiene traccia di quale registro fisico del pool corrisponda in quel momento a ciascuno dei suoi registri logici.
Register Renaming: più registri fisici per lo stesso registro logico. La conseguenza chiave di questa architettura è che non solo un singolo registro logico viene mappato su registri fisici diversi in tempi successivi, ma possono esistere contemporaneamente più registri fisici attivi che contengono versioni generazionali diverse 
dello stesso registro logico .Questo serve a eliminare i falsi conflitti sui dati (falsi hazard).
Dipendenza                 Tipo                                Causa                                                                                                            Risoluzione Hardware
RAW (Read After Write)  Vera dipendenza                        L'istruzione B legge il dato prodotto dall'istruzione A.                                                        Impossibile da eliminare: B deve attendere A.
WAR (Write After Read)  Falsa dipendenza (Anti-dipendenza)     L'istruzione B sovrascrive un registro prima che un'istruzione precedente A lo abbia finito di leggere.         Risolta dal Register Renaming.
WAW (Write After Write) Falsa dipendenza (Output dipendenza)   Due istruzioni scrivono nello stesso registro architetturale.                                                   Risolta dal Register Renaming.

Esempio pratico nella pipeline di esecuzione, istruzioni eseguite in sequenza stretta.Snippet di codice
mov rax, [mem1]       ; Istruzione 1
add rbx, rax          ; Istruzione 2 (legge rax)
mov rax, [mem2]       ; Istruzione 3 (sovrascrive rax: falsa dipendenza con 2)
imul rcx, rax         ; Istruzione 4 (legge il nuovo rax)
Se ci fosse un solo registro RAX fisico:L'istruzione 3 dovrebbe attendere che l'istruzione 2 finisca di leggere RAX, serializzando inutilmente la pipeline.
Con il Register Renaming via RAT:
1) La RAT assegna per l'Istruzione 1: RAX ---> registro fisico P12.
2) L'Istruzione 2 legge da P12.
3) All'Istruzione 3, la RAT assegna istantaneamente un nuovo registro fisico libero: RAX ---> P45.
4) L'Istruzione 4 legge da P45.Nello stesso identico ciclo di clock, dentro la pipeline del core fisico coesistono due registri reali distinti:
   - P12: contiene il vecchio valore di RAX per l'operazione add.
   - P45: contiene il nuovo valore di RAX per l'operazione imul.
Solo quando le istruzioni arrivano in fondo alla pipeline (Retirement nel Reorder Buffer - ROB), la modifica diventa definitiva a livello architetturale e il vecchio registro fisico P12 viene rimesso nella lista dei registri liberi (Free List) pronti per essere riassegnati.

Quando un processo è in esecuzione attiva sulla CPU, i suoi valori non si trovano nella RAM dentro il task_struct, ma vivono esclusivamente nei circuiti elettrici dei registri hardware del processore. Aggiornare la RAM a ogni ciclo di clock per riflettere le modifiche ai registri distruggerebbe le prestazioni della macchina.
Il task_struct contiene i registri nel senso di una fotografia statica (snapshot) scattata e salvata in memoria solo quando il processo viene interrotto o sospeso. Il salvataggio si articola in due passaggi distinti tra hardware e software:
Processo in User Space (Ring 3)
           |
           | 1. Syscall o Timer Interrupt (preemption)
           v
Transizione Hardware a Kernel Space (Ring 0)
   - La CPU passa allo Stack Kernel del processo.
   - L'hardware pusha automaticamente: SS, RSP, RFLAGS, CS, RIP.
           |
           | 2. Routine di ingresso del Kernel (es. entry_SYSCALL_64)
           v
Salvataggio sullo Stack Kernel: struct pt_regs
   - Il kernel esegue una sequenza di push: RAX, RBX, RCX, RDX, RSI, RDI, RBP, R8-R15.
   - Ora TUTTI i registri dello user-space sono congelati in RAM.
           |
           | 3. Lo Scheduler decide il Context Switch (schedule() -> __switch_to())
           v
Salvataggio nel task_struct: struct thread_struct
   - Il kernel salva lo stato interno del kernel (callee-saved registers, RSP del kernel,
     FS_BASE/GS_BASE per il TLS, e i registri multimediali/FPU tramite istruzione XSAVE).
Quando il processo è addormentato o in coda di attesa:
- I registri generali che stavi usando in C/Assembly (rax, rsp, rip, ecc.) si trovano memorizzati nei byte della struttura pt_regs allocata sul suo stack kernel.
- I puntatori e i registri di controllo per la ripresa del thread si trovano nel campo thread (struct thread_struct) dentro il suo task_struct.
Al momento del risveglio, il percorso si inverte: il kernel ricarica i valori dalla RAM tramite istruzioni pop, ricarica l'Instruction Pointer e lo stack pointer con sysretq o iretq, e la CPU riprende l'esecuzione senza che il processo si sia accorto dell'interruzione.

Un registro come RAX e' dunque una pura interfaccia logica,un nome standardizzato dall'architettura ISA x86-64. Nelle istruzioni binarie della CPU è codificato come un semplice indice numerico a 3 o 4 bit (il registro 0000_b).Ma ad es. P12 e P45 sono le vere celle di memoria fisica ad altissima velocità 
situate nel PRF (Physical Register File) all'interno del core. RAX è l'etichetta astratta che il codice software referenzia. La RAT (Register Alias Table) funziona come una tabella di routing interna: Nome Architetturale (RAX) -----> RAT ------> ID Registro Fisico (es. P12 o P45).

Rivediamo cosa accade internamente con due istruzioni contigue:
mov rax, 10    ; Istruzione A
mov rax, 20    ; Istruzione B
1) Stadio di Rename per l'Istruzione A:
   il decoder vede una scrittura su RAX. La logica di allocazione estrae un registro fisico libero dalla Free List (ad esempio P12).La RAT viene aggiornata: RAX ----> P12. 
   L'unità di esecuzione scriverà il valore 10 dentro la cella fisica P12.
2) Stadio di Rename per l'Istruzione B. Il decoder incontra un'altra scrittura su RAX. Invece di attendere che l'Istruzione A termini, assegna un nuovo registro libero dal pool: P45.
   La RAT viene sovrascritta: RAX -----> P45. L'unità di esecuzione scriverà il valore 20 dentro P45.
3) La coesistenza temporanea: finché l'Istruzione A non ha terminato la sua intera trafila nella pipeline (fase di Retirement nel Reorder Buffer), sia P12 che P45 esistono contemporaneamente nel silicio. Qualsiasi istruzione successiva che richiede RAX 
   leggerà direttamente da P45, mentre istruzioni precedenti ancora in volo leggono in sicurezza da P12.Solo quando l'istruzione B viene dichiarata completata e irreversibile (retired), l'hardware capisce che il contenuto di P12 non servirà mai più a 
   nessuno e rimette P12 nella lista dei registri fisici riutilizzabili.


-----------DISTINZIONE TRA TRANSIZIONE DI PRIVILEGIO(USER -> KERNEL) E CONTEXT SWITCH(TASK A -> TASK B)---------------------------

Ogni volta che si verifica una transizione dall'anello utente (Ring 3) al kernel (Ring 0) — che si tratti di una syscall (write, fork, execve), di un interrupt hardware (la tastiera, la scheda di rete) o del timer della CPU,avviene questo:
1) Salvataggio automatico hardware: la CPU passa istantaneamente dallo Stack Utente allo Stack Kernel dedicato a quel singolo processo e vi deposita automaticamente i 5 registri critici: SS, RSP, RFLAGS, CS e RIP.
2) Salvataggio software via Kernel: La routine di ingresso del kernel (entry_SYSCALL_64) esegue una sequenza di istruzioni push per riversare tutti i registri general-purpose (RAX, RBX, RCX, ecc.) dentro la struttura struct pt_regs allocata sullo stack kernel.
In questa fase i valori dei registri sono congelati in RAM nello stack kernel. Se il kernel deve solo servire una syscall veloce (es. getpid()), legge i dati, scrive il risultato nel campo regs->ax in memoria, riesegue le pop per ricaricare i registri fisici ed esegue sysretq. 
Il processo non ha mai subito un context switch: è rimasto sempre lo stesso task.

Per ogni processo esistono due stack completamente separate ed indipendenti.
| Caratteristica |             Stack Utente (User Stack)                                 | Stack Kernel (Kernel Stack) |

| Privilegio CPU | Ring 3 (User Space)                                                   | Ring 0 (Kernel Space) |
| Dimensione     | Dinamica (cresce su richiesta, tipicamente fino a 8 MB via ulimit -s) | Fissa e ridottissima: 16 KiB (4 pagine da 4 KiB) su architetture x86-64 moderne |
| Collocazione   | Nello spazio virtuale utente (indirizzi alti dello spazio user, cresce verso il basso) | Mappata nello spazio di memoria riservato al kernel, legata al task_struct |
| Contenuto      | Variabili locali del codice C/asm, frame delle funzioni utente, argomenti di funzioni | Copia dei registri utente (`struct pt_regs`), frame delle funzioni interne del kernel |
| Ciclo di vita  | Usata continuamente durante la normale esecuzione del programma       | Usata soltanto quando la CPU è in Ring 0 (durante syscall, interrupt o fault) |

Perché non si può usare una sola stack?
Separare le stack non è una scelta di stile, ma un vincolo hardware di sicurezza e affidabilità del sistema:

1) Sicurezza e Isolamento dei Privilegi:
   Se il kernel usasse la stack dell'utente per eseguire le proprie funzioni, lo spazio utente potrebbe leggere i dati sensibili del kernel rimasti in memoria o, peggio, un thread malevolo potrebbe modificare i frame di ritorno del kernel mentre la syscall è in esecuzione 
   (attacco di tipo TOCTOU / Time-of-Check to Time-of-Use o stack pivoting).
2) Resilienza ai Crash (Immunità da Stack Overflow Utente):
   Se un programma va in ricorsione infinita in user-space, RSP esaurisce la memoria o finisce per puntare a un indirizzo non mappato. Se la CPU tentasse di gestire l'eccezione o la syscall pushando i dati sullo stesso stack corrotto dell'utente, 
   genererebbe un Double Fault immediato seguito da un crash irreversibile dell'hardware (Triple Fault / CPU Reset). La stack kernel garantisce sempre al sistema operativo un'area di memoria vergine, allineata e sicuramente valida su cui operare.

Come fa la CPU a sapere quale stack kernel usare?
L'utente non ha alcun controllo sullo stack kernel. È l'hardware stesso a gestire la commutazione.
Quando lo scheduler esegue un context switch da un processo A a un processo B, non solo cambia la tabella delle pagine tramite CR3, ma aggiorna anche il puntatore dello stack kernel del nuovo processo all'interno del TSS(Task State Segment, campo RSP0) della CPU.
In questo modo, qualsiasi futura syscall o interruzione hardware che colpirà il core utilizzerà automaticamente lo stack kernel corretto associato al processo B.

Se la chiamata era bloccante (come waitpid() con opzione 0, o una lettura da disco) oppure se il timer di sistema segnala che il quanto di tempo è scaduto (preemption), interviene lo scheduler:
1) Il kernel decide di sospendere il Task A per far girare il Task B.
2) Viene invocata la funzione __switch_to(): il kernel salva lo stato interno del kernel del Task A (i registri callee-saved, il puntatore allo stack kernel RSP) nel suo task_struct.
3) Scambio delle tabelle di memoria: Il kernel carica nel registro di controllo CR3 l'indirizzo della Page Table del Task B. La MMU ora mappa l'universo virtuale di B.
4) Scambio dello stack: Il registro RSP della CPU viene fatto puntare allo stack kernel del Task B.
5) Da questo momento la CPU sta eseguendo il Task B.

Quando lo scheduler decide di riattivare il processo dormiente (che passa dallo stato TASK_INTERRUPTIBLE a TASK_RUNNING sulla CPU):
1) Lo scheduler ricarica lo stack pointer del processo e i registri kernel.
2) La routine di uscita del kernel (syscall_return_via_sysret o iretq) preleva dalla RAM dello stack kernel, tramite istruzioni pop, i valori salvati in pt_regs.
3) I dati tornano fisicamente dentro le celle del Physical Register File della CPU.
4) L'istruzione sysretq ripristina contemporaneamente RIP e RSP originali e declassa i privilegi della CPU a Ring 3. Il processo riprende l'esecuzione dal ciclo di clock esatto in cui era stato interrotto, senza alcuna consapevolezza di essere stato parcheggiato in RAM.

Nel caso specifico di execve, la dinamica del ritorno subisce una deviazione radicale:
Processo chiama execve
           |
           Salva registri vecchi in pt_regs sullo Stack Kernel
Kernel distrugge il vecchio spazio virtuale e mappa il nuovo ELF
           |
           INVECE di lasciare intatto pt_regs...
Il Kernel SOVRASCRIVE pt_regs in RAM con valori vergini:
   - regs->ip  = entry point del nuovo binario (_start)
   - regs->sp  = cima del nuovo stack utente (contenente argc, argv, envp)
   - regs->ax  = 0
   - regs->bx, cx, dx, ... = 0 (azzerati per sicurezza e privacy dei dati)
           |
           Ritorno standard a Ring 3 via sysretq / iretq
La CPU preleva i valori da pt_regs ricaricando i registri fisici
   -> RIP punta a _start: il nuovo programma comincia da zero
execve entra in kernel mode salvando i vecchi registri come qualsiasi altra chiamata, ma prima di tornare a Ring 3 cancella quello snapshot in RAM e ne fabbrica uno nuovo. Quando la routine di ritorno ricarica i registri fisici dalla memoria, 
la CPU non torna al vecchio codice, ma si ritrova proiettata all'inizio del nuovo programma.

--------VARIABILI STATICHE IN ASSEMBLY-------------------------------------------------------
In Assembly esistono solo bytes,non tipi primitivi. Una variabile statica equivale a riservare una locazione di memoria fissa che non risiede nello stack (quindi preserva il suo valore tra le chiamate di funzione) e non viene esportata tramite
la direttiva global (quindi rimane privata per quel file sorgente). Usare la direttiva global equivarrebbe infatti a renderla una variabile globale di C.
In base all'inizializzazione, si dichiara nella sezione .data(se inizializzata ad un valore diverso da 0) oppure .bss(non inizializzata o inizializzata a 0).
A differenza delle variabili locali sullo stack ([rsp + offset]), le variabili statiche si leggono e scrivono usando l'indirizzamento RIP-relative ([rel nome_variabile]

----------DIFF----------------------------------------------------------------------------------
Il flag -U sta per Unified Format (formato unificato), lo standard de facto utilizzato per la lettura dei delta e per generare le patch (è lo stesso motore di output usato sotto il cofano da git diff). Rispetto al diff classico che usa < e >, il 
formato unificato mostra le righe rimosse precedute da - e quelle aggiunte precedute da +. Il numero affianco alla flag indica le righe di contesto (context lines).
Normalmente, il formato unificato (spesso invocato con la flag breve -u, che equivale a -U 3) stampa 3 righe di codice intatto prima e dopo la modifica. Questo serve a orientare l'occhio umano fornendo le coordinate logiche del blocco alterato.
Forzando il parametro a 0, impongo a diff di comportarsi in modo chirurgico: sopprime totalmente il codice circostante. L'output si riduce al puro delta.

------QUINE IN PYTHON-----------------------------------------------------------------------------
Il passaggio da C/Assembly a Python sposta l'asse della difficoltà: scompare la gestione manuale della memoria e dei file descriptors, ma emergono nuove insidie legate a come l'interprete parsa i dati.
Per non violare la regola del no cheat (niente open(__file__), niente sys.modules), bisogna rimanere conformi al il Teorema di Kleene (P = A + D). In Python, per implementare questa struttura in modo chirurgico,servono tre nozioni architetturali.

1) %r (repr())
   Python possiede nativamente il concetto di Rappresentazione Ufficiale dell'Oggetto, accessibile tramite la funzione builtin repr() o lo specificatore di formato %r che la richiama.
   Se passo una stringa a %r, Python non si limita a incollare i caratteri (come farebbe %s), ma la avvolge automaticamente nelle virgolette (singole o doppie) e fa l'escape automatico di tutti i ritorni a capo (\n diventa il testo letterale \ e n).
   Questo disintegra la complessità dello stringone in un attimo. Il quine minimo assoluto in Python si scrive così:

   s = 's = %r\nprint(s %% s)'
   print(s % s)

   Invece di printf(s, 10, 34, s), uso la string interpolation vecchio stile col %. %r prende la variabile passata e le inietta virgolette e escape. %% fa l'escape del carattere percentuale (esattamente come in printf), ed e' l'unico escape necessario per l'uso 
   dell'interprete.
   L'uso delle f-string e' sconsigliato,mi costringerebbe a raddoppiare ogni singola parentesi graffa in tutto il blocco di codice per farne l'escape, trasformando il file in spaghetti code illeggibile.

2) Adattamento della Macro
   Il subject recita: "In case of a language without define/macro, you will naturally have to adapt the program accordingly."  Python è interpretato a runtime e non ha una fase di preprocessing. Per tradurre l'architettura di una macro rispettando l'intento del subject, 
   esistono due pattern Pythonici equivalenti:
   
   - Global Lambda: una funzione anonima dichiarata a livello di modulo (fuori da qualsiasi scope interno) che viene invocata alla fine. Equivalente funzionale del richiamare la macro alla fine del file.
   ESEMPIO:
   # Al posto di #define WRITE(...)
   MACRO_WRITE = lambda fd, payload: fd.write(payload)

   - stringa costante globale + execution block: definisco tutto nello scope globale in uppercase (convenzione Python per le costanti), e lo passo a una funzione che esegue l'I/O.

3) File I/O e Lifecycle Management
   Sully in C compila con gcc, esegue con execve (o system), e muore.
   In Python la fase di compilazione non esiste, ma il processo di clonazione deve essere riprodotto fedelmente.

   - I/O pulito: uso sempre i context manager (with open(...) as f:). Oltre a chiudere automaticamente il file (evitando resource leak dei descrittori file in un processo ricorsivo), mi risparmia righe di codice che andrebbero a ingrassare il payload D.
   - esecuzione del child: al posto di chiamare un binario ./Sully_X, devo invocare l'interprete passando il nuovo script.
   In Python, usare brutalmente os.system("python3 Sully_X.py") è sconsigliato (potrebbero esserci alias o virtual environment che deviano l'eseguibile).Il miglior approccio usa sys.executable, che contiene il path assoluto esatto del binario Python che sta eseguendo il padre in quel momento:
   ESEMPIO: 
   import os
   import sys

   # ... generazione file ...
   os.system(f"{sys.executable} {new_file_name}")

   - interpolazione multipla: dato che Sully richiede la mutazione del contatore X, la stringa di formato non avrà solo %r, ma anche %d per iniettare l'intero, seguendo la stessa logica che ho applicato con dprintf in Assembly e C.

*/