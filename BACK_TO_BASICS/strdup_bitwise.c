#include <stdio.h>
#include "./memcpy_bitwise_swar.c"
#include "./strlen_bitwise.c"
#include <stdlib.h>

char * ft_strdup(const char *s){
    size_t len = strlen_hex(s);
    char *dup = malloc(len + 1);
    if(!dup)
        return NULL;
    ft_memcpy((unsigned char *)dup, (unsigned char *)s, len);
    dup[len] = 0;
    return dup;
}


int main () {
    printf("%s\n", ft_strdup("qrfpuhrofrofreoghreogerogroeigwroiegoirgjoirgj"));
}