#include <stdio.h>
#include "./keep_highest_bit.c"
unsigned int ft_divide_no_operator(unsigned int a, unsigned int b)
{
    unsigned int result = 0;
    if (b == 0)
        return 0;
    int i = 0;
    for (; i < 31 && a >= (b << i); i++)
        ; // trovo il piu' piccolo multiplo di b appena superiore ad a
    i--;  // diminuisco i perche' e' uscito dal ciclo dopo l'ultimo i++
    // a questo punto vado a ritroso
    for (; i >= 0; i--)
    {
        result |= ((1U << i) & -(a >= (b << i))); // accendo i bit del risultato
        a -= ((b << i) & -(a >= (b << i)));       // sottraggo ad a quello che ho aggiunto al risultato
    }
    return result;
}

// int count_leading_zeros(unsigned int a)
// {
//     int i = 31;
//     for (; i >= 0 && !(a << i & 1); i--)
//         ;
//     return i;
// }

// unsigned int ft_divide_no_operator(unsigned int a, unsigned int b)
// {
//     unsigned int result = 0;
//     if (!b)
//         return 0;
//     // con questa soluzione di appoggio,che replica quanto fa la istruzione builtin__clz,
//     // individuo per entrambi i numeri la posizione del MSB,e sottraggo le due posizioni
//     // cosi non ho bisogno di ciclare sull'intero b
//     // il resto e' identico alla precedente
//     int i = (31 - count_leading_zeros(a)) - (31 - count_leading_zeros(b));
//     for (; i >= 0; i--)
//     {
//         result |= ((1U << i) & -(a >= (b << i))); // accendo i bit del risultato
//         a -= ((b << i) & -(a >= (b << i))); 
//     }
//     return result;
// }

// quindi per esempio se ho 37 : 7,prima trovo il piu piccolo multiplo di b superiore a 37,cioe 56 con i = 3,
// poi riduco i a 2,cioe 28 e quindi rientro sotto a,e a questo punto sottraggo ad a tutti i multipli di b inferiori ad a decrescente,
// quindi 37 - (7 << 2),cioe 9,e faccio OR di result con 1 shiftato di i
// questa versione e' molto piu ottimizzata delle seguenti perche prima trova i,e poi va a ritroso

// versione molto inefficiente,trova il primo multiplo di b maggiore di a,ma poi scala b dal resto,il che pue' richiedere centinaia di iterazioni
//  unsigned int ft_divide_no_operator(unsigned int a, unsigned int b)
//  {
//      int i = 0;
//      for(; a >= (b << i) && i < 32; i++);
//      unsigned int result = (1 << --i);
//      unsigned int carry = a - (b << i);
//      while (carry >= b)
//      {
//          result++;
//          carry -= b;
//      }
//      return result;
//  }

// versione mediamente efficiente,a differenza di quella di sopra itera su tutti e 32 i bit per trovare result,ma non ha la complicazione del carry
//  unsigned int ft_divide_no_operator(unsigned int a, unsigned int b)
//  {
//      unsigned int result = 0;
//      int i = 31;

//     if (b == 0)
//         return 0;

//     while (i >= 0)
//     {
//         if ((b << i) <= a)
//         {
//             a -= (b << i);
//             result |= (1U << i);
//         }
//         i--;
//     }
//     return result;
// }

int main()
{
    printf("%d %d %u\n", ft_divide_no_operator(10, 2), ft_divide_no_operator(16, 3), ft_divide_no_operator(10000, 25));
}