#include "retrieve.h"
#include "libft.h"

char	*retrieve_file_path_in_cwd(char *cwd, char *cmd)
{
	char	*file_path;

	file_path = ft_strjoin(cwd, cmd);
	if (access(file_path, F_OK) == -1)
		return (NULL);
	free((char *)cmd);
	return (file_path);
}

char	*retrieve_command_in_pathvar(char **cmd, char **pathvar_entries)
{
	char	*cmd_path;

	while (pathvar_entries)
	{
		cmd_path = ft_strjoin3(*pathvar_entries, "/", *cmd);
		if (!cmd_path)
		{
			printf ("%s, %s\n", *pathvar_entries, cmd_path);
			perror("ft_strjoin3 :");
			exit(EXIT_FAILURE);
		}
		if (access(cmd_path, F_OK) == 0)
		{
			free((char *)*cmd);
			*cmd = cmd_path;
			return (cmd_path);
		}
		free(cmd_path);
		pathvar_entries++;
	}
	return (NULL);
}
