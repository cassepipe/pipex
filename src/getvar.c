#include "libft.h"
#include "getvar.h"

#include <string.h>

char	*get_path_var(char **envp)
{
	while (*envp)
	{
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
		if (!ft_strncmp(*envp, "PWD=", 4))
		{
			return (*envp + 4);
		}
		envp++;
	}
	return (NULL);
}
