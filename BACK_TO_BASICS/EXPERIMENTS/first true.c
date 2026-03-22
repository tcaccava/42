#include <stdio.h>

//trova in primo elemento true in un array di boolean

int first_true(int *a, int size, int index){
    if(index == size)
        return -1;
    if(a[index])
        return index;
    return first_true(a,size, index + 1);
}
int main() {
    int a[] = {0,0,0,0,0,0,1,0,1,0,1};
    printf("%d\n", first_true(a,11,0));
}