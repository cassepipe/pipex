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

#define	FILE_ARGS	2
#define	PROGRAM_NAME	1

#define PIPE_FUTURE_READ_END 0
#define PIPE_WRITE_END 1
#define PIPE_READ_END 2
#define OUTFILE 3


static const char	g_cmd_not_found[] = {
	"command not found: "
};

static const char	g_empty_string[] = {
	"The name of the input or output file cannot be an empty string\n"
};

static const char	g_no_file_or_dir[] = {
	"No such file or drectory: "
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
		{
			write(STDERR_FILENO, g_no_file_or_dir, sizeof(g_no_file_or_dir));
			ft_puts_stderr(cmdv[0]);
		}
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

int	execute(char *cmd, char **envp, char **pathvar_entries, int *fd)
{
	int cpid;
	/*char filePath[PATH_MAX];*/

	pipe_or_die(fd);
	dup2(fd[PIPE_READ_END], 0);
	close(fd[PIPE_READ_END]);
	dup2(fd[PIPE_WRITE_END], 1);
	close(fd[PIPE_WRITE_END]);
	cpid = fork();
	if (cpid == 0)
	{
		/*if (readlink("/proc/self/fd/0", filePath, PATH_MAX) != -1)*/
		/*    dprintf(STDERR_FILENO, "In cmd=  %s 0 points to %s\n", cmd, filePath);*/
		/*if (readlink("/proc/self/fd/1", filePath, PATH_MAX) != -1)*/
		/*    dprintf(STDERR_FILENO, "In cmd=  %s 1 points to %s\n", cmd, filePath);*/
		close(fd[PIPE_FUTURE_READ_END]);
		find_exec(cmd, envp, pathvar_entries);
	}
	fd[PIPE_READ_END] = fd[PIPE_FUTURE_READ_END];
	return cpid;
}

int	execute_last(char *cmd, char **envp, char **pathvar_entries, int *fd)
{
	int cpid;
	/*char filePath[PATH_MAX];*/

	dup2(fd[PIPE_READ_END], 0);
	close(fd[PIPE_READ_END]);
	dup2(fd[OUTFILE], 1);
	close(fd[OUTFILE]);
	cpid = fork();
	if (cpid == 0)
	{
		/*if (readlink("/proc/self/fd/0", filePath, PATH_MAX) != -1)*/
		/*    dprintf(STDERR_FILENO, "In cmd=  %s 0 points to %s\n", cmd, filePath);*/
		/*if (readlink("/proc/self/fd/1", filePath, PATH_MAX) != -1)*/
		/*    dprintf(STDERR_FILENO, "In cmd=  %s 1 points to %s\n", cmd, filePath);*/
		find_exec(cmd, envp, pathvar_entries);
	}
	return cpid;
}

int	main(int ac, char **av, char **envp)
{
	int		n;
	int		wstatus;
	char	**pathvar_entries;
	int		last_pid;
	int		fd[4];

	if (ac < PROGRAM_NAME + FILE_ARGS + 2)
		print_usage_exit();
	fd[PIPE_READ_END] = ft_open(av[1], O_RDONLY, 0666);
	fd[OUTFILE] = ft_open(av[ac - 1], O_WRONLY | O_CREAT, 0666);
	pathvar_entries = ft_split(get_path_var(envp), ':');
	n = -1;
	av += 2;
	while (++n < ac - PROGRAM_NAME - FILE_ARGS - 1)
		execute(*(av + n), envp, pathvar_entries, fd);
	last_pid = execute_last(*(av + n), envp, pathvar_entries, fd);
	free_null_terminated_array_of_arrays(pathvar_entries);
	n = 0;
	while (n < ac - PROGRAM_NAME - FILE_ARGS - 1)
	{
		wait(NULL);
		n++;
	}
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
