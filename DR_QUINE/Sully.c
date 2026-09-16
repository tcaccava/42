#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    static unsigned short runs = 0;
    char filename[] = "Sully_ .c";
    if(runs >= 0)
        filename[6] = (unsigned char)runs;
    else 
        return 0; 
    int file = open(filename, O_CREAT, 0777);
    if(file < 0)
        printf("Error opening the file");
    pid_t child = fork();
    char **env;
    char* src_code = "";
    char* output = "Sully";
    if(child < 0)
        env = getenv(child);
    if(child > 0){
        execve("/usr/bin/gcc",(char *[]){"gcc","-Wall","-Wextra","-Werror",src_code, "-o", output, NULL},env);
        
    int exit_status;
    waitpid(child,&exit_status,0);

    
    
}