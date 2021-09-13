#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "main.h"

#define main2 main

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

int	main1(int ac, char **av)
{
	int	pipefd[2];
	int child_pid;
	int infile_fd;

	if (pipe(pipefd) == -1)
		perror("pipe :");
	child_pid = fork();
	if (child_pid ==  0)
	{
		char *array[] = {"/usr/bin/wc", "-l", NULL};
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		print_execv(array);
		if(execv(array[0], array) == -1)
			perror("execv 1:");
	}
	else
	{
		//2 --> stdout
		//pipefd[1] --> pipe_write_end
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		// fd 2 now points to our pipe's write end
		// Then we can exec our binary that uses 2 for its output
		print_execv(&av[1]);
		if (execv(av[1], &av[1]) == -1)
			perror("execv 2:");
	}

	puts("Hi on my way out");

	return (0);
}

int	main2(int ac, char **av)
{
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
		print_execv(&av[1]);
		if (execv(av[1], &av[1]) == -1)
			perror("execv 1:");
	}
	else
	{
		wait(NULL);
		char *array[] = {"/usr/bin/wc", "-l", NULL};
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		print_execv(array);
		if(execv(array[0], array) == -1)
			perror("execv 2:");
	}

	puts("Hi on my way out");

	return (0);
}

int	main3(int argc, char** argv)
{
	int pipefd[2];

	argv[3] = NULL;
	for (int i = 0; i < argc;  i++)
		printf("argv[%i] = %s\n", i, argv[i]);
	pipe(pipefd);
	// child process, executes echo :w
	int childPid = fork();
	if (childPid == 0)
	{
		const char *av[] = {
			"/bin/echo",
			"h_e_l_l_o_ _w_o_r_l_d",
			NULL,
		};
		close(pipefd[0]); // closing unused read end
		dup2(pipefd[1], 1);
		close(pipefd[1]); // closing original write end, because it has been duplicated
		if (execv(argv[1], &argv[1]) == -1)
			perror("execv: ");
		exit(0);
	}
	else
	{
		// parent process, wires up echo's output to tr's input <]
		char *av[] = {
			"/usr/bin/tr",
			"-d",
			"_",
			NULL,
		};
		close(pipefd[1]); // closing unused write end
		dup2(pipefd[0], 0);
		close(pipefd[0]); // closing original read end, because it has been duplicated
		if (execv(av[0], av) == -1)
		{
			perror("execv: ");
		}
	}
	return (0);
}
