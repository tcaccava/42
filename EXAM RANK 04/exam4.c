#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int err(char *s)
{
	while(*s)
		write(2, s++, 1);
	return 1;
}
int cd(int c, char **v)
{
	if(c != 2)
		return(err("error: cd: bad arguments\n"));
	if(chdir(v[1]) == - 1)
		return(err("error: cd: cannot change directory to "), err(v[1]), err("\n"));
	return 0;
}
int exec(int c, char **v, char **envp)
{
	int is_pipe = v[c] && !strcmp(v[c], "|");
	int status;
	int pid;
	int fd[2];
	if(!is_pipe && !strcmp(v[0],"cd"))
		return(cd(c,v));
	if(is_pipe && pipe(fd) == -1)
		return(err("error: fatal\n"));
	pid = fork();
	if(pid == -1)
		return(err("error: fatal\n"));
	if(!pid)
	{
		v[c] = 0;
		if(is_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return(err("error: fatal\n"));
		execve(v[0], v, envp);
		return(err("error: cannot execute "),err(v[0]), err("\n"));
	}
	waitpid(pid, &status, 0);
	if(is_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        	return(err("error: fatal\n"));
	return(WIFEXITED(status) && WEXITSTATUS(status));
}

int main(int c,char **v, char** envp)
{
	int status = 0;
	int i = 0;
	if (c > 1)
	{
		while(v[i] && v[++i])
		{
			v = v + i;
			i = 0;
			while(v[i] && strcmp(v[i],"|") && strcmp(v[i], ";"))
				i++;
			if(i)
				status = exec(i,v,envp);
		}
	}
	return status;
}
