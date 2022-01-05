/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:15:32 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:15:32 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "pipex.h"

#define READ_END 0
#define WRITE_END 1

static const char	g_cmd_not_found[] = {
	"command not found: "
};

static const char	g_empty_string[] = {
	"The name of the input or output file cannot be an empty string\n"
};

static int	ft_open(char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		if (*filename == '\0')
			write(STDERR_FILENO, g_empty_string, sizeof(g_empty_string));
		else
		{
			write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
			perror(filename);
		}
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

static void	file_is_ok_or_die(char **cmdv, char **pathvar_entries)
{
	if (access(cmdv[0], X_OK) == -1)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		if (cmdv[0][0] != '/')
		{
			write(STDERR_FILENO, g_cmd_not_found, sizeof(g_cmd_not_found));
			ft_puts_stderr(cmdv[0]);
		}
		else
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
}

void	find_exec(char *cmd_str, char **env, char **pathvar_entries)
{
	char	**cmdv;

	cmdv = ft_split(cmd_str, ' ');
	if (!pathvar_entries || !cmdv)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		perror("malloc");
		free_null_terminated_array_of_arrays(cmdv);
		free_null_terminated_array_of_arrays(pathvar_entries);
		exit(EXIT_FAILURE);
	}
	cmdv[0] = get_command_path(cmdv, get_pwd_var(env), pathvar_entries);
	file_is_ok_or_die(cmdv, pathvar_entries);
	execve(cmdv[0], cmdv, env);
	free_null_terminated_array_of_arrays(cmdv);
	free_null_terminated_array_of_arrays(pathvar_entries);
}

#include <limits.h>
#include <fcntl.h>


int	execute_pipeline(char *cmd, char **envp, char **pathvar_entries, int pipe_end)
{
	int	child_pid;
	int pipe_fd[2];
	char filePath[PATH_MAX];


	child_pid = fork();
	if (child_pid == -1)
		perror("fork");
	else if (child_pid == 0)
	{
		pipe_or_die(pipe_fd);
		dprintf(STDERR_FILENO, "In cmd = %s, opened pipe[READ_END]=%i and pipe[WRITE_END]=%i\n", cmd, pipe_fd[0], pipe_fd[1]);
		if (pipe_end - 1 < 0)
		{
			dprintf(STDERR_FILENO, "In cmd= %s fd 1 now points to pipe[WRITE_END]\n", cmd);
			redirect_fd_to_fd(1, pipe_fd[1]);
		}
		if (pipe_end + 1 > 0)
		{
			dprintf(STDERR_FILENO, "In cmd= %s fd 0 now points to pipe[READ_END]\n", cmd);
			redirect_fd_to_fd(0, pipe_fd[0]);
		}
		if (readlink("/proc/self/fd/0", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 0 points to %s\n", cmd, filePath);
		if (readlink("/proc/self/fd/1", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 1 points to %s\n", cmd, filePath);
		dprintf(STDERR_FILENO, "About to exec cmd=%s\n", cmd);
		find_exec(cmd, envp, pathvar_entries);
	}
	return child_pid;
}

int	execute(char *cmd, char **envp, char **pathvar_entries, int *reading_pipe, int *writing_pipe)
{
	int cpid;
	char filePath[PATH_MAX];

	pipe_or_die(writing_pipe);
	close(reading_pipe[WRITE_END]);
	dup2(reading_pipe[READ_END], 0);
	close(reading_pipe[READ_END]);
	dup2(writing_pipe[WRITE_END], 1);
	close(writing_pipe[WRITE_END]);
	cpid = fork();
	if (cpid == 0)
	{
		if (readlink("/proc/self/fd/0", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 0 points to %s\n", cmd, filePath);
		if (readlink("/proc/self/fd/1", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 1 points to %s\n", cmd, filePath);
		close(writing_pipe[READ_END]);
		find_exec(cmd, envp, pathvar_entries);
	}
	reading_pipe[READ_END] = writing_pipe[READ_END];
	return cpid;
}

int	execute_last(char *cmd, char **envp, char **pathvar_entries, int *reading_pipe, int outfile)
{
	int cpid;
	char filePath[PATH_MAX];

	close(reading_pipe[WRITE_END]);
	dup2(reading_pipe[READ_END], 0);
	close(reading_pipe[READ_END]);
	dup2(outfile, 1);
	close(outfile);
	cpid = fork();
	if (cpid == 0)
	{
		if (readlink("/proc/self/fd/0", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 0 points to %s\n", cmd, filePath);
		if (readlink("/proc/self/fd/1", filePath, PATH_MAX) != -1)
			dprintf(STDERR_FILENO, "In cmd=  %s 1 points to %s\n", cmd, filePath);
		find_exec(cmd, envp, pathvar_entries);
	}
	return cpid;
}

int	main(int ac, char **av, char **envp)
{
	int		n;
	int		n_cmds;
	int		wstatus;
	char	**pathvar_entries;
	int		last_pid;
	int		reading_pipe[2];
	int		writing_pipe[2];
	int		outfile_fd;

	if (ac < 5)
		print_usage_exit();
	n_cmds = ac - 3;
	pathvar_entries = ft_split(get_path_var(envp), ':');
	reading_pipe[READ_END] = ft_open(av[1], O_RDONLY, 0666);
	outfile_fd = ft_open(av[ac - 1], O_WRONLY | O_CREAT, 0666);
	n = 0;
	av += 2;
	while (n < n_cmds - 1)
	{
		execute(*(av + n), envp, pathvar_entries, reading_pipe, writing_pipe);
		n++;
	}
	last_pid = execute_last(*(av + n), envp, pathvar_entries, reading_pipe, outfile_fd);
	close(reading_pipe[READ_END]);
	close(outfile_fd);
	n = 0;
	while (n <  n_cmds -1)
	{
		dprintf(STDERR_FILENO, "Waiting for av[n]=%s\n", av[n]);
		wait(NULL);
		n++;
	}
	dprintf(STDERR_FILENO, "Waiting for av[n]=%s\n", av[n]);
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
