#include <stdio.h>

int are_base64_chars(unsigned char *src, size_t src_len)
{
    while (src_len--)
        if (!((*src >= 65 && *src <= 90) || (*src >= 97 && *src <= 122) || (*src >= 48 && *src <= 57) || (*src == '+') || (*src == '/')))
            return 0;
    return 1;
}

int rec_padding(unsigned char *src, size_t src_len){
    if(src[src_len - 1] == '=' && src[src_len - 2] == '=')
        return 2;
    else if(src[src_len - 1] == '=')
        return 1;
    else 
        return 0;
}



int base64_decode(unsigned char *src, size_t src_len, unsigned char *dst)
{
    if(!(are_base64_chars(src,src_len)))
        return -1;
    if (src_len & 3)
        return -2;
    
}