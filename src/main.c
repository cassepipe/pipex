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
	if ((outfile_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		perror("open ");
	if (pipe(pipefd) == -1)
		perror("pipe ");

	pathvar_entries = ft_split(get_path_var(envp), ':');

	child_pid = fork();
	if (child_pid == -1)
		perror("fork ");
	else if (child_pid ==  0)
	{
		close(pipefd[0]);
		redirect_fd_to_fd(0, infile_fd);
		redirect_fd_to_fd(1, pipefd[1]);
		cmd1 = ft_split(av[2], ' ');
		cmd1[0] = get_command_path(cmd1[0], get_pwd_var(envp), pathvar_entries);
		if (execve(cmd1[0], cmd1, envp) == -1)
		{
			perror("execve 1:");
		}
		free_null_terminated_array_of_arrays(cmd1);
		free_null_terminated_array_of_arrays(pathvar_entries);
	}
	else
	{
		close(pipefd[1]);
		redirect_fd_to_fd(0, pipefd[0]);
		redirect_fd_to_fd(1, outfile_fd);
		cmd2 = ft_split(av[3], ' ');
		cmd2[0] = get_command_path(cmd2[0], get_pwd_var(envp), pathvar_entries);
		if (execve(cmd2[0], cmd2, envp) == -1)
		{
			perror("execve 2:");
		}
		free_null_terminated_array_of_arrays(cmd2);
		free_null_terminated_array_of_arrays(pathvar_entries);

	}
	return (0);
}
