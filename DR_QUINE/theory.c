/*
---EQUAZIONI E FUNZIONI-----------------------------------------------------------------------------------
Un' equazione descrive una condizione,cioe' una regola, di uguaglianza tra due espressioni algebriche, per la quale possono esistere specifiche soluzioni che realizzano quella eguaglianza.E' quindi una condizione statica che non fa letteralmente nulla.
Una funzione è un operatore meccanico,una procedura algoritmica reale, un pezzo di codice o una trasformazione matematica che prende un input(dominio) e ,obbedendo a quella regola, restituisce un output(codominio) che corrisponde alle soluzioni di quella equazione.
Una funzione parziale calcolabile e' una funzione matematica f che può essere fisicamente calcolata da una Macchina di Turing in un tempo finito. Si dice "parziale" perché non è garantito che sia definita per ogni input possibile nel suo dominio.
Se per un dato input la Macchina di Turing entra in un loop infinito,quindi non termina,o crasha(Undefined Behavior,Segfault,divisione per zero), la funzione per quell'input è semplicemente non definita. 
Se invece la macchina termina sempre e restituisce sempre un risultato per ogni input del suo dominio, la funzione diventa totale. 

---TEOREMA DELLA FERMATA------------------------------------------------------------------------------------
Il teorema della fermata e' un limite insuperabile della logica dimostrato nel 1936: afferma che è matematicamente impossibile scrivere un programma che prenda in input il codice sorgente di un altro programma e decida, nel 100% dei casi,
se quel codice eseguito terminera' o meno. Posso scriverlo per casi specifici, ma l'analizzatore statico universale che funziona per ogni programma possibile è matematicamente impossibile da creare. Non e' possibile per esempio scrivere un parser 
in C che garantisca a priori l'assenza di loop. Se fosse possibile, si creerebbero paradossi logici distruttivi (un programma che entra in loop solo se l'analizzatore dice che terminerà). Per questo il compilatore non può salvare alcun programmatore 
da un while(1) o da una ricorsione difettosa. Un linguaggio come C,ma vale per qualunque altro linguaggio Turing completo, mappa la classe delle funzioni parziali perche' i suoi costrutti consentono di fare cose come un ciclo while true infinito senza 
uscita o andare in segfault/ UB,ma questo non significa che non ci si possa scrivere anche una funzione completa.
Ogni funzione totale in un linguaggio Turing completo è matematicamente solo un caso specifico (un sottoinsieme) di una funzione parziale che per puro caso "è definita ovunque".

---MACCHINA E LINGUAGGIO DI TURING-----------------------------------------------------------------------------------------
Una macchina di Turing e' sostanzialmente il modello matematico assoluto di un computer,cioe' la definizione matematica di una macchina capace di computare. E' composto da almeno tre componenti fisici (teorici):
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

---PUNTO FISSO DI UNA FUNZIONE E QUINE------------------------------------------------------------------------------
Nel lambda calcolo e nella teoria della computazione, un punto fisso di una funzione è un valore che viene mappato su se stesso dalla funzione ,ovvero (f(x) = x). In sostanza un punto fisso x di una funzione f è semplicemente un valore tale per cui l'esecuzione 
della funzione su quel valore restituisce il valore stesso. Esempio : se f(x) = x^2, i punti fissi sono 0 (0^2 = 0) e 1 (1^2 = 1). Se F è l'operazione di invertire una stringa, le stringhe palindrome (es. "radar") sono i punti fissi. 
Un Quine è letteralmente il punto fisso di un ambiente di compilazione ed esecuzione,cioe' un costrutto informatico il cui output coincide esattamente con il proprio codice sorgente . Esempio : una funzione E(s) che prende una stringa s (il file .c), la compila, 
la esegue e raccoglie l'output sullo standard output. Normalmente, se metto dentro s un "print(2+2)",la funzione eseguita mi restituisce 4 : E(s) = "4".Se s è un Quine, avviene questo: E(s) = s. La stringa sorgente in ingresso sopravvive intonsa al processo di compilazione
ed esecuzione, ritornando se stessa.
Se prendiamo un compilatore o un interprete, possiamo vederlo come una funzione E (Execution environment) che prende in input un codice sorgente S e produce in output un risultato R: E(S) = R.
Un Quine non è altro che un codice sorgente Q il cui output è esattamente sé stesso. Quindi E(Q) = Q.
Un Quine è letteralmente il punto fisso dell' interprete o compilatore. E il fatto che i quine esistano per qualunque linguaggio di programmazione Turing-completo è garantito al 100% proprio dal Secondo Teorema di Ricorsione.
In realta',per essere piu' precisi,il primo teorema,quando parla di punto fisso non si riferisce ad input o output numerici(quelli sono solo una semplificazione per rendere comprensibili teoremi cosi' complessi),ma alle funzioni stesse. Possiamo quindi immaginare un punto fisso 
come una funzione f che ,passata in input ad un altra funzione F,restituisce se stessa come output. Il parellelo concettuale piu' immediato e' una funzione C che accetta un puntatore a funzione come argomento e ne restituisce un altro,o ancora meglio un decoratore python,cioe' un wrapper che 
prende una funzione e la ritorna modificata, a un livello più macroscopico, un compilatore,cioe' un programma che prende in input un file di testo (il codice sorgente di un altro programma) e sputa fuori un binario. 
In matematica (nello specifico nel Lambda Calcolo, che è la base teorica della computazione), i tipi primitivi non esistono: tutto è una funzione di ordine superiore. 
Perché questo ha a che fare con la ricorsione? Perché quando scrivo una funzione ricorsiva in C (es. f(x) = x * f(x-1)), sto in realta' scrivendo un'equazione in cui l'incognita da trovare è la funzione f. Sto cioe' definendo f usando f stessa. Il compilatore si trova davanti a un costrutto circolare. 
Il Primo Teorema mi salva garantendo che per un simile costrutto circolare esiste sempre almeno una vera funzione matematica che si comporta esattamente così. Quella funzione è il minimo punto fisso.
In logica formale e teoria degli insiemi,non posso scrivere una definizione circolare,cioe' definire un'entità menzionando il suo stesso nome prima che quell'entità sia stata completamente definita. È un errore sintattico e logico insormontabile. Per aggirare questo blocco e dimostrare che la ricorsione 
e' corretta sotto il profilo logico-formale, i matematici devono fare un giro di boa: creano un operatore esterno F (il generatore),dicono che F prende una funzione "grezza" o incompleta g e la espande di un gradino,quindi definiscono la vera funzione ricorsiva f come quel particolare oggetto che rappresenta 
il punto d'arresto dell'operatore, cioè F(f) = f.L'operatore F è il trucco formale necessario per strappare la ricorsione al paradosso logico della circolarità.



---TEOREMI DI RICORSIONE DI KLEEN E NUMERAZIONE DI GODEL----------------------------------------------------------------------------
I due teoremi di ricorsione di Kleen poggiano su un assioma fondamentale: la numerazione di Godel. Qualsiasi macchina di Turing puo' essere codificata nella forma di un intero o di una singola stringa univoca. Questo annulla la distinzione del codice sorgente di un 
programma in codice e dati: un programma puo' manipolare altri programmi trattandoli come numeri. La numerazione di Godel non e' un hash: le funzioni di hash hanno dimensione fissa,generano collisioni e non sono revertibili;non posso ricostruire il file originale 
dall'hash. La numerazione di Gödel è una codifica biunivoca (isomorfismo) senza alcuna perdita di informazione. Per esempio un sorgente .c e' un file di testo, quindi una sequenza di byte ASCII. Se prendo i byte in hex e li concateno, otterro' un singolo,
gigantesco numero intero. Da quel numero posso riottenere l'esatto codice sorgente applicando l'operazione inversa (decodifica). Questo concetto dimostra una cosa fondamentale in informatica teorica: codice e dati sono la stessa identica cosa. Un programma è solo un grosso intero 
che la macchina di Turing interpreta come istruzioni. Quindi basicamente la numerazione di Godel di un programma e' semplicemente la rappresentazione binaria del suo sorgente,convertita in un intero gigantesco a precisione arbitraria, la cui bitness dipende dalla dimensione del sorgente ,e che 
semplicemente contiene tutta l'informazione codificata in forma binaria.

Il primo Teorema (Teorema del Minimo Punto Fisso) riguarda la semantica del codice,quindi la logica matematica a fondamento della informatica teorica che legittima la programmazione ricorsiva: stabilisce che ogni equazione che definisce una funzione in termini di se stessa(ricorsiva) possiede solo un minimo punto fisso,e quest'ultimo e' calcolabile. 
È ciò che garantisce che quando scrivo una funzione ricorsiva in C, matematicamente abbia un senso e calcoli qualcosa di specifico. Sia phi di x la funzione calcolata dal programma con indice (numero di Gödel) x. Il teorema afferma che per ogni funzione totale calcolabile f che fa cio' che fa phi di x, esiste 
un indice di Godel e(un programma) che, durante la sua esecuzione, calcola la stessa identica cosa che calcolerebbe un programma modificato dalla funzione f. Se usiamo una funzione f che semplicemente stampa il suo input, il teorema dimostra matematicamente che esiste un programma capace di 
stampare il proprio codice sorgente (se stesso) senza leggerlo dall'esterno.
Quando scrivo una funzione ricorsiva in C, per es. int f(int n) { return n == 0 ? 1 : n * f(n-1); }, matematicamente sto scrivendo un'equazione in cui f compare sia a destra che a sinistra: f = F(f),ovvero quella funzione f deve soddisfare una regola(un equazione) in cui compare se stessa.  Sto cercando quella funzione f tale che, se la do in pasto all'
operatore di trasformazione F, ti restituisce esattamente se stessa. Questa è la definizione algebrica di punto fisso. Il problema matematico è che un'equazione ricorsiva del genere ha quasi sempre infiniti punti fissi. Esistono cioe' infinite funzioni diverse che, se infilate dentro F, fanno quadrare l'equazione. 
Tra queste ci sono funzioni corrette, ma anche funzioni non corrette.Non corrette rispetto a quale rule set? La "correttezza" rispetto a quale rule set?Il "rule set" è la specifica formale del problema che voglio computare (ad esempio, le proprietà matematiche che definiscono univocamente il fattoriale o la serie di Fibonacci).Se scrivo un'equazione ricorsiva scritta male,
l'algebra formale potrebbe trovare dei punti fissi che soddisfano formalmente l'equazione F(f) = f ma che, rispetto al problema reale che volevo risolvere, sono spazzatura (es. restituiscono valori costanti, o non terminano dove dovrebbero). Tra tutti i punti fissi possibili che fanno quadrare l'equazione algebrica, solo il minimo punto fisso corrisponde esattamente alla semantica pulita 
e corretta del tuo algoritmo, senza effetti collaterali o invenzioni arbitrarie. Come faccio a sapere che quel codice definisce una funzione matematica reale e non un paradosso logico? 
Il Primo Teorema di Kleene dice che ogni equazione di questo tipo ha un solo minimo punto fisso, ovvero esiste una e una sola funzione matematica che è la "più piccola" (quella definita su meno input possibili) a soddisfare l'equazione. Per capire il concetto di "minimo", bisogna comprendere 
come i matematici classificano le funzioni. In questo contesto, una funzione è considerata "più piccola" o parziale di un'altra se è meno definita, ovvero se per un maggior numero di input non restituisce un bel niente . La funzione in assoluto più piccola di tutte è quella che non sa fare nulla: diverge per qualunque input.
Il Primo Teorema di Kleene in sostanza stabilisce quale, tra infinite funzioni possibili, è quella "vera" che descrive un calcolo effettivo,affermando che esiste sempre un "minimo punto fisso" per qualsiasi operatore ricorsivo monotonicamente crescente e che esso è l'unica funzione che fa solo ed esclusivamente ciò che le è richiesto dai casi base,
senza aggiungere comportamenti arbitrari o inventati.
Come si costruisce questo minimo? Il teorema non si limita a dire che esiste, ma spiega come ottenerlo
-Passo 0 : si parte da una funzione teorica  che fallisce (diverge o va in loop) per qualsiasi input le venga passato. Questa è la funzione più piccola possibile nello spazio matematico.
-Passo 1: la funzione vuota viene passata all'operatore F,che calcola il primo livello di logica e restituisce una funzione rudimentale che sa gestire solo il caso base (es. calcola correttamente solo quando l'input è 0).
-Passo 2: la funzione parziale viene passata ad F,che ne espande la logica, ottenendo una funzione che gestisce il caso base e il primo step di ricorsione (es. input 0 e 1).
-Il Limite: ripetendo questo processo all'infinito (o per un numero transfinito di volte), la sequenza di funzioni converge a un limite convergente che è il minimo punto fisso. È la funzione matematica completa e perfetta (ad esempio, la funzione fattoriale esatta) che soddisfa l'equazione ricorsiva senza inventarsi alcun comportamento spurio per gli input non previsti.

Quando scrivi la logica di una ricorsione, stai scrivendo un'equazione.
Esempio: se scrivo int f(int x) { return f(x); }, qual è la funzione matematica che esce fuori? È la funzione "vuota", cioè una funzione parziale che diverge (va in loop) su qualunque input. Quella funzione vuota è il minimo punto fisso di quell'equazione ricorsiva.
In matematica un'equazione deve comunque avere un significato formale. 
Esempio pratico del Primo Teorema:
Mettiamo il caso che scriva un fattoriale in C dimenticandomi il caso base:
int f(int x) { return x * f(x-1); }
Qual è la funzione matematica che soddisfa questa regola? È la funzione che per qualsiasi input crasha o va in loop infinito. Quella funzione (la "funzione ovunque indefinita",cioe' la funzione vuota) è il minimo punto fisso di questa ricorsione rotta.
Mettiamo invece che la abbia scritta correttamente:
int f(int x) { if (x == 0) return 1; else return x * f(x-1); }
Qual è la funzione che soddisfa questa regola? È la funzione matematica del fattoriale. Il fattoriale vero e proprio è il minimo punto fisso di questa definizione.
Perche' e' nata tutta questa costruzione teorica di fronte ad un problema che appare insignificante?
Quando in C scrivo una ricorsione, a livello x86-64 sto solo piazzando un'istruzione call che punta a un indirizzo di memoria (o a un'etichetta nel segmento .text) che si trova due righe più sopra. Alla CPU non frega nulla del paradosso logico: la CPU vede un indirizzo, pusha il Base Pointer e 
l'Instruction Pointer sullo stack, allinea i registri e salta. Se salta all'infinito, sfonda lo stack e si va in segfault. La macchina fisica non si occupa della semantica.
Ma la matematica pura non ha lo stack, i registri o l'Assembly. In logica matematica pura, non posso definire un'entità usando sé stessa. È un paradosso logico, come lo sarebbe definire la parola "cane" scrivendo sul vocabolario "entità che si comporta da cane". È un nonsense che fa collassare le equazioni.
Per dare un senso logico (e matematico) a quella call verso se stessa, i teorici si sono inventati questo escamotage per dimostrare che la ricorsione ha una sua integrita' logico-formale:
1) Immaginiamo un "costruttore" di codice, una funzione di ordine superiore: F_Builder.
2) F_Builder prende in input una funzione fittizia qualsiasi e la "potenzia", iniettandoci dentro la logica operativa di un algoritmo (ad esempio, le regole di un fattoriale).
3) All'inizio, gli passo un input spazzatura: una funzione "vuota" che genera solo undefined behavior. F_Builder elabora questa spazzatura e restituisce una funzione che sa calcolare solo il caso base (es. x = 0).
4) Se prendo questa nuova funzione e la ripasso a F_Builder, ne restituisce una leggermente migliorata, che sa calcolare il caso base e lo step successivo (x = 1).
5) Iterando questo ciclo, la funzione si "costruisce" un passo alla volta. Arriva un punto in cui, se butti la funzione dentro F_Builder, la funzione che esce è identica, byte per byte, logica per logica, a quella che è entrata.
Questo limite di saturazione, dove l'input (la funzione vecchia) equivale all'output (la funzione nuova) senza subire alterazioni, è il punto fisso. Ed è matematicamente l'esatta funzione ricorsiva completa, deterministica e funzionante.
Il Primo Teorema è una colossale pezza teorica: serve "solo" a garantire ai matematici e ai creatori di compilatori che permettere a una funzione di chiamare se stessa non innesca una fallacia circolare che invalida la logica matematica e quindi la computabilita', ma produce sempre un 
comportamento deterministico (che sia un calcolo corretto o un crash inevitabile).

In termini formali il secondo teorema di ricorsione di Kleene afferma che ,per ogni funzione parziale calcolabile f, esiste un indice di Godel p ,cioe' un programma, che eseguito restituisce lo stesso risultato di f(p). Ovvero ,per dirla in modo equivalente, e' sempre possibile scrivere un sorgente che eseguito
dia un output il cui numero di godel e' identico a quello del sorgente stesso.
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
Il Secondo Teorema di ricorsione entra in gioco qui: dimostra matematicamente che esiste sempre una via di fuga a questo loop infinito,garantendo che è possibile separare la logica del programma dai suoi dati (componente attiva e passiva) per aggirare il problema dell'espansione infinita,
che è il principio esatto su cui si basa l'architettura di un Quine. Esiste sempre un modo per un programma di calcolare il proprio numero di Gödel (il proprio codice sorgente) e passarlo come argomento a sé stesso.

Per costruire un Quine valido, la teoria di base (spesso associata al costruttore universale di von Neumann) richiede la scomposizione del sorgente in due entità ontologicamente distinte ma interdipendenti:
1) la componente attiva (codice): le istruzioni operative incaricate di formattare e stampare.
2) la componente passiva (dati): una rappresentazione in memoria (generalmente una stringa o un buffer) che mappa esattamente le istruzioni della componente attiva.
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
-ma in C devi mettere i caratteri di escape per le virgolette interne perche' vengano interpretate come tali: " \"...\" "
-ma ora la stringa deve contenere i caratteri di escape \, quindi devi fare l'escape delle escape: " \\\"...\\\" "
-questo genera una ricorsione logica infinita in fase di scrittura del codice: non posso definire la stringa passiva includendo letteralmente le virgolette o i ritorni a capo (newline) necessari per formattarla, perché per farlo sarei costretto ad aggiungere altri caratteri di escape,
 alterando la stringa che stavo cercando di rappresentare.

Il legame matematico tra il punto fisso e il codice sorgente si riduce a questa identità: Compiler(Source) = Binary,e poi Execution(Binary) = Output. Nel momento in cui scrivo un Quine, cerco un codice sorgente S tale che Execution(Compiler(S)) = S. L'output dell'esecuzione deve essere bit a bit identico al 
sorgente S che è entrato nel compilatore. Questo è il punto fisso: lo spazio dei dati in cui l'operazione di stampa/compilazione non sposta né altera di un byte la struttura sintattica dell'oggetto di partenza.
Il punto centrale del secondo teorema di ricorsione di Kleene applicato al problema del Quine è la dimostrazione formale che un sistema formale puo' possedere la specifica strutturale di se stesso attraverso una relazione di ricorsione incrociata tra l'operatore e il suo operando.
La struttura teorica si regge su tre pilastri concettuali puri:
1) Isomorfismo di dominio: poiché codice e dati condividono lo stesso spazio numerico (la numerazione di Gödel), non esiste alcuna barriera ontologica tra un'istruzione che elabora e un dato che descrive. Una funzione può avere come proprio dominio la stringa di caratteri che la definisce.
2) La dualità funzionale (il doppio strato): per evitare il regresso all'infinito in cui un oggetto che descrive se stesso dovrebbe espandersi all'infinito, la teoria impone la separazione in due entità logiche simmetriche: un operatore di transizione (la logica attiva ) e un template strutturale (la componente passiva).
3) La risoluzione del punto fisso (F(x) = x): il teorema dimostra che esiste sempre una configurazione in cui l'operatore di transizione agisce sul template strutturale producendo in uscita esattamente lo stesso template e la stessa regola che lo ha generato. In termini logici, la struttura collassa
   in un'identità chiusa dove l'input descrittivo e l'output generato coincidono perfettamente.Questo è il motivo per cui la teoria esclude qualsiasi input esterno o lettura da file: l'autoriferimento non è un'operazione di I/O, ma una proprietà topologica dello spazio di computazione, garantita dal fatto che la 
   logica e la stringa descrittiva sono legate da un punto fisso matematico.
*/