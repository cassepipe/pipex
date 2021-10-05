/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "pipex.h"

char	*retrieve_file_path_in_cwd(char *cwd, char *cmd)
{
	char	*file_path;

	file_path = ft_strjoin3(cwd, "/", cmd);
	free(cmd);
	if (!file_path)
	{
		return (NULL);
	}
	return (file_path);
}

char	*retrieve_command_in_pathvar(char *cmd, char **pathvar_entries)
{
	char	*cmd_path;

	while (*pathvar_entries)
	{
		cmd_path = ft_strjoin3(*pathvar_entries, "/", cmd);
		if (!cmd_path)
		{
			free((char *)cmd);
			return (NULL);
		}
		if (access(cmd_path, F_OK) == 0)
		{
			free((char *)cmd);
			return (cmd_path);
		}
		free(cmd_path);
		pathvar_entries++;
	}
	return (cmd);
}
