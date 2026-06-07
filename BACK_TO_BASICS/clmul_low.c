#include <stdio.h>

/*
La Carryless Multiplication (moltiplicazione senza riporto) non fa altro che prendere la moltiplicazione e "privarla" della parte più fastidiosa per un programmatore low-level: il riporto.
L’Algebra dei Polinomi su GF(2): la moltiplicazione classica tra interi si basa sull'aritmetica in base 10 (o base 2 nei computer) dove i calcoli si propagano da destra a sinistra tramite i
riporti (1 + 1 = 0 con riporto di 1).
La carryless multiplication opera invece nel campo finito di Galois chiamato GF(2) (Galois Field a 2 elementi: 0 e 1). In questo universo matematico, i numeri interi non rappresentano quantità,
ma i coefficienti di un polinomio: un intero a 32 bit come 13 (che in binario è 1101) non viene visto come 13, ma come il polinomio: A(x) = 1 * x^3 + 1 * x^2 + 1 * x^1 + 1 * x^0 = x^3 + x^2 + 1
Quando moltiplichi due polinomi in questo campo, le regole cambiano radicalmente:
-La moltiplicazione dei coefficienti è una congiunzione logica: 1 * 1 = 1, tutto il resto fa 0. Questo corrisponde esattamente all'operatore bitwise AND (&).
-L'addizione dei coefficienti avviene senza riporto: 1 + 1 = 0. Questo corrisponde esattamente all'operatore bitwise XOR (^).
Di conseguenza, la moltiplicazione carryless è l'operazione che calcola il prodotto di due polinomi in GF(2). Ogni bit del risultato finale dipende esclusivamente dall'intersezione dei bit dei
fattori, isolando completamente ogni colonna dalle altre.

Esempio Pratico:prendiamo due numeri a 4 bit per visualizzare lo schema logico.
a = 1101_2 (Decimale 13 --> x^3 + x^2 + 1)
b = 1011_2 (Decimale 11 --> x^3 + x + 1)
Facciamo la moltiplicazione in colonna classica, ma applicando lo XOR invece dell'addizione quando sommiamo le righe parziali.

      1 1 0 1   (a)
    x 1 0 1 1   (b)
    ---------
      1 1 0 1   (a shiftato di 0, perché il bit 0 di b è 1)
    1 1 0 1 .   (a shiftato di 1, perché il bit 1 di b è 1)
  0 0 0 0 . .   (a shiftato di 2, perché il bit 2 di b è 0)
1 1 0 1 . . .   (a shiftato di 3, perché il bit 3 di b è 1)
---------

Ora sommiamo verticalmente colonna per colonna usando lo XOR (se il numero di 1 nella colonna è dispari il risultato è 1, se è pari è 0):

Colonna 0 (LSB):1 --> 1
Colonna 1: 0 ^ 1  --> 1
Colonna 2: 1 ^ 0 ^ 0 --> 1
Colonna 3: 1 ^ 1 ^ 0 ^ 1 --> 1 (tre 1 fanno 1, nessun riporto va alla colonna 4)
Colonna 4: 1 ^ 0 ^ 0 --> 1
Colonna 5: 0 ^ 1 --> 1
Colonna 6 (MSB): 1 --> 1

Il risultato finale è 1111111 in binario (Decimale 127).
Se avessi fatto una moltiplicazione normale, 13 * 11 avrebbe fatto 143 (10001111). La totale assenza di riporti ha cambiato completamente l'output.

Applicazioni pratiche: senza la moltiplicazione carryless, internet non potrebbe essere sicuro o veloce come lo è oggi. Questa operazione apparentemente bizzarra è il motore di tre pilastri dell'informatica moderna:
-AES-GCM (Cifratura e autenticazione): e' il protocollo crittografico che protegge quasi tutto il traffico HTTPS mondiale. GCM utilizza un sistema di autenticazione chiamato GMAC, che richiede la valutazione di un polinomio ad altissima velocità su un campo a 128 bit (GF(2^128)).
Svolgere questa operazione con algoritmi classici richiederebbe cicli d'orologio immensi o tabelle di lookup gigantesche esposte ad attacchi di temporizzazione (cache-timing attacks).
Quando invii dati protetti da AES-GCM (tipo quando navighi su un sito HTTPS), non vuoi solo che i dati siano segreti, vuoi anche essere sicuro che un hacker non alteri i bit cammino facendo. Per farlo, serve un "Tag di autenticazione" (una firma del messaggio).
Per calcolare questa firma, il protocollo prende il testo cifrato e lo moltiplica per una chiave d'autenticazione segreta.
Se usassi la moltiplicazione normale: i riporti creerebbero una dipendenza sequenziale (il bit 3 dipende dal bit 2, che dipende dal bit 1...). Questo rende il calcolo asimmetrico e vulnerabile a attacchi basati sul tempo (timing attacks), in cui un hacker capisce la chiave misurando
i nanosecondi impiegati dai riporti a propagarsi nel silicio.
Con la moltiplicazione carryless ogni colonna di bit è indipendente. Il calcolo richiede lo stesso identico tempo indipendentemente dai dati (tempo costante). In più, l'assenza di riporti fa sì che se un hacker cambia anche un solo bit del messaggio cifrato, il risultato della moltiplicazione
si distrugge in modo totalmente imprevedibile e geometrico, rendendo il tentativo di frode immediatamente visibile.
-CRC (Cyclic Redundancy Check): i controlli di integrità dei dati (come il CRC32 usato nei pacchetti Ethernet, nei file ZIP o nei PNG) non sono altro che il resto di una divisione tra polinomi in GF(2). Per accelerare questo calcolo su
blocchi di dati enormi, si usa la moltiplicazione carryless combinata con la riduzione di Barrett( invece di dividere un numero X per un divisore D, moltiplichiamo X per l'inverso di D (precalcolato) e poi shiftiamo a destra).
Nel CRC facciamo la stessa identica cosa, ma interamente dentro il mondo senza riporti (GF(2)).Il Divisore (D): e' il polinomio standard del CRC (ad esempio, il CRC32 usa un polinomio fisso a 32 bit).L'Inverso (I): non è una frazione con la virgola.
È un ammasso di bit calcolato a tavolino una volta per tutte, risolvendo l'equazione polinomiale in modo che, se moltiplicato per D (senza riporti!), dia un pattern di bit pulito. La moltiplicazione: quando arriva il tuo flusso di dati (X), la CPU esegue la moltiplicazione carryless tra i dati e
questo inverso magico (I). Il risultato di questa moltiplicazione carryless ti dà il quoziente della divisione polinomiale.
Calcolare il CRC32 di un file per assicurarsiche non sia corrotto: il CRC non è altro che il resto di una divisione matematica tra polinomi.
Se provassi a fare una divisione normale nel codice, la CPU dovrebbe fare un ciclo lentissimo bit dopo bit. Ma l'algebra ci dice che dividere per un numero equivale a moltiplicare per il suo inverso.
Grazie alla moltiplicazione carryless, la CPU può prendere i blocchi del tuo file, moltiplicarli per una costante magica (l'inverso del polinomio del CRC) e sputare fuori il checksum istantaneamente, processando 64 o 128 bit alla volta.
-Codici di correzione errore (BCH e Reed-Solomon): usati nei QR code, nelle trasmissioni satellitari e nei vecchi CD/DVD per ricostruire i dati corrotti o graffiati. Operano interamente sulla matematica dei campi finiti.

Implementazione Hardware: per una CPU o una FPGA, implementare un moltiplicatore aritmetico classico è un incubo ingegneristico. Richiede alberi di addizionatori complessi (come i Wallace Trees) e circuiti dedicati alla propagazione ultra-rapida del riporto (Carry-Lookahead),
che consumano silicio, generano calore e aumentano il cammino critico (il tempo che il segnale impiega a stabilizzarsi).
La carryless multiplication a livello hardware è, al contrario, di una semplicità disarmante:
-Una griglia di porte logiche AND calcola istantaneamente tutti i prodotti parziali (ogni bit di a con ogni bit di b).
-Un albero di porte logiche XOR (chiamato albero di parità) unisce le righe.
Non essendoci alcun riporto, non esiste propagazione orizzontale del segnale. Ogni colonna è indipendente. Questo significa che l'hardware può calcolare il risultato con un ritardo di propagazione bassissimo e frequenze di clock elevatissime.
Nelle architetture moderne (x86_64), Intel e AMD hanno introdotto l'istruzione dedicata PCLMULQDQ (all'interno delle estensioni SSE5/AVX). Questa istruzione prende due registri a 128 bit, ti permette di scegliere tramite
un flag quali metà a 64 bit moltiplicare, e sputa fuori il risultato carryless a 128 bit in pochissimi cicli di clock, garantendo un throughput spaventoso per la cifratura dei dati.
*/
unsigned int clmul_low(unsigned int a, unsigned int b)
{
  unsigned int res = 0;
  for (int i = 0; i <= 31; i++)
    res ^= (a << i) & -(b >> i & 1);
  return res;
}

int main()
{
  printf("%u\n", clmul_low(13, 11)); // 127
}