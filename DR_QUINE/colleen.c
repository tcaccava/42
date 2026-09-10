#include <stdio.h>

int main()
{
    char*s = "#include <stdio.h>%1$c%1$cint main()%1$c{%1$c    char*s = %2$c%3$s%2$c;%1$c    printf(s,10,34,s);%1$c}%1$c";
    printf(s,10,34,s);
}