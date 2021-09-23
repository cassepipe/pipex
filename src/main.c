#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <assert.h>
#include "libft.h"
#include "main.h"
#include <errno.h>
#include <string.h>

static int	open_or_die(char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		perror("open ");
	return (fd);
}

static void	pipe_or_die(int *pipe_fds)
{
	int	r;

	r = pipe(pipe_fds);
	if (r == -1)
		perror("pipe ");
}

void	execute_pipeline(char *cmd_str, int read_from, int write_to, char **env)
{
	char	**pathvar_entries;
	char	**cmd;

	pathvar_entries = ft_split(get_path_var(env), ':');
	cmd = ft_split(cmd_str, ' ');
	cmd[0] = get_command_path(cmd[0], get_pwd_var(env), pathvar_entries);
	redirect_fd_to_fd(0, read_from);
	redirect_fd_to_fd(1, write_to);
	if (execve(cmd[0], cmd, env) == -1)
	{
		perror("execve:");
	}
}

int	main(int ac, char **av, char **envp)
{
	int		pipefd[2];
	int		child_pid;
	int		infile_fd;
	int		outfile_fd;

	infile_fd = open_or_die(av[1], O_RDONLY, 0000);
	outfile_fd = open_or_die(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	pipe_or_die(pipefd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork ");
	}
	else if (child_pid == 0)
	{
		close(pipefd[0]);
		execute_pipeline(av[2], infile_fd, pipefd[1], envp);
	}
	else
	{
		close(pipefd[1]);
		execute_pipeline(av[3], pipefd[0], outfile_fd, envp);
	}
	return (0);
}
