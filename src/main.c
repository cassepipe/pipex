/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:15:32 by tpouget           #+#    #+#             */
/*   Updated: 2022/01/06 22:23:30 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include "pipex.h"

#define FILE_ARGS 2
#define PROGRAM_NAME 1

#define PIPE_FUTURE_READ_END 0
#define PIPE_WRITE_END 1
#define PIPE_READ_END 2
#define OUTFILE 3

void	find_exec(char *cmd_str, char **env, char **pathvar_entries)
{
	char	**cmdv;

	cmdv = ft_split(cmd_str, ' ');
	if (!pathvar_entries || !cmdv)
	{
		perror("pipex: malloc");
		free_null_terminated_array_of_arrays(cmdv);
		free_null_terminated_array_of_arrays(pathvar_entries);
		exit(EXIT_FAILURE);
	}
	cmdv[0] = get_command_path(cmdv, get_pwd_var(env), pathvar_entries);
	file_is_ok_or_die(cmdv, pathvar_entries);
	execve(cmdv[0], cmdv, env);
	free_null_terminated_array_of_arrays(cmdv);
	free_null_terminated_array_of_arrays(pathvar_entries);
	perror(cmdv[0]);
	exit(EXIT_FAILURE);
}

int	execute(char *cmd, char **envp, char **pathvar_entries, int *fd)
{
	int	cpid;

	pipe_or_die(fd);
	dup2(fd[PIPE_READ_END], 0);
	close(fd[PIPE_READ_END]);
	dup2(fd[PIPE_WRITE_END], 1);
	close(fd[PIPE_WRITE_END]);
	cpid = fork();
	if (cpid == 0)
	{
		close(fd[PIPE_FUTURE_READ_END]);
		find_exec(cmd, envp, pathvar_entries);
	}
	fd[PIPE_READ_END] = fd[PIPE_FUTURE_READ_END];
	return (cpid);
}

int	execute_last(char *cmd, char **envp, char **pathvar_entries, int *fd)
{
	int	cpid;

	dup2(fd[PIPE_READ_END], 0);
	close(fd[PIPE_READ_END]);
	dup2(fd[OUTFILE], 1);
	close(fd[OUTFILE]);
	cpid = fork();
	if (cpid == 0)
	{
		find_exec(cmd, envp, pathvar_entries);
	}
	return (cpid);
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
	fd[OUTFILE] = ft_open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	fd[PIPE_READ_END] = ft_open(av[1], O_RDONLY, 0666);
	pathvar_entries = ft_split(get_path_var(envp), ':');
	av += 2;
	n = -1;
	while (++n < ac - PROGRAM_NAME - FILE_ARGS - 1)
		execute(*(av + n), envp, pathvar_entries, fd);
	last_pid = execute_last(*(av + n), envp, pathvar_entries, fd);
	free_null_terminated_array_of_arrays(pathvar_entries);
	n = -1;
	while (++n < ac - PROGRAM_NAME - FILE_ARGS - 1)
		wait(NULL);
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
