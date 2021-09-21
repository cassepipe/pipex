#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "libft.h"
#include "get_command_path.h"

char	*get_command_path(char *cmd, char *cwd, char **pathvar_entries)
{
	printf("get_command_path:cmd = %s\n", cmd);
	//We have an abosulte path
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == -1)
		{
			perror("No such file directory :");
			exit(EXIT_FAILURE);
		}
	}
	//Or a relative path
	else if (ft_strchr(cmd, '/'))
	{
		if (!(cmd = retrieve_file_path_in_cwd(cwd, cmd)))
		{
			perror("No such file directory :");
			exit(EXIT_FAILURE);
		}
	}
	//Or just a command name
	else if (!(cmd = retrieve_command_in_pathvar(cmd, pathvar_entries)))
	{
		perror("Command not found :");
		exit(EXIT_FAILURE);
	}
	return (cmd);
}
