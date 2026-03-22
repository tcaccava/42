#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int max_subarray(int *a, int start,int end){
    int max;
    if(start == end)
        return a[start];
    max = max_subarray(a,start,end -1);
    return (a[end] > max) ? a[end] : max;
}

int main() {
    int a[] = {14,9,7,13,25,2,43};
    printf("%d\n", max_subarray(a,2,6));
}