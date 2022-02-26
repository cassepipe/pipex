/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrappers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 17:08:22 by tpouget           #+#    #+#             */
/*   Updated: 2022/02/26 17:08:22 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "pipex.h"

int	ft_open(char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		ft_perror("pipex: ", filename, ": No such file or directory\n");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	pipe_or_die(int *pipe_fds)
{
	int	r;

	r = pipe(pipe_fds);
	if (r == -1)
	{
		perror("pipex: pipe");
		exit(EXIT_FAILURE);
	}
}

void	file_is_ok_or_die(char **cmdv, char **pathvar_entries)
{
	if (access(cmdv[0], X_OK) == -1)
	{
		if (cmdv[0][0] != '/')
			ft_perror("pipex: ", cmdv[0], ": command not found\n");
		else
			ft_perror("pipex: ", cmdv[0], ": No such file or directory\n");
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
