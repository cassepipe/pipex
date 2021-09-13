#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "main.h"

static void print_execv(char **arg_list)
{
	if (arg_list)
	{
		while (*arg_list)
		{
			printf("%s\n", *arg_list);
			arg_list++;
		}
	}
}

int	main(int ac, char **av)
{
	(void)(ac);
	int	pipefd[2];
	int child_pid;
	int infile_fd;

	if (pipe(pipefd) == -1)
		perror("pipe :");
	child_pid = fork();
	if (child_pid ==  0)
	{
		//2 --> stdout
		//pipefd[1] --> pipe_write_end
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		// fd 2 now points to our pipe's write end
		// Then we can exec our binary that uses 2 for its output
		if (execv(av[1], &av[1]) == -1)
			perror("execv 1:");
	}
	else
	{
		char *array[] = {"/usr/bin/wc", "-l", NULL};
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		if(execv(array[0], array) == -1)
			perror("execv 2:");
	}
	return (0);
}
