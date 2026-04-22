#include <stdio.h>
#include <stdint.h>
#include "./popcount.c"
// conta i bytes accesi in un numero a 32 bit,il risultato e' noto come Hamming Weight dal nome dell'inventore dell'algoritmo
unsigned int ft_popcount_swar32(unsigned int n)
{
    unsigned int even_bytes = n & 0x55555555;//isolo i bit pari,un hex 55 e' 01010101,cioè 85
    unsigned int odd_bytes = n >> 1 & 0x55555555;//isolo i bit dispari,prima shifto a dx,poi applico la maschera
    unsigned int sum = odd_bytes + even_bytes;//con la somma ogni coppia di bit conterrà la somma di bit adiacenti,quindi sara 00,01 o 2,cioe' 10
    sum = (sum & 0x33333333) + ( sum >> 2 & 0x33333333);//ripeto il metodo su coppie di bit,il 33 della maschera e' 00110011,cioè 51;ora ogni nibble può valere da 0 a 4
    sum = (sum & 0x0F0F0F0F) + (sum >> 4 & 0X0F0F0F0F);//stesso sui nibble di ogni singolo byte;ora ogni byte contiene da 0 a 8
    sum = (sum & 0X00FF00FF) + (sum >> 8 & 0x00FF00FF);//isolo i singoli bytes,ora ogni meta' word contiene da 0 a 16
    sum = (sum & 0x0000FFFF) + (sum >> 16 & 0x0000FFFF);//isolo le coppie di bytes,ottengo il popcount
    return sum;
}

int main()
{
    printf("%d\n", ft_popcount_swar32(0xFFFFFFFF));
    printf("%d\n", ft_popcount_swar32(0x0000FFFF));

}