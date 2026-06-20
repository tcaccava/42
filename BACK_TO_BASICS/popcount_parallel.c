#include <stdio.h>
#include "./popcount_swar32.c"
// Applica popcount_swar32 a un intero array in parallelo, salvando i risultati. Poi calcola la somma totale di tutti i popcount con SWAR su uint64_t processando due elementi alla volta.
long popcount_parallel(unsigned int *arr, int n, unsigned int *results) {
    int i = 0;
    for(; i < n; i++)
        results[i] = popcount_swar32(arr[i]);
    uint64_t res = 0;
    while(i){
        uint64_t temp = results[i--];
        temp = (temp << 32) | results[i--];
        res += temp; 
    }
    return (res & 0xFFFF) + (res & 0x0000FFFF);
    
}

int main() {
    unsigned int arr[] = {8,16,32,64};
    unsigned int results[sizeof(arr)];
    printf("%l\n", popcount_parallel(arr,4,results));
}
