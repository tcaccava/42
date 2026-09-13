// useless comment
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define STR "// useless comment%1$c#include <stdio.h>%1$c#include <fcntl.h>%1$c#include <unistd.h>%1$c#define STR %2$c%3$s%2$c%1$c#define CMD do {int fd = open(%2$cGrace_kid.c%2$c, O_WRONLY | O_CREAT | O_TRUNC, 0644); if (fd >= 0) dprintf(fd, STR, 10, 34, STR); close(fd);} while (0)%1$c#define EXEC int main() {CMD;}%1$cEXEC%1$c"
#define CMD do {int fd = open("Grace_kid.c", O_WRONLY | O_CREAT | O_TRUNC, 0644); if (fd >= 0) dprintf(fd, STR, 10, 34, STR); close(fd);} while (0)
#define EXEC int main() {CMD;}
EXEC
