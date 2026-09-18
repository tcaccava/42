#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(int argc,char **argv,char ** envp)
{
    static unsigned short runs = 0;
    if(runs > 5)
        return 0;
    char filename[] = "Sully_ .c";
    filename[6] = (unsigned char)(runs + 48);
    int file = open(filename, O_CREAT, 0777);
    if (file < 0)
        printf("Error opening the file");
    pid_t pid = fork();
    char **env;
    char *src_code = "#include <stdio.h>%1$c#include <unistd.h>%1$c#include <fcntl.h>%1$c#include <stdlib.h>%1$c#include <sys/wait.h>%1$c#include <sys/types.h>%1$cint main()%1$c{    static unsigned short runs = 0;%1$c    if(runs > 5)%1$c        return 0;%1$c    char filename[] = %2$cSully_ .c%2$c;%1$c    filename[6] = (unsigned char)(runs + 48);%1$c    int file = open(filename, O_CREAT, 0777);%1$c    if (file < 0)%1$c        printf(%2$cError opening the file%2$c);%1$c    pid_t child = fork();%1$c    char **env;%1$c    char *src_code = %2$c%3$s%2$c;%1$c    char *output = (runs == 0) ? %2$cSully%2$c : %2$cSully_ %2$c;%1$c    if (runs >= 0)%1$c        output[6] = (unsigned char)(runs + 48);%1$c    if (child < 0)%1$c        env = getenv(child);%1$c    int exit_status;%1$c    if (child > 0)%1$c    {%1$c        execve(%2$c/usr/bin/gcc%2$c, (char *[]){%2$cgcc%2$c, %2$c-Wall%2$c, %2$c-Wextra%2$c, %2$c-Werror%2$c, src_code, %2$c-o%2$C, output, NULL}, env);%1$c        waitpid(child, &exit_status, 0);%1$c    }%1$c    if (WIFEXITED(exit_status))    {%1$c        int exit_code = WEXITSTATUS(exit_status);%1$c        if (exit_code == 0) {%1$c            runs += 1;%1$c            main()%1$c        };%1$c    }%1$c}";
    char *output = (runs == 0) ? "Sully" : "Sully_ ";
    if (runs > 0)
        output[6] = (unsigned char)(runs + 48);
    int exit_status;
    if(pid == 0){
        execve("/usr/bin/gcc", (char *[]){"gcc", "-Wall", "-Wextra", "-Werror", src_code, "-o", output, NULL}, envp);
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