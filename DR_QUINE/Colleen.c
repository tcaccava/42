#include <stdio.h>
// it is math proven that there is no way to write a monolitic source code that works as a quine

void call_printf(char *s)
{
    printf(s, 10, 34, s);
}

int main()
{
    char *s = "#include <stdio.h>%1$c// it is math proven that there is no way to write a monolitic source code that works as a quine%1$c%1$cvoid call_printf(char *s)%1$c{%1$c    printf(s, 10, 34, s);%1$c}%1$c%1$cint main()%1$c{%1$c    char *s = %2$c%3$s%2$c;%1$c    // the only way to build a quine is the separation of the active and passive components%1$c    call_printf(s);%1$c}%1$c";
    // the only way to build a quine is the separation of the active and passive components
    call_printf(s);
}
