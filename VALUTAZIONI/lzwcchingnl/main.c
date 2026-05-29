#include "get_next_line_bonus.h"
# include <fcntl.h>
#include <stdio.h>
int main(int argc, char** argv)
{
    int fd = 0;
    char *str;
    int fd2 = 0;
    char *str2;
    fd = open(argv[1], O_RDONLY);
    fd2 = open(argv[2], O_RDONLY);
    str = get_next_line(fd);
    str2 = get_next_line(fd2);
    while (str || str2) {
        if (str) {
            printf("%s", str);
            str = get_next_line(fd);
        }
        if (str2) {
            printf("%s", str2);
            str2 = get_next_line(fd2);
        }
    }
    close (fd);
    close (fd2);
    return 0;
}