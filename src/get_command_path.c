#include "pipex.h"

char	*get_command_path(char *cmd, char *cwd, char **pathvar_entries)
{
	if (cmd[0] == '/')
	{
		return (cmd);
	}
	else if (ft_strchr(cmd, '/'))
	{
		cmd = retrieve_file_path_in_cwd(cwd, cmd);
	}
	else
	{
		cmd = retrieve_command_in_pathvar(cmd, pathvar_entries);
	}
	return (cmd);
}
