#include <stdio.h>
#include <unistd.h>
void call_me(){
    write(1,"tobia",5);
}

int main() {
    char *previous = "#include <stdio.h>\n#include <unistd.h>\nvoid call_me(){\n\twrite(1,\22tobia\22,5);\n}\n";
    char *main = "int main() {\n\tchar *previous = ";
    printf(" %s", passive,passive);
}