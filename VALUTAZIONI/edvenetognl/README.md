*This project has been created as part of the 42 curriculum by edveneto.*

## Description
L'obiettivo di questo progetto è programmare una funzione in C, denominata `get_next_line`, che restituisca una riga letta da un file descriptor (fd). Chiamate ripetute alla funzione permettono di leggere il file di testo puntato dal file descriptor sequenzialmente, una riga alla volta. Oltre a fornire una funzione estremamente utile da aggiungere alla propria libreria, questo progetto introduce e consolida un concetto fondamentale della programmazione in C: l'uso delle variabili statiche.

## Instructions
Il progetto deve essere compilato includendo un flag specifico che determina la quantità di byte letti per ogni chiamata di sistema.

* **File della parte obbligatoria:** `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`.
* **Compilazione:** Compila il codice aggiungendo l'opzione `-D BUFFER_SIZE=n` al tuo compilatore. Questo definisce la dimensione del buffer utilizzata internamente dalla funzione `read()`.
* **Esempio di comando:** `cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 <files>.c`.

## Resources
* **Utilizzo dell'IA:** Utilizzata per approfondimenti, comprenzione degli argomenti e revisione norma.

## Algorithm
L'algoritmo si basa sull'utilizzo di una variabile statica per preservare il testo in eccesso letto dal file descriptor tra una chiamata e l'altra della funzione.

1. **Lettura e accumulo:** La funzione utilizza `read()` per estrarre blocchi di dati (della grandezza definita da `BUFFER_SIZE`) dal file descriptor, unendoli al contenuto della variabile statica. L'obiettivo è leggere il meno possibile ad ogni chiamata, interrompendo la lettura non appena viene incontrato un carattere di *new line* (`\n`).
2. **Estrazione della riga:** Non appena viene identificato un `\n` all'interno della stringa accumulata, la funzione estrae la porzione di testo fino a quel carattere (incluso il `\n` stesso) per preparare il valore di ritorno.
3. **Aggiornamento dello stato:** I caratteri rimanenti dopo il `\n` vengono salvati nuovamente nella variabile statica per essere utilizzati dalla successiva chiamata di `get_next_line()`.
4. **Gestione della fine del file o degli errori:** Se `read()` restituisce 0 (nessun altro dato da leggere) o si verifica un errore, la funzione libera la memoria allocata e restituisce `NULL`. Nel caso della fine del file, la stringa viene restituita senza il carattere `\n` se il file originale non termina in quel modo. 
