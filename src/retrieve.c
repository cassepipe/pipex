#include "retrieve.h"
#include "libft.h"

char	*retrieve_file_path_in_cwd(char *cwd, char* cmd)
{
	char	*file_path;

	file_path = ft_strjoin(cwd, cmd);
	if (access(file_path, F_OK) == -1)
		return (NULL);
	free((char *)cmd);
	return (file_path);
}

char	*retrieve_command_in_pathvar(char *cmd, char **pathvar_entries)
{
	char *cmd_path;

	/*printf("retrieve_command_in_pathvar:cmd = %s\n", cmd);*/
	while (pathvar_entries)
	{
		cmd_path = ft_strjoin3(*pathvar_entries,"/", cmd);
		if (!cmd_path)
		{
			perror("ft_strjoin :");
			exit(EXIT_FAILURE);
		}
		//If path exists
		if (access(cmd_path, F_OK)  == 0)
		{
			free((char*)cmd);
			return cmd_path;
		}
		free(cmd_path);
		pathvar_entries++;
	}
	return (NULL);
}
