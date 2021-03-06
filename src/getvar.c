/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getvar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "pipex.h"

char	*get_path_var(char **envp)
{
	static const char	no_path_var_msg[] = {
		"pipex: Could not find PATH in the environmment variables\n"
		"Aborting...\n"
	};

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			return (*envp + 5);
		}
		envp++;
	}
	write(STDERR_FILENO, no_path_var_msg, sizeof(no_path_var_msg));
	exit(EXIT_FAILURE);
}

char	*get_pwd_var(char **envp)
{
	static const char	no_pwd_var_msg[] = {
		"pipex: Could not find PWD in the environmment variables\n"
		"Aborting...\n"
	};

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PWD=", 4))
		{
			return (*envp + 4);
		}
		envp++;
	}
	write(STDERR_FILENO, no_pwd_var_msg, sizeof(no_pwd_var_msg));
	return (NULL);
}
