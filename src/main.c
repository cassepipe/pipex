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
#include "t_job.h"

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

int	handle_redir_in(t_job job, int *fd)
{
	if (job.curr_cmd_idx == 0)
		fd[PIPE_READ_END] = open(job.cmds[-1], O_RDONLY, 0644);
	if (fd[PIPE_READ_END] == -1)
	{
		ft_perror("pipex: ", job.cmds[-1], ": No such file or directory\n");
		return (1);
	}
	return (0);
}

int	handle_redir_out(t_job job, int *fd)
{
	if (job.curr_cmd_idx == job.nb_cmds - 1)
		fd[PIPE_WRITE_END] = open
			(job.cmds[job.nb_cmds], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd[PIPE_WRITE_END] == -1)
	{
		ft_perror ("pipex: ", job.cmds[job.nb_cmds],
			": Permission denied\n");
		return (1);
	}
	return (0);
}

int	execute_job(t_job job, int *fd)
{
	int	cpid;
	int	ret;

	ret = pipe_or_err(fd);
	if (!ret)
		ret = handle_redir_in(job, fd);
	dup2(fd[PIPE_READ_END], 0);
	close(fd[PIPE_READ_END]);
	if (!ret)
		ret = handle_redir_out(job, fd);
	dup2(fd[PIPE_WRITE_END], 1);
	close(fd[PIPE_WRITE_END]);
	cpid = fork();
	if (cpid == 0)
	{
		close(fd[PIPE_FUTURE_READ_END]);
		if (ret == 0)
			find_exec
				(job.cmds[job.curr_cmd_idx], job.envp, job.pathvar_entries);
		else
			free_null_terminated_array_of_arrays(job.pathvar_entries);
		exit(EXIT_FAILURE);
	}
	fd[PIPE_READ_END] = fd[PIPE_FUTURE_READ_END];
	return (cpid);
}

int	main(int ac, char **av, char **envp)
{
	int		wstatus;
	int		last_pid;
	int		fd[3];
	t_job	job;

	if (ac < PROGRAM_NAME + FILE_ARGS + 2)
		print_usage_exit();
	av += 2;
	job.envp = envp;
	job.pathvar_entries = ft_split(get_path_var(envp), ':');
	if (job.pathvar_entries == NULL)
		return (EXIT_FAILURE);
	job.cmds = av;
	job.nb_cmds = ac - PROGRAM_NAME - FILE_ARGS;
	job.curr_cmd_idx = -1;
	while (++job.curr_cmd_idx < job.nb_cmds)
		last_pid = execute_job(job, fd);
	free_null_terminated_array_of_arrays(job.pathvar_entries);
	job.curr_cmd_idx = -1;
	while (++job.curr_cmd_idx < job.nb_cmds - 1)
		wait(NULL);
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
