#include "hexdump.h"
void write_hex_byte(unsigned char c)
{
    char *hex = "0123456789abcdef";
    char out[2];

    out[0] = hex[c >> 4];
    out[1] = hex[c & 0x0F];
    write(1, out, 2);
}

void write_offset(unsigned long offset)
{
    char hex[16] = "0123456789abcdef";
    char buf[8];
    int i = 7;

    while (i >= 0)
    {
        buf[i] = hex[offset & 0x0F];
        offset >>= 4;
        i--;
    }
    write(1, buf, 8);
}

void print_line(unsigned char *buf, ssize_t n, unsigned long offset)
{
    ssize_t i;

    write_offset(offset);
    write(1, "  ", 2);

    i = 0;
    while (i < 16)
    {
        if (i < n)
            write_hex_byte(buf[i]);
        else
            write(1, "  ", 2);
        if (i % 2)
            write(1, " ", 1);
        i++;
    }

    write(1, "|", 1);
    i = 0;
    while (i < n)
    {
        if (buf[i] >= 32 && buf[i] <= 126)
            write(1, &buf[i], 1);
        else
            write(1, ".", 1);
        i++;
    }
    write(1, "|\n", 2);
}

int main(int ac, char **av)
{
    int fd;
    unsigned char buf[16];
    ssize_t n;
    unsigned long offset;

    if (ac != 3 || strcmp(av[1], "-C") != 0)
        return (1);

    fd = open(av[2], O_RDONLY);
    if (fd == -1)
    {
        write(2, "hexdump: ", 9);
        write(2, basename(av[2]), strlen(basename(av[2])));
        write(2, ": ", 2);
        write(2, strerror(errno), strlen(strerror(errno)));
        write(2, "\n", 1);
        return (1);
    }

    offset = 0;
    while ((n = read(fd, buf, 16)) > 0)
    {
        print_line(buf, n, offset);
        offset += n;
    }

    write_offset(offset);
    write(1, "\n", 1);
    close(fd);
    return (0);
}
