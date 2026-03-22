#include "tail.h"
// tail e' una system call che,quando usata con il parametro -n num,legge e stampa le ultime
//  num linee di un file,mentre con il -c legge gli ultimi num byte di un file

int str_cmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

void put_str(char *s)
{
    while (*s)
        write(2, s++, 1);
    write(2, "\n", 1);
}

void print_error(char *av)
{
    write(2, "tail: ", 6);
    char *file_name = basename(av);
    write(2, file_name, strlen(file_name));
    write(2, ": ", 2);
    put_str(strerror(errno));
}
void tail(char *file, int last_bytes, int length)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        print_error(file);
        return;
    }
    ssize_t bytes_read = 0;
    char buf[4096];
    int count = 0;
    int start = (last_bytes > length) ? 0 : length - last_bytes;
    while ((bytes_read = read(fd, buf, 1)) > 0)
    {
        if (count + bytes_read <= start)
        {
            // tutto il blocco prima dello start → ignora
        }
        else if (count < start && count + bytes_read > start)
        {
            // blocco contiene lo start
            int offset = start - count;
            write(1, buf + offset, bytes_read - offset);
        }
        else
        {
            // blocco tutto dopo lo start
            write(1, buf, bytes_read);
        }
        count += bytes_read;
    }
    if (bytes_read == -1)
        print_error(file);

    close(fd);
}

int calc_length(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        print_error(file);
        return -1;
    }
    ssize_t bytes_read = 0;
    char buf[4096];
    int count = 0;
    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0)
        count += bytes_read;
    if (bytes_read == -1)
    {
        print_error(file);
        return -1;
    }
    close(fd);
    return count;
}

int main(int c, char **v)
{
    if (c != 4)
        return 1;
    if (str_cmp(v[1], "-c") != 0)
        return 1;
    int last_bytes = atoi(v[2]);
    if (last_bytes < 0)
        last_bytes = 0;
    int length = calc_length(v[3]);
    tail(v[3], last_bytes, length);
    return 0;
}