#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main() {
    int num = 5;
    char arr[5]= "tobi";
    char * str = "tobia";
    arr[0] = 'm';
    str[0] = 'f';
    str = "gianluca";
    char *name = malloc(6);
    strlcpy(name, "tobia", 6);
}