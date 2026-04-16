#include <stdio.h>
//moltiplica usando solo bitwise e addizione
//per ogni bit di bit,se acceso,applica ad a uno shift a sinistra corrispondente
//per esempio se a e' 5 e b e' 11,cioe 0101 e 1011,prende i 3 bit accesi di 11,cioe bit 0,1,3
//e somma a result gli shift corrispondenti di a,cioe a << 0,a << 1, a << 3,quindi 5,10,40,ottenendo 55
unsigned int ft_multiply_no_operator(unsigned int a, unsigned int b){
    unsigned int result = 0;
    for(int i = 0;i < 32; i++)
        result += (a << i) & -(b >> i & 1);
    return result;
}
//alternativa piu' complessa e meno elegante
// unsigned int ft_multiply_no_operator(unsigned int a, unsigned int b)
// {
//     unsigned int count_shift = 0;
//     for (int i = 0; i <= 7; i++)
//     {
//         count_shift += (b >> i & 1) * i;
//     }
//     unsigned int res = a << count_shift;
//     unsigned int factor = 1;
//     while (count_shift--)
//         factor *= 2;
//     while (factor != b)
//     {
//         if (factor < b)
//         {
//             res += a;
//             factor += 1;
//         }
//         else
//         {
//             res -= a;
//             factor -= 1;
//         }
//     }
//     return res;
// }

int main()
{
    printf("%u %u %u %u\n", ft_multiply_no_operator(2, 5), ft_multiply_no_operator(10, 6), ft_multiply_no_operator(9, 3), ft_multiply_no_operator(8, 4));
}