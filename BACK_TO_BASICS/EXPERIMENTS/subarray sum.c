#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//questo esercizio e' lo stesso di array subseq target sum,ma li ho stamapato tutte le
// sottosequenze e poi ho filtrato in fase di stampa solo quelle la cui somma fa il target
//qui invece e' la stessa riorsione a selezionare solo quelle corrette
int subarray_sum(int *a, int*tmp,int index, int target,int tindex, int len){
    if(target == 0)
        return 1;
    if(index == len)
        return 0;
    if(subarray_sum(a,tmp,index + 1, target,tindex,len))
        return 1;
    tmp[tindex] = a[index];
    if (subarray_sum(a,tmp,index + 1,target - tmp[tindex], tindex + 1,len))
        return 1;
    return 0;
}

int main()
{
    int a[4] = {1, 2, 3, 11};
    int tmp[4];
    printf("%d\n", subarray_sum(a,tmp, 0, 17, 0, 4));
}