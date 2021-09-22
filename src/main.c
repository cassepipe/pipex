#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "libft.h"
#include "main.h"

void	execute_pipeline(char **argv, int read_from, int write_to, char **envp)
{
	redirect_fd_to_fd(0, read_from);
	redirect_fd_to_fd(1, write_to);
	if (execve(argv[0], argv, envp) == -1)
	{
		perror("execve 1:");
	}
	free_null_terminated_array_of_arrays(argv);
}

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
		cmd1 = ft_split(av[2], ' ');
		cmd1[0] = get_command_path(cmd1[0], get_pwd_var(envp), pathvar_entries);
		execute_pipeline(cmd1, infile_fd, pipefd[1], envp);
		free_null_terminated_array_of_arrays(pathvar_entries);
	}
	else
	{
		close(pipefd[1]);
		cmd2 = ft_split(av[3], ' ');
		cmd2[0] = get_command_path(cmd2[0], get_pwd_var(envp), pathvar_entries);
		execute_pipeline(cmd2, pipefd[0], outfile_fd, envp);
		free_null_terminated_array_of_arrays(pathvar_entries);

	}
	return (0);
}
