#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int digits_occ(int num, int target)
{
    int occurrence = 0;
    if (num == 0)
        return 0;
    if (num % 10 == target)
        occurrence++;
    occurrence += digits_occ(num / 10, target);
    return occurrence;
}

int main()
{
    printf("%d\n", digits_occ(773277707, 7));
}