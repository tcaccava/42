# Overflow e conversioni tra interi in C: guida completa

Guida pensata per chi studia C (in particolare alla 42) e vuole capire **una volta per tutte** cosa succede quando un valore "non ci sta" in un tipo intero.

---

## Indice

1. [Le basi: quanti valori sta in un tipo](#1-le-basi-quanti-valori-sta-in-un-tipo)
2. [Come sono rappresentati gli interi (complemento a due)](#2-come-sono-rappresentati-gli-interi-complemento-a-due)
3. [Le quattro categorie di comportamento dello standard](#3-le-quattro-categorie-di-comportamento-dello-standard)
4. [Overflow degli unsigned: wraparound garantito](#4-overflow-degli-unsigned-wraparound-garantito)
5. [Overflow dei signed: undefined behavior](#5-overflow-dei-signed-undefined-behavior)
6. [Conversioni tra tipi](#6-conversioni-tra-tipi)
7. [Promozioni intere e conversioni aritmetiche usuali](#7-promozioni-intere-e-conversioni-aritmetiche-usuali)
8. [Trappole classiche](#8-trappole-classiche)
9. [`atoi`, `strtol` e come scrivere una `ft_atoi` sicura](#9-atoi-strtol-e-come-scrivere-una-ft_atoi-sicura)
10. [Controllare l'overflow prima che avvenga](#10-controllare-loverflow-prima-che-avvenga)
11. [Strumenti: compilatore e sanitizer](#11-strumenti-compilatore-e-sanitizer)
12. [Applicazione al Rush00](#12-applicazione-al-rush00)
13. [Esercizi con soluzioni](#13-esercizi-con-soluzioni)
14. [Cheat sheet finale](#14-cheat-sheet-finale)

---

## 1. Le basi: quanti valori sta in un tipo

Un tipo intero con **n bit** può rappresentare esattamente **2ⁿ valori distinti**. Se provi a rappresentarne uno in più, "non ci sta": questo è l'overflow.

| Tipo | Bit tipici | Range (signed) | Range (unsigned) |
|---|---|---|---|
| `char` | 8 | -128 … 127 (*) | 0 … 255 |
| `short` | 16 | -32 768 … 32 767 | 0 … 65 535 |
| `int` | 32 | -2 147 483 648 … 2 147 483 647 | 0 … 4 294 967 295 |
| `long` | 64 su Linux/macOS a 64 bit, 32 su Windows | ±9.2·10¹⁸ (64 bit) | 0 … 1.8·10¹⁹ (64 bit) |
| `long long` | 64 | ±9.2·10¹⁸ | 0 … 1.8·10¹⁹ |

(*) `char` semplice può essere signed o unsigned a seconda della piattaforma. Per questo si usano `signed char` / `unsigned char` quando conta.

Importante: lo standard garantisce solo **minimi** (per esempio `int` ha almeno 16 bit). Le dimensioni sopra sono quelle tipiche dei sistemi moderni.

I limiti sono definiti in `<limits.h>`:

```c
INT_MAX     //  2147483647
INT_MIN     // -2147483648
UINT_MAX    //  4294967295
LONG_MAX, LONG_MIN, LLONG_MAX, ...
```

Nota l'**asimmetria** dei signed: `INT_MIN` ha valore assoluto `INT_MAX + 1`. Ci sono più numeri negativi che positivi. Questo causerà problemi più avanti.

---

## 2. Come sono rappresentati gli interi (complemento a due)

I computer moderni rappresentano i signed in **complemento a due**.

- Un unsigned a n bit è un normale numero binario: valori da `0` a `2ⁿ - 1`.
- Un signed a n bit usa lo stesso schema, ma il bit più significativo (MSB) pesa **-2ⁿ⁻¹** invece di **+2ⁿ⁻¹**.

Esempio con 8 bit:

```
0111 1111  =  127            (INT8_MAX)
1000 0000  = -128            (INT8_MIN)
1111 1111  =   -1
0000 0000  =    0
```

Per negare un numero: **inverti tutti i bit e somma 1** (`-x == ~x + 1`).

Cosa succede sommando 1 a 127 in 8 bit, "a livello hardware"?

```
  0111 1111   (127)
+ 0000 0001
= 1000 0000   (-128)   <- il bit di segno si accende
```

Questo è il **wraparound** che l'hardware fa di solito. Ma attenzione: **il fatto che l'hardware faccia così non significa che il linguaggio C lo garantisca** (vedi sezione 5).

> Dal C23 lo standard impone il complemento a due. Prima era "solo" quasi universale. Questo però **non** rende definito l'overflow dei signed.

---

## 3. Le quattro categorie di comportamento dello standard

Lo standard C classifica ciò che può accadere in quattro modi. Capirli è la chiave di tutto.

### 3.1 Comportamento definito

Lo standard dice esattamente cosa succede. Su qualsiasi compilatore, stesso risultato.

```c
unsigned int u = UINT_MAX;
u = u + 1;      // u == 0, garantito
```

### 3.2 Implementation-defined

Il risultato **può variare tra compilatori**, ma ogni compilatore deve **scegliere un comportamento e documentarlo**. Il programma resta valido e prevedibile su quel compilatore.

```c
long big = 4294967297;   // 2^32 + 1
int  x   = (int)big;     // implementation-defined
```

GCC documenta che in questo caso tiene gli ultimi 32 bit (modulo 2³²), quindi `x == 1`. Non è un bug: è una scelta documentata.

Altri esempi: `sizeof(int)`, la dimensione di `long`, lo shift a destra di un numero negativo, la signedness di `char`.

### 3.3 Unspecified

Il compilatore sceglie tra più possibilità, ma **non è obbligato a documentare** quale.

```c
f(g(), h());   // non è specificato se g() o h() viene valutata per prima
```

### 3.4 Undefined behavior (UB)

Lo standard **non pone alcun requisito**. Non significa "risultato strano": significa che **il programma non ha più un significato definito**. Può:

- dare il valore che ti aspetti (per caso),
- dare un valore sbagliato,
- crashare,
- essere compilato in modo che il codice dopo l'UB si comporti in modo incomprensibile,
- cambiare comportamento tra `-O0` e `-O2`, o tra una versione del compilatore e l'altra.

```c
int a = INT_MAX;
a = a + 1;        // UB
```

### Tabella riassuntiva

| Categoria | Risultato prevedibile? | Documentato? | Esempio |
|---|---|---|---|
| Definito | Sì, ovunque | Sì (standard) | `UINT_MAX + 1` |
| Implementation-defined | Sì, su quel compilatore | Sì | `(int)` di un `long` fuori range |
| Unspecified | Una tra poche opzioni | No | ordine di valutazione degli argomenti |
| Undefined | **No, nessuna garanzia** | Non ha senso | `INT_MAX + 1` |

---

## 4. Overflow degli unsigned: wraparound garantito

Per i tipi **unsigned** lo standard garantisce l'aritmetica **modulo 2ⁿ**. Non esiste "overflow" in senso tecnico: il risultato "si avvolge".

```c
unsigned int a = UINT_MAX;
a + 1;       // 0
0u - 1;      // UINT_MAX
a * 2;       // UINT_MAX - 1   (cioè 2^33 - 2 mod 2^32 = 4294967294)
```

È utile per hash, generatori pseudo-casuali, aritmetica modulare. È anche fonte di bug se non te lo aspetti.

**Esempio di bug tipico:**

```c
unsigned int a = 3;
unsigned int b = 5;
if (a - b > 0)          // SEMPRE vero (a - b == 4294967294), mai negativo!
    printf("a > b?\n");
```

Un unsigned non può mai essere negativo: `a - b` dà un numero enorme.

---

## 5. Overflow dei signed: undefined behavior

Per i tipi **signed** (`int`, `long`, `short`, ...) un'**operazione aritmetica** il cui risultato non è rappresentabile è **undefined behavior**.

```c
int a = INT_MAX;
a + 1;          // UB
a * 2;          // UB
INT_MIN - 1;    // UB
-INT_MIN;       // UB  (+2147483648 non esiste in int)
```

### 5.1 "Ma sul mio PC dà -2147483648!"

Spesso vero, perché l'istruzione `add` della CPU fa wraparound. Ma **questo non è garantito dal linguaggio**, e soprattutto non è ciò su cui si basa il compilatore.

### 5.2 Perché l'UB è pericoloso: l'ottimizzatore

Il compilatore **assume che l'UB non avvenga mai**. Quindi può ragionare così:

```c
int check(int a)
{
    if (a + 1 > a)      // se a + 1 sforasse sarebbe UB, quindi "non può succedere"
        return 1;
    return 0;
}
```

Il compilatore può concludere che `a + 1 > a` è **sempre vero** e trasformare la funzione in `return 1;`. Il tuo controllo, scritto proprio per intercettare l'overflow con `a == INT_MAX`, **sparisce dal binario**.

Altro esempio, un ciclo che "dovrebbe" terminare per wraparound:

```c
for (int i = 1; i > 0; i *= 2)   // si aspetta che i diventi negativo
    ;
```

Con UB il compilatore è libero di trattarlo come un ciclo infinito, o in altri modi.

### 5.3 Regola pratica

> **Mai contare sul wraparound dei signed.** Controlla *prima* che l'operazione non sfori (sezione 10), oppure usa un tipo più largo, oppure un unsigned se vuoi davvero il modulo.

### 5.4 Altri UB legati agli interi signed

| Espressione | Problema |
|---|---|
| `INT_MIN / -1` | il risultato `+2147483648` non è rappresentabile (UB, e su x86 crasha con SIGFPE) |
| `INT_MIN % -1` | UB anch'esso |
| `x / 0`, `x % 0` | UB |
| `abs(INT_MIN)`, `-INT_MIN` | UB, perché `+INT_MIN` non esiste (asimmetria del range) |
| `1 << 31` su `int` | scrive nel bit di segno: UB prima di C23 per valori non rappresentabili |
| `x << n` o `x >> n` con `n < 0` o `n >= bit di x` | UB |
| `x << n` con `x` negativo | UB |

(Lo shift **a destra** di un numero negativo non è UB: è implementation-defined, di solito shift aritmetico.)

---

## 6. Conversioni tra tipi

Un'overflow non nasce solo da operazioni: può nascere da una **conversione** quando il valore non entra nel tipo di destinazione. Qui le regole sono **diverse** dall'aritmetica.

### 6.1 Verso un tipo unsigned: sempre definito

Il valore viene ridotto **modulo 2ⁿ** (n = bit del tipo di destinazione).

```c
unsigned int  a = -1;          // UINT_MAX (4294967295)
unsigned char c = 300;         // 300 mod 256 = 44
unsigned int  d = 4294967297;  // 4294967297 mod 2^32 = 1
```

### 6.2 Verso un tipo signed, valore fuori range: implementation-defined

Non è UB. Il risultato dipende dal compilatore (o viene sollevato un segnale definito dall'implementazione, caso raro).

```c
int  x = (int)4294967297L;     // GCC/Clang: 1  (tiene i 32 bit bassi)
int  y = (int)2147483648L;     // GCC/Clang: -2147483648
signed char s = 200;           // GCC/Clang: -56
```

GCC e Clang fanno così: **tengono gli n bit bassi e li reinterpretano in complemento a due**. Ma in teoria un altro compilatore potrebbe fare diversamente.

### 6.3 Verso un tipo più largo: sempre sicuro

Se il valore è rappresentabile nel tipo di destinazione, **non cambia**. Un `int` entra sempre in un `long long`. Un `int` negativo in un `unsigned long` viene prima esteso col segno e poi ridotto modulo 2⁶⁴ (spesso sorprendente):

```c
int n = -1;
unsigned long u = n;    // 18446744073709551615 (ULONG_MAX su 64 bit)
```

### 6.4 Float → intero

Se la parte intera non è rappresentabile nel tipo intero di destinazione, è **UB**. (Caso storico: il fallimento del razzo Ariane 5 nel 1996 coinvolse una conversione da virgola mobile a 64 bit in intero a 16 bit fuori range.)

### 6.5 Tabella riassuntiva delle conversioni fuori range

| Da → A | Comportamento |
|---|---|
| qualsiasi intero → unsigned | **definito**, modulo 2ⁿ |
| qualsiasi intero → signed (valore non rappresentabile) | **implementation-defined** |
| float → intero (parte intera non rappresentabile) | **UB** |
| operazione aritmetica signed che sfora | **UB** |
| operazione aritmetica unsigned che sfora | **definito**, modulo 2ⁿ |

---

## 7. Promozioni intere e conversioni aritmetiche usuali

Prima di operare, il C converte automaticamente gli operandi. Conoscere queste regole evita molti bug.

### 7.1 Promozioni intere

I tipi più piccoli di `int` (`char`, `short`, `bool`) vengono promossi a `int` **prima** di qualsiasi operazione aritmetica.

```c
unsigned char a = 200;
unsigned char b = 100;
int sum = a + b;            // 300: avviene in int, nessun overflow
unsigned char r = a + b;    // 300 convertito in unsigned char = 44 (definito)
```

### 7.2 Conversioni aritmetiche usuali

Quando i due operandi hanno tipi diversi, vengono portati a un tipo comune. Semplificando:

1. se uno è `long double`/`double`/`float`, si va a virgola mobile;
2. altrimenti dopo le promozioni: se uno è `unsigned` e di rango uguale o maggiore, **l'altro viene convertito in unsigned**;
3. altrimenti si va al tipo di rango maggiore (se può contenere tutti i valori dell'altro).

La regola 2 è la fonte di errori più frequente: **signed e unsigned della stessa dimensione → il signed diventa unsigned**.

---

## 8. Trappole classiche

### 8.1 Confronto signed/unsigned

```c
int      a = -1;
unsigned b = 1;
if (a < b)          // FALSO! a viene convertito in unsigned: 4294967295 < 1
    puts("minore");
```

`-Wextra` (incluso nelle flag di 42) segnala `-Wsign-compare` in molti di questi casi.

### 8.2 Ciclo infinito con `size_t`

```c
for (size_t i = n - 1; i >= 0; i--)   // i >= 0 è SEMPRE vero per un unsigned
    ...
```

Al passaggio da `0` a `i--` si ottiene `SIZE_MAX`, e il ciclo non termina (o scrive fuori da un array). Soluzione:

```c
for (size_t i = n; i-- > 0; )
    ...
```

### 8.3 `n - 1` quando `n` è un `size_t` pari a 0

```c
size_t len = 0;
size_t last = len - 1;      // SIZE_MAX, non -1
```

### 8.4 Moltiplicazioni per allocare memoria

```c
int n = 1000000;
int *p = malloc(n * 4096);   // n * 4096 sfora int: UB, e poi alloca una quantità sbagliata
```

Un overflow sul calcolo della dimensione può portare ad allocare molto meno del previsto e poi scrivere oltre il buffer: classica origine di vulnerabilità. Usa tipi larghi (`size_t`) e controlla.

### 8.5 `abs` e `INT_MIN`

```c
abs(INT_MIN);     // UB: +2147483648 non è un int
```

Se devi gestire valori arbitrari, lavora in `long long` oppure in `unsigned` dopo aver trattato il caso a parte.

### 8.6 `char` con segno

```c
char c = 200;        // se char è signed: -56 (implementation-defined)
if (c == 200)        // FALSO su piattaforme con char signed
```

Per i byte usa `unsigned char`.

### 8.7 Funzioni `ctype.h`

`isalpha(c)` & co. richiedono un valore rappresentabile in `unsigned char` o `EOF`. Passare un `char` negativo è UB: casta a `(unsigned char)`.

---

## 9. `atoi`, `strtol` e come scrivere una `ft_atoi` sicura

### 9.1 `atoi`

```c
int atoi(const char *nptr);
```

- Converte la stringa in `int`.
- **Non segnala errori**: non puoi distinguere `"0"` da `"abc"`, né sapere se il numero era troppo grande.
- Se il valore non è rappresentabile in `int`, il comportamento è **indefinito** (lo standard lo dice esplicitamente).
- Nella pratica su glibc è `(int)strtol(nptr, NULL, 10)`: il `long` a 64 bit viene **troncato** a 32 bit.

Esempi su glibc (Linux 64 bit), ricordando che **non puoi farci affidamento**:

| Stringa | Valore ottenuto |
|---|---|
| `"2147483647"` | `2147483647` |
| `"2147483648"` | `-2147483648` |
| `"4294967295"` | `-1` |
| `"4294967296"` | `0` |
| `"4294967297"` | `1` |
| `"99999999999999999999"` | `-1` (strtol satura a `LONG_MAX`, che troncato dà -1) |

L'ultimo caso mostra bene perché è fragile: un numero enorme diventa `-1`, un valore plausibile.

### 9.2 `strtol`: la versione che segnala errori

```c
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int parse_int(const char *s, int *out)
{
    char *end;
    long  v;

    errno = 0;
    v = strtol(s, &end, 10);
    if (end == s || *end != '\0')      // nessuna cifra, o caratteri in più
        return (0);
    if (errno == ERANGE)               // fuori range per long
        return (0);
    if (v > INT_MAX || v < INT_MIN)    // entra in long ma non in int
        return (0);
    *out = (int)v;
    return (1);
}
```

Il controllo `v > INT_MAX` serve perché su sistemi dove `long` è a 64 bit, `strtol` non segnala errore per valori che sforano solo `int`.

### 9.3 Una `ft_atoi` sicura (stile 42)

Alla 42 le funzioni di libreria sono spesso vietate: devi scrivere la tua. Idea: accumuli in un tipo **più largo** (`long long`) e controlli a ogni cifra.

```c
int ft_atoi_safe(const char *s, int *out)
{
    long long n;
    int       sign;

    n = 0;
    sign = 1;
    while (*s == ' ' || (*s >= 9 && *s <= 13))
        s++;
    if (*s == '+' || *s == '-')
        if (*s++ == '-')
            sign = -1;
    if (*s < '0' || *s > '9')
        return (0);
    while (*s >= '0' && *s <= '9')
    {
        n = n * 10 + (*s++ - '0');
        if ((sign == 1 && n > INT_MAX) || (sign == -1 && -n < INT_MIN))
            return (0);
    }
    if (*s != '\0')
        return (0);
    *out = (int)(n * sign);
    return (1);
}
```

Perché `n * 10` non sfora: dopo ogni cifra, se `n` supera il limite la funzione esce subito. Quindi `n` non supera mai circa `2147483648` prima del `*10`, e `long long` (64 bit) lo contiene con enorme margine.

(Questo codice è solo illustrativo: non è formattato per rispettare tutte le regole della Norme, per esempio sul numero di righe o sulle dichiarazioni.)

---

## 10. Controllare l'overflow prima che avvenga

Il principio: **verifica l'operazione senza eseguirla**, usando solo operazioni che non possono sforare.

### 10.1 Addizione

```c
int safe_add(int a, int b, int *res)
{
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b))
        return (0);          // overflow
    *res = a + b;
    return (1);
}
```

`INT_MAX - b` con `b > 0` non sfora, e `INT_MIN - b` con `b < 0` non sfora: i controlli sono sicuri.

### 10.2 Sottrazione

```c
int safe_sub(int a, int b, int *res)
{
    if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b))
        return (0);
    *res = a - b;
    return (1);
}
```

### 10.3 Moltiplicazione (metodo semplice con tipo più largo)

```c
int safe_mul(int a, int b, int *res)
{
    long long r = (long long)a * b;

    if (r > INT_MAX || r < INT_MIN)
        return (0);
    *res = (int)r;
    return (1);
}
```

Funziona perché il prodotto di due `int` entra sempre in 64 bit. **Non** è applicabile a `long long * long long`: lì servono i controlli con divisione oppure i builtin.

### 10.4 Builtin del compilatore (GCC/Clang)

```c
int r;
if (__builtin_add_overflow(a, b, &r))
    /* overflow */;
```

Esistono anche `__builtin_sub_overflow` e `__builtin_mul_overflow`. Sono estensioni del compilatore, non C standard. Dal **C23** esiste lo standard `<stdckdint.h>` con `ckd_add`, `ckd_sub`, `ckd_mul`.

### 10.5 Se vuoi proprio il wraparound

Passa per gli unsigned, che lo garantiscono:

```c
int wrap_add(int a, int b)
{
    return ((int)((unsigned int)a + (unsigned int)b));
}
```

L'addizione è definita (modulo 2³²); la conversione finale a `int` è implementation-defined, ma con GCC/Clang dà il risultato "atteso" a complemento a due.

---

## 11. Strumenti: compilatore e sanitizer

Per **testare** il tuo codice in locale (non per la consegna, dove valgono le flag del subject):

| Flag | Cosa fa |
|---|---|
| `-fsanitize=undefined` | UBSan: rileva a runtime overflow signed, shift invalidi, divisioni per zero, ecc. e stampa un messaggio |
| `-fsanitize=address` | ASan: errori di memoria (utile se un overflow porta a scritture fuori buffer) |
| `-ftrapv` | fa abortire il programma su overflow signed (meno affidabile di UBSan) |
| `-fwrapv` | **definisce** il wraparound dei signed (cambia la semantica; non usarlo per nascondere bug) |
| `-Wconversion` | avvisa su conversioni che possono perdere valore |
| `-Wsign-conversion` | avvisa su conversioni signed/unsigned |
| `-Wsign-compare` | avvisa su confronti signed/unsigned (già in `-Wextra`) |

Esempio:

```bash
cc -Wall -Wextra -Werror -Wconversion -fsanitize=undefined main.c -o test
./test 2147483647
```

UBSan stampa qualcosa come `runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'`.

---

## 12. Applicazione al Rush00

Nel progetto:

```c
void rush(int x, int y);
```

- **Parte mandatory**: gli argomenti sono già `int`. Se il chiamante passa un valore fuori range, l'eventuale conversione è avvenuta *prima* di entrare in `rush`, ed è implementation-defined, non UB. Dentro `rush` basta gestire i valori non positivi.
- **Parte bonus con argv**: gli argomenti arrivano come **stringhe** (`char *`). Per ottenere degli `int` serve una conversione:
  - `atoi` **non è nelle funzioni permesse** (il subject ammette solo `write`);
  - anche se lo fosse, **non segnala errori** e per input oltre `INT_MAX` il comportamento è indefinito (su glibc: troncamento a 32 bit).
- Conseguenza pratica (su glibc):
  - `"2147483648"` → negativo → scartato dal controllo `<= 0`;
  - `"4294967296"` → `0` → scartato dal controllo `== 0`;
  - `"4294967297"` → `1` → **disegna un rettangolo largo 1**, output sbagliato e silenzioso.
- Soluzione: scrivere una `ft_atoi` che accumuli in `long long` e **rifiuti** gli input fuori range (sezione 9.3), poi stampare un errore o non disegnare nulla.

Un'ulteriore osservazione: anche con input validi, `rush(2147483647, 2147483647)` proverebbe a stampare ~4.6·10¹⁸ caratteri. Non è un problema di overflow, ma di tempo: il subject richiede di non andare in crash né in loop infinito, quindi decidi con i compagni un limite ragionevole e documentalo.

---

## 13. Esercizi con soluzioni

**Esercizio 1.** Cosa vale `x`?

```c
unsigned char x = 250;
x += 10;
```

<details><summary>Soluzione</summary>

`250 + 10 = 260` viene calcolato in `int` (promozione), poi convertito in `unsigned char`: `260 mod 256 = 4`. Definito dallo standard.
</details>

**Esercizio 2.** Questo codice è corretto?

```c
int a = INT_MAX;
if (a + 1 < 0)
    puts("overflow rilevato");
```

<details><summary>Soluzione</summary>

No: `a + 1` è UB. Il compilatore può assumere che non succeda e rimuovere il test. Il controllo corretto è `if (a > INT_MAX - 1)`, oppure usare `__builtin_add_overflow`.
</details>

**Esercizio 3.** Cosa stampa?

```c
int a = -1;
unsigned int b = 1;
printf("%d\n", a < b);
```

<details><summary>Soluzione</summary>

`0`. `a` viene convertito in `unsigned` (`UINT_MAX`), e `UINT_MAX < 1` è falso.
</details>

**Esercizio 4.** Categoria di comportamento (definito / implementation-defined / UB)?

1. `(int)3000000000L`
2. `UINT_MAX * 2`
3. `INT_MIN / -1`
4. `(unsigned char)-1`
5. `atoi("99999999999")`

<details><summary>Soluzione</summary>

1. Implementation-defined (GCC: `-1294967296`).
2. Definito (`UINT_MAX * 2 mod 2^32 = 4294967294`).
3. UB.
4. Definito (`255`).
5. UB (secondo lo standard).
</details>

**Esercizio 5.** Perché questo ciclo non termina mai?

```c
for (unsigned int i = 5; i >= 0; i--)
    printf("%u\n", i);
```

<details><summary>Soluzione</summary>

Un unsigned è sempre `>= 0`. Dopo `i == 0`, `i--` dà `UINT_MAX` e il ciclo continua. Serve un'altra forma, per esempio `for (unsigned i = 6; i-- > 0; )`.
</details>

**Esercizio 6.** Scrivi una funzione `int safe_neg(int a, int *res)` che neghi `a` senza UB.

<details><summary>Soluzione</summary>

```c
int safe_neg(int a, int *res)
{
    if (a == INT_MIN)
        return (0);
    *res = -a;
    return (1);
}
```
</details>

---

## 14. Cheat sheet finale

**Regole d'oro**

1. **Unsigned che sfora** → wraparound garantito (modulo 2ⁿ).
2. **Signed che sfora in un'operazione** → UB. Mai contare sul wraparound.
3. **Valore fuori range convertito a unsigned** → definito (modulo 2ⁿ).
4. **Valore fuori range convertito a signed** → implementation-defined (GCC/Clang: tiene i bit bassi).
5. **Float → intero fuori range** → UB.
6. Mescolare signed e unsigned della stessa dimensione → il signed diventa unsigned.
7. `atoi` non segnala errori e ha UB fuori range: usa `strtol` o una `ft_atoi` con controlli.
8. Per controllare un overflow: **verifica prima**, oppure usa un tipo più largo, oppure i builtin / `ckd_*`.
9. `INT_MIN` non ha un opposto positivo: attenzione a `-x`, `abs(x)`, `x / -1`.
10. Per testare: `-fsanitize=undefined`.

**Tabella finale**

| Situazione | Esito |
|---|---|
| `UINT_MAX + 1` | `0` (definito) |
| `INT_MAX + 1` | UB (di solito `INT_MIN`, ma non garantito) |
| `(unsigned char)300` | `44` (definito) |
| `(signed char)200` | implementation-defined (di solito `-56`) |
| `(int)4294967297L` | implementation-defined (GCC: `1`) |
| `INT_MIN / -1` | UB |
| `abs(INT_MIN)` | UB |
| `atoi("4294967297")` | UB per lo standard (glibc: `1`) |
| `-1 < 1u` | falso |
