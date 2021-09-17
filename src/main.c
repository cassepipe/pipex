#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "main.h"
#include "libft.h"

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

int	main(int ac, char **av, char **envp)
{
	int		pipefd[2];
	int		child_pid;
	int		infile_fd;
	int		outfile_fd;
	char	**cmd1;
	char	**cmd2;

	if ((infile_fd = open(av[1], O_RDONLY)) < 0)
		perror("open ");
	if ((outfile_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC)) < 0)
		perror("open ");
	if (pipe(pipefd) == -1)
		perror("pipe ");
	child_pid = fork();
	if (child_pid == -1)
		perror("fork ");
	else if (child_pid ==  0)
	{
		cmd1 = ft_split(av[2], ' ');
		//2 --> stdout
		//pipefd[1] --> pipe_write_end
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		// fd 2 now points to our pipe's write end
		// Then we can exec our binary that uses 2 for its output
		if (execve(cmd1[0], cmd1, envp) == -1)
			perror("execve 1:");
		free(cmd1);
	}
	else
	{
		cmd2 = ft_split(av[3], ' ');
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		if (execve(cmd2[0], cmd2, envp) == -1)
			perror("execve 2:");
		free(cmd2);
	}
	return (0);
}
