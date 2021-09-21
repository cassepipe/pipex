#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "libft.h"
#include "main.h"

int	main(int ac, char **av, char **envp)
{
	int		pipefd[2];
	int		child_pid;
	int		infile_fd;
	int		outfile_fd;
	char	**cmd1;
	char	**cmd2;
	char	**pathvar_entries;

	if ((infile_fd = open(av[1], O_RDONLY)) < 0)
		perror("open ");
	if ((outfile_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		perror("open ");
	if (pipe(pipefd) == -1)
		perror("pipe ");

	pathvar_entries = ft_split(get_path_var(envp), ':');

	child_pid = fork();
	if (child_pid == -1)
		perror("fork ");
	else if (child_pid ==  0)
	{
		//The child is never going to use the read end of the pipe
		close(pipefd[0]);

		//grep reads from fd 0 so 0 must point to infile
		dup2(infile_fd, 0);
		close(infile_fd);

		//grep writes to fd 1 so 1 must point to the write end of the pipe
		dup2(pipefd[1], 1);
		close(pipefd[1]);

		//Retrieving the args
		cmd1 = ft_split(av[2], ' ');

		// Append ./

		//Get the command path
		/*printf("*cmd1 = %s\n", *cmd1);*/
		cmd1[0] = get_command_path(cmd1[0], get_pwd_var(envp), pathvar_entries);
		/*printf("*cmd1 = %s\n", *cmd1);*/

		if (execve(cmd1[0], cmd1, envp) == -1)
		{
			perror("execve 1:");
		}
		for (int i = 0; cmd1[i] != NULL; i++)
			free(cmd1[i]);
		free(cmd1);
		for (int i = 0; pathvar_entries[i] != NULL; i++)
			free(pathvar_entries[i]);
		free(pathvar_entries);
	}
	else
	{
		//The parent is never goind to use the write end of the pipe
		close(pipefd[1]);

		//wc reads from 0 so 0 must point to the read end of the pipe
		dup2(pipefd[0], 0);
		close(pipefd[0]);

		//wc write to the 1 so 1 must point to the outfile
		dup2(outfile_fd, 1);
		close(outfile_fd);

		//Retrieving args
		cmd2 = ft_split(av[3], ' ');

		// Append ./

		//Get the command path
		cmd2[0] = get_command_path(cmd2[0], get_pwd_var(envp), pathvar_entries);

		if (execve(cmd2[0], cmd2, envp) == -1)
		{
			perror("execve 2:");
		}
		for (int i = 0; cmd2[i] != NULL; i++)
			free(cmd2[i]);
		free(cmd2);
		for (int i = 0; pathvar_entries[i] != NULL; i++)
			free(pathvar_entries[i]);
		free(pathvar_entries);
	}
	return (0);
}
