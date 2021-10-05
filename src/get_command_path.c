/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:19:15 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:19:15 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "pipex.h"

static void	free_all_and_die(char **cmdv, char **pathvar_entries)
{
	free_null_terminated_array_of_arrays(cmdv);
	free_null_terminated_array_of_arrays(pathvar_entries);
	exit(EXIT_FAILURE);
}

static void	die_from_malloc_failure(char **cmdv, char **pathvar_entries)
{
	free_null_terminated_array_of_arrays(cmdv);
	free_null_terminated_array_of_arrays(pathvar_entries);
	write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
	perror("malloc");
	exit(EXIT_FAILURE);
}

char	*get_command_path(char **cmdv, char *cwd, char **pathvar_entries)
{
	if (!cwd)
		free_all_and_die(cmdv, pathvar_entries);
	if (cmdv[0] && cmdv[0][0])
	{
		if (cmdv[0][0] == '/')
			return (cmdv[0]);
		else if (ft_strchr(cmdv[0], '/'))
		{
			cmdv[0] = retrieve_file_path_in_cwd(cwd, cmdv[0]);
			if (!cmdv[0])
				die_from_malloc_failure(&cmdv[1], pathvar_entries);
		}
		else
		{
			cmdv[0] = retrieve_command_in_pathvar(cmdv[0], pathvar_entries);
			if (!cmdv[0])
				die_from_malloc_failure(&cmdv[1], pathvar_entries);
		}
	}
	return (cmdv[0]);
}
