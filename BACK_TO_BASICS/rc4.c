#include <stdio.h>
#include <string.h>

// RC4 e' un leggendario cifrario a flusso (stream cipher) ,inventato da Ron Rivest nel 1987, che ha fatto la storia della crittografia simmetrica.
// Rivest Cipher 4  genera un flusso di byte pseudo-casuali (chiamato keystream) che viene unito ai dati in chiaro tramite l'operazione bitwise XOR.
// Per decifrare, basta applicare lo stesso identico keystream ai dati cifrati (operazione simmetrica).
// L'algoritmo si basa su un vettore di stato interno (una permutazione) di 256 byte, chiamato S, e si articola in due fasi matematiche rigide:
// 1) KSA (Key-Scheduling Algorithm): inizializza lo stato S usando la chiave segreta (lunga da 1 a 256 byte)
// Nella KSA prima si riempie S con i valori da 0 a 255 (S[i] = i),poi si mescola gli elementi di S basandosi sui byte della chiave tramite un indice di sfasamento j:
// j = (j + S[i] + chiave[i (% key_len}]) % 256,quindi scambia (swap) i valori di S[i] e S[j].
// 2) PRGA (Pseudo-Random Generation Algorithm): genera un byte di keystream alla volta modificando lo stato di S ad ogni ciclo. Usando due puntatori i e j (inizialmente a 0).
// i = (i + 1) % 256, j = (j + S[i]) % 256, swap S[i] e S[j], l'indice del byte di keystream generato sarà: t = (S[i] + S[j]) % 256.
// Il byte di keystream è K = S[t].
// RC4 non è più considerato sicuro (è stato deprecato ufficialmente da RFC 7465). Il motivo risiede in un bias statistico nei primi byte del keystream (specialmente il secondo byte
// ha una probabilità anomala di essere 0). Se un attaccante intercetta milioni di messaggi cifrati con la stessa chiave, può ricostruire il testo in chiaro.
// Nonostante le sue vulnerabilità attuali, RC4 è stato per decenni il re della crittografia grazie alla sua leggerezza:
// -WEP (Wired Equivalent Privacy):Il primo e fallimentare protocollo di sicurezza del Wi-Fi usava RC4 (e i suoi bug di inizializzazione della chiave hanno permesso di violare le reti Wi-Fi in pochi minuti).
// -TLS / SSL: utilizzato fino a pochi anni fa per proteggere il traffico HTTPS sul web come fallback veloce quando le CPU non avevano l'accelerazione hardware per AES.
// -Microsoft RDP & BitTorrent: usato per offuscare il traffico e proteggere le sessioni remote con pochissimo overhead di calcolo.
// Non esistono istruzioni native RC4 nei processori Intel, AMD o ARM moderni. Trattandosi di un algoritmo deprecato e insicuro, l'industria del silicio ha preferito concentrare tutti i transistor sull'
// accelerazione hardware di AES (tramite le istruzioni AES-NI) e di ChaCha20 (via istruzioni vettoriali SIMD).
// Accelerazione via Software (sfruttamento HW): l'algoritmo non è parallelizzabile intrinsecamente perché la generazione del byte di keystream N dipende strettamente dallo stato modificato
// dal byte N-1. Non può quindi sfruttare le istruzioni hardware SIMD (AVX/NEON) per elaborare più byte contemporaneamente. L'unico vantaggio hardware che sfrutta è che le operazioni sono tutte ad 8 bit,
// mappandosi perfettamente sui registri base della CPU ed evitando il crossword.
// In passato (soprattutto nei vecchi router Wi-Fi), l'RC4 veniva implementato in hardware usando una Dual-Port RAM da 256 byte. Questo blocco di memoria hardware consentiva di leggere S[i] e S[j]
// nello stesso ciclo di clock, eseguendo lo swap e la generazione del keystream in un singolo ciclo hardware, senza latenze di caricamento tipiche delle CPU.

void rc4_ksa(unsigned char *S, const unsigned char *key, int key_len)
{
    int i, j = 0;

    // inizializzazione lineare
    for (i = 0; i < 256; i++)
        S[i] = i;

    // mescolamento tramite la chiave
    for (i = 0; i < 256; i++)
    {
        j = (j + S[i] + key[i % key_len]) % 256;
        // swap bitwise
        S[i] ^= S[j];
        S[j] ^= S[i];
        S[i] ^= S[j];
    }
}

void rc4_prga_xor(unsigned char *S, const unsigned char *data, unsigned char *output, size_t len)
{
    int i = 0, j = 0, t;
    unsigned char keystream_byte;

    for (size_t k = 0; k < len; k++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        // swap dello stato
        S[i] ^= S[j];
        S[j] ^= S[i];
        S[i] ^= S[j];

        // generazione indice e byte di keystream
        t = (S[i] + S[j]) % 256;
        keystream_byte = S[t];

        // cifratura/decifratura tramite XOR
        output[k] = data[k] ^ keystream_byte;
    }
}

int main()
{
    unsigned char S[256];
    unsigned char key[] = "Key";
    unsigned char plaintext[] = "Plaintext";
    size_t len = strlen((char *)plaintext);

    unsigned char ciphertext[10];
    unsigned char decrypted[10];

    // cifratura
    rc4_ksa(S, key, strlen((char *)key));
    rc4_prga_xor(S, plaintext, ciphertext, len);

    // decifratura (reinizializzo lo stato S con la stessa chiave)
    rc4_ksa(S, key, strlen((char *)key));
    rc4_prga_xor(S, ciphertext, decrypted, len);
    decrypted[len] = '\0';

    printf("Decifrato: %s\n", decrypted); // ristampa "Plaintext"
    return 0;
}