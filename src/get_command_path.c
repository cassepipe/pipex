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

char	*get_command_path(char **cmdv, char *cwd, char **pathvar_entries)
{
	if (cmdv[0] && *cmdv[0])
	{
		if (cmdv[0][0] == '/')
		{
			return (cmdv[0]);
		}
		else if (ft_strchr(cmdv[0], '/'))
		{
			cmdv[0] = retrieve_file_path_in_cwd(cwd, cmdv[0]);
			if (!cmdv[0])
			{
				free_null_terminated_array_of_arrays(&cmdv[1]);
				write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
				perror("malloc");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cmdv[0] = retrieve_command_in_pathvar(cmdv[0], pathvar_entries);
			if (!cmdv[0])
			{
				free_null_terminated_array_of_arrays(&cmdv[1]);
				write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
				perror("malloc");
				exit(EXIT_FAILURE);
			}
		}
	}
	return (cmdv[0]);
}
