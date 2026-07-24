#include <stdio.h>

// Codifica n usando la rappresentazione di Zeckendorf: ogni intero positivo si scrive come somma di numeri di Fibonacci non consecutivi, 
// in modo unico. Il bit i del risultato vale 1 se il numero di Fibonacci F(i+2) fa parte della somma. 
// Esempio: n=11 = 8+3 = F(6)+F(4) → bit 4 e bit 2 accesi → 0b010100. Usato nella compressione di interi in database.

unsigned int fibonacci_encode(unsigned int n) 