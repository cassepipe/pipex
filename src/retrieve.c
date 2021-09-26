#include <stdio.h>
#include <unistd.h>
#include "pipex.h"

char	*retrieve_file_path_in_cwd(char *cwd, char *cmd)
{
	char	*file_path;

	file_path = ft_strjoin3(cwd, "/", cmd);
	if (!file_path)
	{
		write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	free(cmd);
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
			write(STDERR_FILENO, "pipex: ", sizeof("pipex: "));
			perror("malloc");
			exit(EXIT_FAILURE);
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
