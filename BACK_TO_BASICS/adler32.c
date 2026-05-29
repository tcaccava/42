#include <stdio.h>

// Adler 32 è un algoritmo di checksum (somma di controllo) inventato da Mark Adler. È una variante diretta del checksum di Fletcher, progettata specificamente
// per rilevare errori accidentali durante la trasmissione o l'archiviazione dei dati.
// L'algoritmo mantiene due accumulatori a 32 bit, chiamati s_1 e s_2, che vengono aggiornati per ogni byte del messaggio secondo le seguenti regole matematiche:
// 1) inizializzazione: s_1 parte da 1 (una scelta fondamentale per rilevare i byte nulli iniziali) e s_2$  da 0.
// 2) iterazione (per ogni byte D_i del messaggio):
//      s_1 = (s_1 + D_i) \ mod M
//      s_2 = (s_2 + s_1) \ mod M
// 3)risultato finale:
//      i due accumulatori (che non supereranno mai il valore di M) vengono uniti in un unico intero a 32 bit: (s_2 * 65536) + s_1, che in C si scrive come (s2 << 16) | s1.
// La costante M è il fulcro dell'algoritmo: e' il più grande numero primo inferiore a 65536 (2^16): l'uso di un modulo primo garantisce che la distribuzione dei checksum nello spazio
// dei risultati sia il più uniforme possibile, minimizzando le "collisioni" (casi in cui messaggi diversi producono lo stesso checksum).
// Il fatto che sia inferiore a 65536 permette a s_1 e s_2 di risiedere perfettamente all'interno di 16 bit ciascuno prima di essere uniti, occupando esattamente i 32 bit dell'
// output finale.
// Rispetto al classico CRC32 (Cyclic Redundancy Check), che si basa sulla divisione polinomiale bitwise, Adler32 è drammaticamente più veloce da calcolare in software,
// poiché richiede solo addizioni e operazioni di modulo, senza calcoli bit-by-bit o enormi tabelle di lookup precalcolate.
// È pero' meno efficace nel rilevare errori su stringhe molto corte (sotto i 100 byte) perché se i dati sono pochi, l'accumulatore s_2 non ha il tempo di "mescolarsi"
// abbastanza, lasciando lo spazio dei bit più significativi parzialmente inutilizzato.
// Adler-32 ha trovato la sua perfetta collocazione ovunque serva validare gigabyte di dati in tempo reale senza gravare sulla CPU:
// -Zlib / Gzip lo usano come checksum per verificare l'integrità dei flussi di dati compressi con l'algoritmo DEFLATE all'interno del formato di archiviazione .zlib
// -il formato PNG (Portable Network Graphics usa zlib per comprimere i pixel,quindi ogni file PNG che apriamo si affida ad Adler-32 per garantire che l'immagine non sia corrotta
// -protocolli di rete leggeri :utilizzato in sistemi embedded dove la CPU non ha la potenza di calcolo necessaria per gestire algoritmi crittografici complessi (come SHA o MD5) o calcoli polinomiali pesanti (CRC32),
// ma necessita comunque di una sicurezza industriale contro la corruzione dei pacchetti
// Non esistono istruzioni native ed esclusive (come un'ipotetica istruzione ADLER32) nelle CPU commerciali general-purpose. L'hardware viene sfruttato in due modi alternativi:
// Accelerazione tramite istruzioni vettoriali (SIMD): poiché manca un'istruzione nativa, le CPU moderne emulano l'hardware parallelo tramite registri vettoriali ad ampia architettura (128, 256 o 512 bit):
// 				-Intel/AMD (AVX2 / AVX-512): viene usata l'istruzione hardware _mm256_sad_epu8 (Sum of Absolute Differences) che esegue in hardware la somma parziale di 32 byte in un colpo solo, permettendo di accumulare dati nei registri a 32 bit
// 				e riducendo drasticamente il numero di operazioni di modulo prima del superamento della capacità del registro.
// 				-ARM (NEON / SVE): si utilizza l'istruzione hardware di prodotto scalare vettoriale vdotq_u32 (Vector Dot Product). Moltiplicando i byte per una sequenza di pesi decrescenti direttamente nei registri hardware,
// 				si calcola la cascata di somme parallele richiesta dall'accumulatore secondario dell'Adler-32 su blocchi da 16 byte per ciclo.
// Co-processori e acceleratori dedicati (SoC / Server):la vera implementazione 100% hardware di Adler-32 risiede in chip ausiliari o blocchi di silicio IP dedicati alla compressione dati.
// 				-Intel QAT (QuickAssist Technology):un co-processore integrato nei chipset dei server Xeon. Contiene circuiti logici dedicati all'algoritmo DEFLATE (zlib) che calcolano l'Adler-32 direttamente in hardware (tramite sommatori e riduttori modulari combinatori a singolo ciclo) a velocità di linea,
//     liberando completamente i core della CPU.
//     -IBM zEnterprise (Mainframe) & Apple Silicon (Serie M): integrano acceleratori hardware on-chip (es. i motori hardware di decompressione di Apple o il Nest Accelerator di IBM) dedicati alla decompressione dei file di sistema.
//     Questi blocchi di silicio calcolano i checksum di integrità (tra cui varianti Adler) tramite logica cablata fissa non programmabile per velocizzare l'I/O.

unsigned int adler32(unsigned char *data, size_t len)
{
 unsigned int s1 = 1;
 unsigned int s2 = 0;
 const unsigned int MOD_ADLER = 65521;

 for (size_t i = 0; i < len; i++)
 {
		// il modulo ad ogni iterazione è computazionalmente costoso — le implementazioni production come zlib eseguono il % ogni 5552 byte (il massimo numero di iterazioni prima che s2 possa superare 2^32) invece che ad ogni byte
		s1 = (s1 + data[i]) % MOD_ADLER;
		s2 = (s2 + s1) % MOD_ADLER;
 }

 return (s2 << 16) | s1;
}

int main()
{
 // il checksum Adler-32 per la stringa "Wikipedia" è 0x11E60398
 unsigned char *test = (unsigned char *)"Wikipedia";
 unsigned int hash = adler32(test, 9);

 printf("Adler-32 di 'Wikipedia': 0x%08X\n", hash);
 return 0;
}
