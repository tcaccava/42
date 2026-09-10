//questo e' il massimo livello di autoriferimento che un blocco di codice monolitico
// puo' raggiungere senza distinzione tra componente attiva e passiva
#include <stdio.h>
#define S "#include <stdio.h>\n#define S \nint main() {\n\tprintf(S);\n}\n"
int main() {
    printf(S);
}
