#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pipex.h"

static const char	g_usage[] = {
	"Usage : "
	"./pipex infile \"cmd_1 [--option]...\" \"cmd_2 [--option]...\" outfile\n"
};

static void	print_usage_exit(void)
{
	write(STDOUT_FILENO, g_usage, sizeof(g_usage));
	exit(EXIT_FAILURE);
}

static int	ft_open(char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		perror(filename);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

static void	pipe_or_die(int *pipe_fds)
{
	int	r;

	r = pipe(pipe_fds);
	if (r == -1)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	execute_pipeline(char *cmd_str, int read_from, int write_to, char **env)
{
	char	**pathvar_entries;
	char	**cmdv;

	redirect_fd_to_fd(0, read_from);
	redirect_fd_to_fd(1, write_to);
	pathvar_entries = ft_split(get_path_var(env), ':');
	cmdv = ft_split(cmd_str, ' ');
	cmdv[0] = get_command_path(cmdv[0], get_pwd_var(env), pathvar_entries);
	if (access(cmdv[0], X_OK) == -1)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		perror(cmdv[0]);
		free_null_terminated_array_of_arrays(cmdv);
		free_null_terminated_array_of_arrays(pathvar_entries);
		if (errno == ENOENT)
			exit(127);
		else if (errno == EACCES)
			exit(126);
		else
			exit(EXIT_FAILURE);
	}
	execve(cmdv[0], cmdv, env);
	free_null_terminated_array_of_arrays(cmdv);
	free_null_terminated_array_of_arrays(pathvar_entries);
}

int	main(int ac, char **av, char **envp)
{
	int		pipefd[2];
	int		child_pid;
	int		infile_fd;
	int		outfile_fd;

	if (ac != 5)
		print_usage_exit();
	pipe_or_die(pipefd);
	child_pid = fork();
	if (child_pid == -1)
		perror("fork");
	else if (child_pid == 0)
	{
		infile_fd = ft_open(av[1], O_RDONLY, 0000);
		close(pipefd[0]);
		execute_pipeline(av[2], infile_fd, pipefd[1], envp);
	}
	else
	{
		outfile_fd = ft_open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		close(pipefd[1]);
		execute_pipeline(av[3], pipefd[0], outfile_fd, envp);
	}
	return (EXIT_SUCCESS);
}
