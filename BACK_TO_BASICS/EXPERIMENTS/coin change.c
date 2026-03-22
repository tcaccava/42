#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int coins(int *a,int size,int index,int target){
    if(target == 0)
        return 1;
    if(target < 0 || index == size)
        return 0;
    int skip = coins(a,size,index + 1,target);//non lo prendo
    int take = coins(a,size,index, target - a[index]);//lo prendo
    return skip + take;

}


int main()
{
    int a[] = {1, 2, 3};
    int tmp[100];
    printf("%d\n", coins(a, 3, 0,10));
}