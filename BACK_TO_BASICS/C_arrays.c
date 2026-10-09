/*A livello hardware e logico, in C esiste **un solo** tipo di array: un blocco di memoria contiguo, fortemente tipizzato, in cui l'accesso agli elementi avviene tramite aritmetica dei puntatori (`arr[i]` è letteralmente `*(arr + i)`).

Tuttavia, quando si parla di "tipi" di array in C, si fa riferimento a due tassonomie distinte che spesso vengono confuse tra loro: **dove** viene allocata la memoria (Storage Duration) e **quando** viene calcolata la sua dimensione.

Ecco la classificazione reale, nuda e cruda, a livello di sistema operativo e compilatore.

### 1. Array Automatici (Sullo Stack)

Sono i classici array dichiarati all'interno di una funzione.

```c
void foo() {
    int arr[10];
}

```

* **Dove vivono:** Nello Stack frame della funzione.
* **Come vengono allocati:** Il compilatore trasforma quella dichiarazione in una singola istruzione Assembly: `sub rsp, 40` (su x86-64, 10 interi * 4 byte). L'allocazione ha un costo computazionale pari a zero cicli di clock reali.
* **Ciclo di vita:** Nascono e muoiono con lo scope della funzione. Appena c'è una `ret`, il registro `RSP` torna su e la memoria viene implicitamente invalidata.
* **Limite:** La dimensione deve essere nota a compile-time (costante).

### 2. Array Dinamici (Sull'Heap)

Sono blocchi di memoria allocati a runtime tramite la libc (`malloc`, `calloc`, `realloc`).

```c
void foo(size_t n) {
    int *arr = malloc(n * sizeof(int));
}

```

* **Dove vivono:** Nell'Heap, un'area di memoria non strutturata gestita dall'allocatore spaziale del SO (es. `glibc`).
* **Come vengono allocati:** A basso livello richiedono system call (`brk` o `mmap` se superano una certa soglia, tipicamente 128KB). Sono infinitamente più lenti da allocare rispetto allo Stack.
* **Ciclo di vita:** Esistono finché non chiami esplicitamente `free(arr)`. Se perdi il puntatore, generi un memory leak.
* **Vantaggio:** La dimensione è determinata a runtime e può cambiare (tramite `realloc`). Non sei limitato dagli ~8MB tipici dello Stack, ma solo dalla RAM/Swap disponibile.

### 3. Array Statici (Segmento Dati)

Sono array dichiarati a livello globale, oppure localmente con la keyword `static`.

```c
int global_arr[10]; // Statico (Globale)

void foo() {
    static int static_arr[10] = {1, 2, 3}; // Statico (Locale)
}

```

* **Dove vivono:** Non vanno mai né sullo Stack né sull'Heap. Vengono "stampati" fisicamente all'interno del file eseguibile (ELF). Se inizializzati, finiscono nella sezione `.data`. Se non inizializzati (o inizializzati a zero), finiscono nella sezione `.bss` (che pesa 0 byte sul disco, ma viene mappata a zero dal kernel all'avvio del processo).
* **Ciclo di vita:** Esistono per tutta la durata del programma. Il loro indirizzo di memoria è assoluto (o calcolato in via relativa tramite `RIP-relative addressing`).

### 4. VLA - Variable Length Arrays (Stack Dinamico)

Introdotti nello standard C99 e resi opzionali nel C11.

```c
void foo(int n) {
    int arr[n]; // n non è noto a compile-time
}

```

* **Cosa sono:** Sono una bestia ibrida. La dimensione viene calcolata a runtime (come un array dinamico), ma la memoria viene allocata sullo Stack (come un array automatico).
* **Dietro le quinte:** Il compilatore emette istruzioni per sottrarre dal registro `RSP` un valore calcolato dinamicamente durante l'esecuzione della funzione, mantenendo la base dello stack frame su `RBP` per non perdere i riferimenti alle variabili locali.
* **Perché sono odiati:** Se l'utente passa un valore `n` gigantesco, il programma va in Segfault immediato perché buca lo Stack, rendendo impossibile gestire l'errore (a differenza di `malloc` che restituirebbe placidamente `NULL`). Linux Torvalds li ha banditi dal kernel Linux nel 2018 per problemi di sicurezza e performance.

---

### In sintesi: Cosa si intende per "Statico" vs "Dinamico"

Quando in C usi questi due termini, la nomenclatura può riferirsi a due cose diverse.

**1. Riferito alla Dimensione (Binding temporale)**

* **Statico:** La dimensione dell'array è fissata a tempo di compilazione. Il compilatore sa esattamente quanti byte servono prima ancora che il programma parta.
* **Dinamico:** La dimensione è nota solo a runtime (tramite `malloc` o VLA), in base a input dell'utente, file o variabili non predicibili.

**2. Riferito alla Storage Duration (Memoria)**

* **Statico:** Il dato vive in `.data` / `.bss`. Sopravvive a ripetute chiamate della funzione in cui si trova, mantenendo il suo valore originale (motivo per cui le funzioni con variabili `static` all'interno falliscono clamorosamente il test di rientranza in contesti multithreading).
* **Dinamico:** Il dato vive nell'Heap, slegato da logiche di scope del codice. Esiste nel far west della memoria virtuale finché il programmatore non ne decreta la morte con la `free`.                               
*/