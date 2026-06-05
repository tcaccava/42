#include <stdio.h>
// Il carry lookahead è il meccanismo hardware che rende i moderni adder veloci invece di usare il ripple carry .
// Il calcolo del carry (riporto) è il vero collo di bottiglia dell'addizione binaria. In un addizionatore classico (Ripple Carry Adder),
// l'i-esimo bit non può calcolare la sua somma finché il bit precedente non gli ha "passato" il suo riporto. Questo crea una dipendenza lineare O(n)
// che rallenta linearmente le prestazioni all'aumentare della dimensione del registro (32 o 64 bit).
// Il Carry-Lookahead risolve questa dipendenza trasformando il calcolo sequenziale dei riporti in un calcolo parallelo combinatorio.
// Il principio si basa sulla separazione della logica di addizione in due segnali indipendenti per ogni coppia di bit:
// -Generate (G_i): un bit di riporto viene generato autonomamente se entrambi i bit addendi nella stessa posizione sono attivi.G_i = a_i & b_i
// -Propagate (P_i): un bit di riporto proveniente dallo stadio precedente viene propagato allo stadio successivo se almeno uno dei due bit addendi è attivo. P_i = a_i | b_i
// Grazie a queste due definizioni, il riporto dello stadio successivo (C_i+1) può essere espresso senza attendere l'effettiva propagazione fisica del bit,
// ma espandendo ricorsivamente la formula booleana: C_i+1 = G_i | (P_i & C_i).
// Srotolando questa ricorrenza, il riporto di qualsiasi posizione dipende esclusivamente dagli input iniziali a e b e dal riporto iniziale C_0.
// Applicando questa cascata di operatori AND e OR bitwise su tutto il registro, la CPU ricava la maschera intera di tutti i riporti simultaneamente, trasformando il problema da temporale
// (aspettare il bit) a spaziale (espansione delle porte logiche).

// Isolare la maschera dei riporti prima o senza eseguire l'addizione completa ha risvolti critici sia nello sviluppo software a basso livello che nella crittografia:
// -Algoritmi di somma in Emulazione/Virtualizzazione: nello sviluppo di emulatori di CPU (JIT compilers o hypervisor), calcolare il vettore dei carry in anticipo permette di settare i flag
// di stato hardware (come il Carry Flag o l' Auxiliary Carry Flag dell'architettura x86) in modo parallelo e branchless, senza dover testare le condizioni bit per bit.
// -Crittografia a precisione arbitraria (Bignum Arithmetic): gli algoritmi come RSA o la crittografia a curve ellittiche (ECC) elaborano interi enormi (256, 512 o 2048 bit).
// Poiché le ALU hardware si fermano a 64 bit, i vettori di carry lookahead vengono estratti via software per concatenare macro-blocchi di addizioni ad
// altissima velocità senza rompere la pipeline d'esecuzione.
// -Algoritmi SWAR(SIMD Within A Register) : consente di simulare l 'addizione parallela di sub-vettori (ad esempio, quattro interi da 8 bit stipati dentro un unico
// registro da 32 bit) impedendo ai riporti di un sub-intero di "sconfinare" e corrompere l' intero adiacente, mascherando i bit di carry generati nei punti di giunzione.

// A livello di silicio, l'architettura del Carry-Lookahead rappresenta uno dei compromessi geometrici più famosi dell'elettronica digitale.
// Riduzione drastica del Gate Delay: in un addizionatore Ripple Carry a 32 bit, il segnale deve attraversare circa 64 porte logiche prima che l'ultimo bit sia stabile.
// In un modulo Carry-Lookahead (CLG), la profondità del circuito è teoricamente fissa: indipendentemente dal numero di bit, il riporto viene calcolato attraversando
// solo 3 stadi logici (uno stadio per generare P e G, uno strato di porte AND in parallelo per le intersezioni, e uno strato finale di macro-porte OR).
// Il problema del Fan-In e del Routing Congestion: espandere la formula pura fino a 32 o 64 bit richiederebbe porte logiche con un numero di ingressi (Fan-In)
// impraticabile per il silicio (una porta OR a 32 ingressi soffrirebbe di un ritardo di propagazione elettrica intrinseco enorme a causa della capacità parassita dei transistor).
// Inoltre, la quantità di fili fisici per collegare ogni bit a tutti i precedenti creerebbe un intasamento dei canali metallici (Routing Congestion).
// Soluzione Hardware Gerarchica (Block-CLA): per ovviare a questo limite, i chip reali non usano un unico blocco lookahead da 32 bit.
// Il silicio viene diviso in gruppi (solitamente blocchi da 4 bit). Ogni blocco genera a sua volta un segnale di "Super-Generate" e "Super-Propagate".
// Questi segnali vengono poi processati da un secondo livello di circuiti lookahead (chiamato Lookahead Carry Generator di secondo livello).
// Questa struttura ad albero (implementata in varianti celebri di Parallel Prefix Adders come i commutatori di Kogge-Stone o Brent-Kung) mantiene la latenza logaritmica O(log n),
// garantendo frequenze di clock superiori ai 4-5 GHz nelle moderne ALU.
unsigned int carry_lookahead(unsigned int a, unsigned int b)
{
    unsigned int generate = a & b;           // AND tra i due numeri da 1 dove entrambi sono 1,quindi l'and contiene le posizioni dei generatori di riporto
    unsigned int propagate = a | b;          // OR contiene 1 se almeno uno dei due bit e' 1,quindi contiene informazioni su dove c'e' un potenziale propagatore di carry proveniente da un bit precedente
    generate |= (generate << 1) & propagate; // shifto generate a sinistra di 1,ne faccio & con P, e salvo il risultato in G,che conterra' informazioni sui bit che generavano carry sovrapposti a quelli che lo possono propagare,e che quindi lo generano essi stessi
    propagate &= propagate << 1;             // aggiorniamo propagate con se stesso shiftato di uno,cosi' che ogni bit di P adesso sappia se una coppia di bit consecutivi puo' propagare carry
    // Ripetiamo ad intervalli crescenti di 2,4,8,e 16 bit
    generate |= (generate << 2) & propagate;
    propagate &= propagate << 2;
    generate |= (generate << 4) & propagate;
    propagate &= propagate << 4;
    generate |= (generate << 8) & propagate;
    propagate &= propagate << 8;
    generate |= (generate << 16) & propagate; // a questo punto Generate contiene la mappa dei carry accumulati,ma si trova nella stessa posizione in cui sono stati generati
    // dal momento che a noi serve la maschera dei carry in ingresso al bit successivo (il riporto del bit 0 deve entrare nel bit 1),l'ultimo atto e' uno shift a sinistra
    generate <<= 1;
    return a ^ b ^ generate; //  generate e' la bitmap di tutti i riporti, lo XOR e' la somma senza riporti,quindi uno xor tra i numeri primitivi e la maschera dei carry genera la somma tal quale
}

int main()
{
    printf("%d\n", carry_lookahead(1, 3));
}