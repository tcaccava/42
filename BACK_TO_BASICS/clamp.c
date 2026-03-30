#include <stdio.h>
unsigned char clamp(unsigned char n, unsigned char min, unsigned char max)
{
    unsigned char minimo = n ^ ((n ^ min) & -(n < min));
    // se n e' minore di min,vogliamo min,altrimenti n;se n < min e' vero,quindi 1, e -1 equivale a 0xff,cioe 255,quindi una maschera
    // di soli 1,mentre se n e' maggiore e quella condizione e' falsa,allora genera una maschera di tutti 0
    // per quanto riguarda il primo pezzo della formula,per definizione a ^ (a ^ b) restituisce b,quindi
    // se la maschera di prima e' zero e quindi min e' minore avremo n ^ 0,cioe' n,mentre se la maschera e' 0xff
    // e quindi n e' minore di min,allora avremo n ^(n ^ min) & 0xff,cioe n ^(n ^ min),cioe n
    // in sostanza se n e'minore di min la maschera e' 0xFF e restituisce min,altrimenti la maschera e' 0x00 e restituisce n
    unsigned char massimo = minimo ^ ((minimo ^ max) & -(minimo > max));

    // adesso minimo vale n oppure min — mai sotto min
    // se minimo > max vogliamo max, altrimenti minimo
    // minimo > max → maschera 0xFF → max ^ (max ^ minimo) = minimo... no
    // vogliamo max quando minimo > max, quindi partiamo da minimo:
    // minimo > max → maschera 0xFF → minimo ^ (minimo ^ max) = max
    // minimo <= max → maschera 0x00 → minimo ^ 0 = minimo

    return massimo;
}
// La regola generale è questa:
// XOR è uno scambio condizionale.
// a ^ (a ^ b) restituisce sempre b — perché XOR è reversibile. Moltiplicando per una maschera 0x00 o 0xFF decidi se applicare o no lo scambio:
// a ^ ((a ^ b) & 0xFF) = b   // scambia
// a ^ ((a ^ b) & 0x00) = a   // non scambia
// La maschera la generi con -(condizione) — che vale 0xFF se la condizione è vera, 0x00 se è falsa.
// Quindi ogni volta che hai un if (condizione) return x; else return y puoi scriverlo come:
// y ^ ((y ^ x) & -(condizione))
// Questo è il pattern branchless universale — senza if, senza salti, una sola espressione.
int main(){
    printf("%d\n",clamp(100, 50, 200));
    printf("%d\n",clamp(20, 50, 200));
    printf("%d\n",clamp(250, 50, 200));
}