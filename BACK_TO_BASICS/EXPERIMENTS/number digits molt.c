#include <stdio.h>

int digits_molt(int num) {
    if(num == 0)
        return 1;
    return (num % 10) * digits_molt(num / 10);
}

int main() {
    printf("%d\n", digits_molt(3456));
}