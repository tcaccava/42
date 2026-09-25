#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    static short runs = 5;
    int is_the_real_sully = !(strcmp(__FILE__, "Sully.c"));
    if (!is_the_real_sully)
        runs--;
    if (runs < 0)
        return 0;
    char filename[] = "Sully_ .c";
    filename[6] = (unsigned char)(runs + 48);
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (file < 0)
        return 1;
    char *src_code = "#include <stdio.h>%1$c#include <unistd.h>%1$c#include <fcntl.h>%1$c#include <stdlib.h>%1$c#include <sys/wait.h>%1$c#include <sys/types.h>%1$c#include <string.h>%1$c%1$cint main(int argc, char **argv, char **envp)%1$c{%1$c    (void)argc;%1$c    (void)argv;%1$c    static short runs = %4$d;%1$c    int is_the_real_sully = !(strcmp(__FILE__, %2$cSully.c%2$c));%1$c    if (!is_the_real_sully)%1$c        runs--;%1$c    if (runs < 0)%1$c        return 0;%1$c    char filename[] = %2$cSully_ .c%2$c;%1$c    filename[6] = (unsigned char)(runs + 48);%1$c    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);%1$c    if (file < 0)%1$c        return 1;%1$c    char *src_code = %2$c%3$s%2$c;%1$c    dprintf(file, src_code, 10, 34, src_code, runs);%1$c    close(file);%1$c    char exe_name[10] = %2$c./Sully_ %2$c;%1$c    exe_name[8] = (unsigned char)(runs + 48);%1$c    pid_t pid_gcc = fork();%1$c    if (!pid_gcc)%1$c    {%1$c        execve(%2$c/usr/bin/gcc%2$c, (char *[]){%2$cgcc%2$c, filename, %2$c-o%2$c, exe_name + 2, NULL}, envp);%1$c        exit(1);%1$c    }%1$c    waitpid(pid_gcc, NULL, 0);%1$c    if (!fork())%1$c    {%1$c        char *exec_args[] = {exe_name, NULL};%1$c        execve(exe_name, exec_args, envp);%1$c        exit(1);%1$c    }%1$c    wait(NULL);%1$c    return 0;%1$c}%1$c";
    dprintf(file, src_code, 10, 34, src_code, runs);
    close(file);
    char exe_name[10] = "./Sully_ ";
    exe_name[8] = (unsigned char)(runs + 48);
    pid_t pid_gcc = fork();
    if (!pid_gcc)
    {
        execve("/usr/bin/gcc", (char *[]){"gcc", filename, "-o", exe_name + 2, NULL}, envp);
        exit(1);
    }
    waitpid(pid_gcc, NULL, 0);
    if (!fork())
    {
        char *exec_args[] = {exe_name, NULL};
        execve(exe_name, exec_args, envp);
        exit(1);
    }
    wait(NULL);
    return 0;
}
