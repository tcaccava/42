#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc,char **argv,char **envp)
{
    static unsigned short runs = 1;
    if(runs > 5)
        return 0;
    char filename[] = "Sully_ .c";
    filename[6] = (unsigned char)(runs + 48);
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (file < 0)
        printf("Error opening the file");
    char *src_code = "#include <stdio.h>%1$c#include <unistd.h>%1$c#include <fcntl.h>%1$c#include <stdlib.h>%1$c#include <sys/wait.h>%1$c#include <sys/types.h>%1$c%1$cint main(int argc,char **argv,char **envp)%1$c{%1$c    static unsigned short runs = %4$d;%1$c    if(runs > 5)%1$c        return 0;%1$c    char filename[] = %2$cSully_ .c%2$c;%1$c    filename[6] = (unsigned char)(runs + 48);%1$c    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);%1$c    if (file < 0)%1$c        printf(%2$cError opening the file%2$c);%1$c    char *src_code = %2$c%3$s%2$c;%1$c    dprintf(file,src_code,10,34,src_code,runs);%1$c    pid_t pid = fork();%1$c    char output[8] = %2$cSully%2$c;%1$c    if (runs > 0)%1$c    {%1$c        output[5] = '_';%1$c        output[6] = (unsigned char)(runs + 48);%1$c        output[7] = 0;%1$c    }%1$c    int exit_status;%1$c    if (pid == 0)%1$c    {%1$c        execve(%2$c/usr/bin/gcc%2$c, (char *[]){%2$cgcc%2$c, filename, %2$c-o%2$c, output, NULL}, envp);%1$c        exit(1);%1$c    }%1$c    else if (pid > 0)%1$c        waitpid(pid, &exit_status, 0);%1$c    if (WIFEXITED(exit_status))%1$c    {%1$c        int exit_code = WEXITSTATUS(exit_status);%1$c        if (exit_code == 0)%1$c        {%1$c            runs += 1;%1$c            main(argc,argv,envp);%1$c        }%1$c    }%1$c}%1$c";
    dprintf(file,src_code,10,34,src_code,runs);
    pid_t pid = fork();
    char output[8] = "Sully";
    if (runs > 0)
    {
        output[5] = '_';
        output[6] = (unsigned char)(runs + 48);
        output[7] = 0;
    }
    int exit_status;
    if (pid == 0)
    {
        execve("/usr/bin/gcc", (char *[]){"gcc", filename, "-o", output, NULL}, envp);
        exit(1);
    }
    else if (pid > 0)
        waitpid(pid, &exit_status, 0);
    if (WIFEXITED(exit_status))
    {
        int exit_code = WEXITSTATUS(exit_status);
        if (exit_code == 0)
        {
            runs += 1;
            main(argc,argv,envp);
        }
    }
}
