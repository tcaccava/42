#ifndef CAT_H
#define CAT_h

#include <unistd.h> //write,read,close
#include <fcntl.h>  //open
#include <errno.h>  //errno
#include <libgen.h> //basename
#include <string.h> //strerror
#include <stdio.h>  //perror

void read_stdin();
void put_str(char *s);
void print_error(char *av);
void display_file(char *av);
void cat(int c, char **av);

#endif