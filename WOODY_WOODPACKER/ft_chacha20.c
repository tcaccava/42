/*

  1. GENESI, FILOSOFIA ARCHITETTURALE E PARADIGMA ARX
  ============================================================================
  ChaCha20 e' un cifrario a flusso (stream cipher) progettato nel 2008 da
  Daniel J. Bernstein (djb) come evoluzione diretta di Salsa20 e standardizzato
  dall'IETF nella RFC 8439.
  A differenza dei cifrari a blocchi classici basati su reti di sostituzione e
  permutazione (SPN, come AES) o reti di Feistel con S-Box (come DES/Blowfish),
  ChaCha20 appartiene alla famiglia dei cifrari ARX. L'intero arsenale
  matematico della funzione di round si riduce a tre sole operazioni primitive
  su word unsigned a 32 bit:

	1) Addizione Modulare:  a = (a + b) mod 2^32
	2) Rotazione Bitwise:   x = (x << n) | (x >> (32 - n))
	3) XOR Bitwise:         a = a ^ b

Ruolo algebrico di ciascuna primitiva:
 - NON-LINEARITA' (Addizione Modulare): se un cifrario usasse esclusivamente
   XOR e rotazioni, ogni bit di output sarebbe una combinazione lineare su
   GF(2) dei bit di input; l'intero cifrario collasserebbe in un sistema di
   512 equazioni lineari risolvibile in tempo polinomiale tramite eliminazione
   di Gauss. L'addizione modulo 2^32 introduce il riporto (carry bit):
   c_{i+1} = (x_i & y_i) | ((x_i ^ y_i) & c_i). La presenza della AND logica
   nel calcolo del carry rompe la linearita' algebrica su GF(2), rendendo
   il sistema resistente alla crittoanalisi lineare e differenziale.
 - DIFFUSIONE INTRA-WORD (Rotazione Bitwise): nell'addizione modulare il
   carry si propaga esclusivamente dai bit meno significativi (LSB) verso i
   piu' significativi (MSB); il bit 0 non riceve mai riporto. La rotazione
   ciclica a sinistra sposta i bit ad alta entropia (MSB) nelle posizioni
   basse (LSB) prima dell'addizione successiva, forzando il carry a
   propagarsi uniformemente su tutti i 32 bit della word.
 - MESCOLAMENTO PRIVO DI BIAS (XOR): combina due word in tempo costante con
   distribuzione perfettamente uniforme (Pr[X ^ Y = 0] = 0.5) senza generare
   riporti spuri che potrebbero annullare l'effetto delle rotazioni.

   Scelta delle costanti di rotazione (16, 12, 8, 7):
   Le quattro distanze di rotazione non sono arbitrarie: sono state selezionate
   per massimizzare l'effetto valanga (Strict Avalanche Criterion - SAC: la
   modifica di 1 singolo bit in ingresso deve invertire ciascun bit in uscita
   con probabilita' ~0.5 nel minor numero possibile di round) e al contempo
   minimizzare il costo su CPU reali (16 e 8 sono multipli esatti di 8 bit,
   mappabili da compilatori e architetture SIMD su shuffle/permute a singolo
   ciclo, mentre 12 e 7 massimizzano la dispersione dei carry tra bit pari e
   dispari).


 2. SICUREZZA SIDE-CHANNEL E CONSTANT-TIME EXECUTION
 ============================================================================
   Nei cifrari basati su S-Box (tabelle di lookup in RAM, come AES implementato
   in software senza accelerazione hardware AES-NI), l'accesso a memoria del
   tipo S[key_byte ^ plaintext_byte] dipende da dati segreti. Su CPU moderne
   dotate di gerarchie di cache (L1/L2/L3), linee di cache diverse (tipicamente
   da 64 byte) hanno tempi di accesso differenti (cache hit ~3-4 cicli vs cache
   miss ~12-100+ cicli). Un attaccante in grado di misurare i tempi di
   esecuzione o di eseguire attacchi Flush+Reload / Prime+Probe puo' ricostruire
   i bit alti degli indici e recuperare la chiave (Cache-Timing Attack).

   Il paradigma ARX elimina questo vettore d'attacco alla radice:
 - Zero tabelle di lookup in memoria (nessun accesso RAM indicizzato da dati
   segreti).
 - Zero salti condizionali (if / branch): il branch predictor della CPU non
   viene mai sollecitato in funzione della chiave o del testo in chiaro.
 - Zero istruzioni a latenza variabile: su architetture x86-64 e ARM, ADD,
   XOR e ROL eseguono in esattamente 1 ciclo di clock indipendentemente
   dal valore numerico degli operandi (a differenza di DIV o di alcune
   moltiplicazioni su microcontrollori embedded).
   Il codice risulta pertanto intrinsecamente "data-oblivious" (constant-time).


 3. ANATOMIA DELLA MATRICE DI STATO 4x4 (512 BIT / 64 BYTE)
 ============================================================================
   Lo stato interno di ChaCha20 e' una matrice 4x4 di 16 word a 32 bit senza
   segno (uint32_t state[16]), pari a 64 byte complessivi:

	+------------+------------+------------+------------+
	|  state[0]  |  state[1]  |  state[2]  |  state[3]  |  Riga 0: Costanti
	| 0x61707865 | 0x3320646e | 0x79622d32 | 0x6b206574 |  ("expand 32-byte k")
	+------------+------------+------------+------------+
	|  state[4]  |  state[5]  |  state[6]  |  state[7]  |  Riga 1: Key [0..127]
	|   Key[0]   |   Key[1]   |   Key[2]   |   Key[3]   |  (primi 16 byte)
	+------------+------------+------------+------------+
	|  state[8]  |  state[9]  |  state[10] |  state[11] |  Riga 2: Key [128..255]
	|   Key[4]   |   Key[5]   |   Key[6]   |   Key[7]   |  (ultimi 16 byte)
	+------------+------------+------------+------------+
	|  state[12] |  state[13] |  state[14] |  state[15] |  Riga 3: Counter (32b)
	|  Counter   |  Nonce[0]  |  Nonce[1]  |  Nonce[2]  |  + Nonce (96 bit)
	+------------+------------+------------+------------+

 - Costanti "Nothing-Up-My-Sleeve" (state[0..3]):
   Rappresentano la stringa ASCII "expand 32-byte k" codificata in
   little-endian:
	 "expa" -> 0x61707865
	 "nd 3" -> 0x3320646e
	 "2-by" -> 0x79622d32
	 "te k" -> 0x6b206574
   Svolgono due funzioni critiche:
   1) Rompono le simmetrie algebriche e garantiscono che lo stato iniziale non
	  sia mai il vettore nullo (all-zero state), evitando punti fissi banali
	  della trasformazione ARX anche nell'ipotesi estrema di chiave, counter e
	  nonce tutti nulli.
   2) Dimostrano l'assenza di backdoor matematiche occulte nei parametri del
	  cifrario (costanti verificabili pubblicamente, in netta antitesi a
	  standard opachi come Dual_EC_DRBG).

 - Chiave a 256 bit (state[4..11]):
   8 word da 32 bit (32 byte totali). Garantisce una sicurezza di 256 bit
   contro attacchi brute-force classici e 128 bit contro l'algoritmo
   quantistico di Grover (post-quantum symmetric security).

 - Block Counter a 32 bit (state[12]) e Nonce a 96 bit (state[13..15]):
   Nella variante IETF RFC 8439, il contatore occupa 1 word (32 bit) e il
   nonce occupa 3 word (96 bit = 12 byte). Il contatore identifica l'indice
   progressivo del blocco da 64 byte corrente (0, 1, 2, ...).
   (Nota storica: nella specifica originale di Bernstein del 2008, il counter
   era a 64 bit su state[12..13] e il nonce a 64 bit su state[14..15]).
   I Nonce permettono di riutilizzare la stessa chiave su flussi diversi senza 
   generare lo stesso keystream.


 4. DINAMICA DEL QUARTER ROUND E DIFFUSIONE COLONNE/DIAGONALI (20 ROUND)
============================================================================
   Il Quarter Round opera su 4 word (a, b, c, d) in due semi-fasi strutturalmente
   identiche ma con distanze di rotazione decrescenti:

	Fase 1:                          Fase 2:
	  a = a + b                        a = a + b
	  d = (d ^ a) <<< 16               d = (d ^ a) <<< 8
	  c = c + d                        c = c + d
	  b = (b ^ c) <<< 12               b = (b ^ c) <<< 7

   Evoluzione rispetto a Salsa20:
   In Salsa20 ogni word veniva aggiornata una sola volta per quarter-round e
   i round si alternavano tra righe e colonne. In ChaCha20 ogni word viene
   aggiornata DUE volte per quarter-round (a e c ricevono due addizioni;
   b e d ricevono due XOR + rotazioni) e i round alternano:
	- 4 Quarter Round sulle COLONNE:
		QR(0, 4, 8, 12), QR(1, 5, 9, 13), QR(2, 6, 10, 14), QR(3, 7, 11, 15)
	- 4 Quarter Round sulle DIAGONALI:
		QR(0, 5, 10, 15), QR(1, 6, 11, 12), QR(2, 7, 8, 13), QR(3, 4, 9, 14)

   Questa coppia (4 colonne + 4 diagonali = 2 round) viene iterata 10 volte,
   per un totale di 20 round (80 esecuzioni del Quarter Round per blocco da
   64 byte). L'alternanza colonne-diagonali raddoppia la velocita' di
   diffusione rispetto a Salsa20: in pochissimi round ogni bit di input
   influenza tutti i 512 bit dello stato. La migliore crittoanalisi nota in
   letteratura viola ~7-7.25 round con complessita' > 2^250, lasciando un
   margine di sicurezza superiore a 12 round completi.


 5. COSTRUZIONE DI DAVIES-MEYER (FEED-FORWARD) E NON-INVERTIBILITA'
 ============================================================================
   Una singola istanza del Quarter Round e' una biiezione strettamente
   invertibile: conoscendo (a, b, c, d), e' banale tornare indietro
   applicando le operazioni inverse in ordine opposto (rotazioni a destra ROR e
   sottrazioni modulo 2^32). Di conseguenza, l'intera cascata dei 20 round
   definisce una permutazione invertibile pi : {0,1}^512 -> {0,1}^512.

   Se il blocco di keystream fosse semplicemente pi(state), un attaccante in
   possesso di 64 byte di testo in chiaro noto (Known-Plaintext Attack, ad
   esempio i primi byte prevedibili di un segmento ELF o padding di zeri)
   calcolerebbe K = C ^ P = pi(state), invertirebbe i 20 round e leggerebbe
   direttamente la chiave segreta dalle celle state[4..11].

   Per impedire l'inversione, al termine dei 20 round ChaCha20 applica la
   somma modulare word-by-word con lo stato iniziale (costruzione feed-forward
   di Davies-Meyer):

   out[i] = (pi(state)[i] + state[i]) mod 2^32    per i = 0..15

   Questa operazione trasforma la permutazione pi in una funzione pseudo-casuale
   unidirezionale (PRF): conoscendo soltanto la somma out[i], non e' possibile
   separare i due addendi pi(state)[i] e state[i], rendendo computazionalmente
   intrattabile risalire alla chiave anche con quantita' arbitrarie di keystream.


 6. LIMITI OPERATIVI: TWO-TIME PAD, LIMITE 256 GiB E MALLEABILITA' (AEAD)
  ============================================================================
 - Divieto di riutilizzo (Key, Nonce) [Two-Time Pad]:
   Poiche' ChaCha20 cifra tramite C = P ^ K, cifrare due messaggi distinti
   P1 e P2 con la stessa identica coppia (Key, Nonce) produce:
	 C1 ^ C2 = (P1 ^ K) ^ (P2 ^ K) = P1 ^ P2
   annullando completamente il keystream ed esponendo la differenza tra i
   due testi in chiaro.
 - Limite volumetrico di 256 GiB per singolo Nonce:
   Con un block counter a 32 bit (state[12]), una coppia (Key, Nonce) puo'
   generare al massimo 2^32 blocchi da 64 byte, pari a:
	 2^32 * 64 byte = 2^38 byte = 256 GiB
   Superata questa soglia, il counter va in overflow tornando a 0 e ripetendo
   il keystream. (Per superare questo limite ed evitare collisioni con nonce
   generati casualmente esiste XChaCha20, che usa un passaggio preliminare
   HChaCha20 per derivare una subkey con un nonce esteso a 192 bit).
 - Malleabilita' e necessita' di Poly1305 nei protocolli di rete:
   ChaCha20 puro garantisce confidenzialita' ma NON integrita' ne'
   autenticita': invertire un bit nel ciphertext (C' = C ^ \Delta) produce
   esattamente lo stesso bit invertito nel plaintext decifrato (P' = P ^ \Delta).
   Per questo motivo in TLS 1.3, WireGuard e QUIC (HTTP/3) ChaCha20 e' sempre
   accoppiato all'autenticatore polinomiale Poly1305 in modalita' AEAD
   (ChaCha20-Poly1305, RFC 8439 Sezione 2.8). Nel contesto di un packer ELF
   come woody_woodpacker, dove l'obiettivo e' la cifratura/offuscamento in
   RAM senza overhead di tag MAC, ChaCha20 standalone e' la scelta ideale.


 7. ARCHITETTURA HARDWARE, ENDIANNESS E PARALLELISMO SIMD (AVX2 / AVX-512)
============================================================================
 - Serializzazione Little-Endian (RFC 8439 Sezione 2.3):
   Ogni word a 32 bit dello stato finale out[0..15] viene serializzata in
   4 byte in ordine little-endian (byte meno significativo all'indirizzo piu'
   basso). Su architetture x86-64 (nativamente little-endian), il layout in
   RAM dell'array uint32_t keystream[16] coincide gia' byte per byte con
   lo stream canonico RFC 8439: non e' necessaria alcuna istruzione BSWAP.
 - Parallelismo Vettoriale (l'opposto di RC4):
   In RC4 la generazione del byte N dipende dallo stato mutato dal byte N-1,
   impedendo qualsiasi vettorizzazione SIMD. In ChaCha20 invece:
   1) Parallelismo intra-blocco (SSE/NEON 128-bit): i 4 Quarter Round delle
	  colonne sono completamente indipendenti tra loro, cosi' come i 4 delle
	  diagonali; tutti e 4 possono essere eseguiti simultaneamente in un
	  singolo registro vettoriale a 128 bit (4 x uint32_t).
   2) Parallelismo inter-blocco (AVX2 256-bit / AVX-512 512-bit): blocchi
	  consecutivi (counter = k, k+1, k+2, ...) sono indipendenti e possono
	  essere calcolati in parallelo elaborando 256 byte (AVX2) o 512 byte
	  (AVX-512) per ciclo di iterazione, raggiungendo ~1-2 cicli di clock
	  per byte su CPU x86-64 senza richiedere estensioni dedicate AES-NI.
 */

#include <stdint.h>
#include <stddef.h>

/*
 * Rotazione ciclica a sinistra su 32 bit (Left Circular Shift).
 * La maschera & 31 evita Undefined Behavior nello standard C99/C11 nel caso
 * limite n == 0 (dove x >> 32 su un uint32_t sarebbe UB). I compilatori
 * moderni (GCC/Clang con -O1 o superiore) riconoscono questo idioma e lo
 * compilano in una singola istruzione hardware rol eax, cl / rol reg, imm8.
 */
static inline uint32_t ft_rol32(uint32_t x, uint32_t n)
{
	return ((x << (n & 31U)) | (x >> ((32U - n) & 31U)));
}

/*
 * Copia byte-level priva di dipendenze esterne (utile per serializzare key,
 * nonce e lo stato iniziale senza includere <string.h>).
 */
static void *ft_chacha_memcpy(void *dst, const void *src, size_t n)
{

	if (!dst && !src)
		return (NULL);
	unsigned char *d = (unsigned char *)dst;
	const unsigned char *s = (const unsigned char *)src;
	size_t i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

/*
 * Zeroizzazione esplicita a prova di Dead-Store Elimination (DSE).
 * Il puntatore volatile impedisce al compilatore di ottimizzare via la
 * pulizia della memoria sullo stack al termine della funzione, evitando che
 * la chiave a 256 bit o il keystream restino residui nella RAM del processo.
 */
static void ft_chacha_bzero(void *ptr, size_t n)
{
	volatile unsigned char *p;
	size_t i;

	p = (volatile unsigned char *)ptr;
	i = 0;
	while (i < n)
	{
		p[i] = 0;
		i++;
	}
}

/*
 * Quarter Round di ChaCha20 (RFC 8439 Sezione 2.1).
 * Mescola 4 word a 32 bit tramite 4 addizioni modulari, 4 XOR e 4 rotazioni
 * con distanze (16, 12, 8, 7). Ogni word viene aggiornata due volte.
 */
static void ft_chacha20_quarter_round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
	*a += *b;
	*d = ft_rol32(*d ^ *a, 16);
	*c += *d;
	*b = ft_rol32(*b ^ *c, 12);
	*a += *b;
	*d = ft_rol32(*d ^ *a, 8);
	*c += *d;
	*b = ft_rol32(*b ^ *c, 7);
}

/*
 * Block Function di ChaCha20 (RFC 8439 Sezione 2.3).
 * Prende la matrice di stato iniziale `state[16]` (64 byte), la copia nel
 * buffer di lavoro `out[16]`, applica 20 round (10 doppi round colonne +
 * diagonali) e infine esegue la somma feed-forward `out[i] += state[i]`.
 */
static void ft_chacha20_block(uint32_t out[16], const uint32_t state[16])
{
	int i;

	i = 0;
	while (i < 16)
	{
		out[i] = state[i];
		i++;
	}
	i = 0;
	while (i < 10)
	{
		/* 4 Quarter Round sulle colonne della matrice 4x4 */
		ft_chacha20_quarter_round(&out[0], &out[4], &out[8], &out[12]);
		ft_chacha20_quarter_round(&out[1], &out[5], &out[9], &out[13]);
		ft_chacha20_quarter_round(&out[2], &out[6], &out[10], &out[14]);
		ft_chacha20_quarter_round(&out[3], &out[7], &out[11], &out[15]);
		/* 4 Quarter Round sulle diagonali della matrice 4x4 */
		ft_chacha20_quarter_round(&out[0], &out[5], &out[10], &out[15]);
		ft_chacha20_quarter_round(&out[1], &out[6], &out[11], &out[12]);
		ft_chacha20_quarter_round(&out[2], &out[7], &out[8], &out[13]);
		ft_chacha20_quarter_round(&out[3], &out[4], &out[9], &out[14]);
		i++;
	}
	/* Feed-forward di Davies-Meyer: rende la funzione non invertibile */
	i = 0;
	while (i < 16)
	{
		out[i] += state[i];
		i++;
	}
}

/*
 * Cifratura / Decifratura simmetrica in-place tramite ChaCha20 (RFC 8439 Sec. 2.4).
 *
 * Parametri:
 * - buf:     puntatore al buffer da cifrare o decifrare in-place
 * - len:     lunghezza in byte del buffer (arbitraria, nessun padding richiesto)
 * - key:     chiave segreta a 256 bit (32 byte)
 * - nonce:   vettore di inizializzazione a 96 bit (12 byte)
 * - counter: valore iniziale del block counter a 32 bit (tipicamente 0 o 1)
 *
 * Nota: essendo uno stream cipher basato su XOR col keystream deterministico,
 * la medesima funzione esegue sia la cifratura che la decifratura.
 */
void ft_chacha20_xor_ic(uint8_t *buf, size_t len, const uint8_t key[32],
						const uint8_t nonce[12], uint32_t counter)
{
	uint32_t state[16];
	uint32_t keystream[16];
	uint8_t *ks_bytes;
	size_t offset;
	size_t chunk;
	size_t i;

	if (!buf || !key || !nonce || len == 0)
		return;
	/* Costanti ASCII "expand 32-byte k" in little-endian */
	state[0] = 0x61707865U;
	state[1] = 0x3320646eU;
	state[2] = 0x79622d32U;
	state[3] = 0x6b206574U;
	ft_chacha_memcpy(&state[4], key, 32);
	state[12] = counter;
	ft_chacha_memcpy(&state[13], nonce, 12);

	ks_bytes = (uint8_t *)keystream;
	offset = 0;
	while (offset < len)
	{
		ft_chacha20_block(keystream, state);
		chunk = len - offset;
		if (chunk > 64)
			chunk = 64;
		i = 0;
		while (i < chunk)
		{
			buf[offset + i] ^= ks_bytes[i];
			i++;
		}
		state[12]++;
		offset += 64;
	}
	ft_chacha_bzero(state, sizeof(state));
	ft_chacha_bzero(keystream, sizeof(keystream));
}

/*
 * Wrapper standard con block counter iniziale impostato a 0.
 */
void ft_chacha20_xor(uint8_t *buf, size_t len, const uint8_t key[32],
					 const uint8_t nonce[12])
{
	ft_chacha20_xor_ic(buf, len, key, nonce, 0U);
}

/* ========================================================================== */
/* TEST SUITE UFFICIALE RFC 8439 SEZIONE 2.4.2                                */
/* Compilare con: gcc -Wall -Wextra -Werror -O3 -DCHACHA20_TEST_MAIN ...      */
/* ========================================================================== */
#ifdef CHACHA20_TEST_MAIN
#include <stdio.h>

static int ft_test_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;
	size_t i;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}

int main(void)
{
	/* RFC 8439 Section 2.4.2 Test Vector (114 bytes) */
	const uint8_t key[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
	const uint8_t nonce[12] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a,
		0x00, 0x00, 0x00, 0x00};
	const uint8_t expected[114] = {
		0x6e, 0x2e, 0x35, 0x9a, 0x25, 0x68, 0xf9, 0x80,
		0x41, 0xba, 0x07, 0x28, 0xdd, 0x0d, 0x69, 0x81,
		0xe9, 0x7e, 0x7a, 0xec, 0x1d, 0x43, 0x60, 0xc2,
		0x0a, 0x27, 0xaf, 0xcc, 0xfd, 0x9f, 0xae, 0x0b,
		0xf9, 0x1b, 0x65, 0xc5, 0x52, 0x47, 0x33, 0xab,
		0x8f, 0x59, 0x3d, 0xab, 0xcd, 0x62, 0xb3, 0x57,
		0x16, 0x39, 0xd6, 0x24, 0xe6, 0x51, 0x52, 0xab,
		0x8f, 0x53, 0x0c, 0x35, 0x9f, 0x08, 0x61, 0xd8,
		0x07, 0xca, 0x0d, 0xbf, 0x50, 0x0d, 0x6a, 0x61,
		0x56, 0xa3, 0x8e, 0x08, 0x8a, 0x22, 0xb6, 0x5e,
		0x52, 0xbc, 0x51, 0x4d, 0x16, 0xcc, 0xf8, 0x06,
		0x81, 0x8c, 0xe9, 0x1a, 0xb7, 0x79, 0x37, 0x36,
		0x5a, 0xf9, 0x0b, 0xbf, 0x74, 0xa3, 0x5b, 0xe6,
		0xb4, 0x0b, 0x8e, 0xed, 0xf2, 0x78, 0x5e, 0x42,
		0x87, 0x4d};
	uint8_t buf[115] =
		"Ladies and Gentlemen of the class of '99: "
		"If I could offer you only one tip for the future, "
		"sunscreen would be it.";
	uint8_t orig[115];

	ft_chacha_memcpy(orig, buf, 114);
	/* Nel test vector RFC 8439 Sec. 2.4.2 il counter iniziale e' 1 */
	ft_chacha20_xor_ic(buf, 114, key, nonce, 1U);
	if (ft_test_memcmp(buf, expected, 114) != 0)
	{
		printf("[FAIL] Ciphertext non conforme a RFC 8439!\n");
		return (1);
	}
	printf("[OK] Cifratura conforme al bit al test vector RFC 8439 (Sec. 2.4.2)\n");

	ft_chacha20_xor_ic(buf, 114, key, nonce, 1U);
	if (ft_test_memcmp(buf, orig, 114) != 0)
	{
		printf("[FAIL] Decifratura fallita!\n");
		return (1);
	}
	printf("[OK] Decifratura simmetrica verificata: \"%s\"\n", (char *)buf);
	return (0);
}
#endif
