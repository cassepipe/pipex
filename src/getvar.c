#include "libft.h"
#include "getvar.h"

#include <string.h>

char	*get_path_var(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			return (*envp + 5);
		}
		envp++;
	}
	return (NULL);
}

char	*get_pwd_var(char **envp)
{
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PWD=", 3))
		{
			return (*envp + 3);
		}
		envp++;
	}
	return (NULL);
}
