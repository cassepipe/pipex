#include <stdlib.h>
#include "pipex.h"

static void	free_all(char **dir, long i)
{
	while (i-- >= 0)
		free(dir[i]);
	free(dir);
}

static const char	*next_sep(const char *str, char sep)
{
	while (*str && *str != sep)
		str++;
	return (str);
}

static const char	*next_word(const char *str, char sep)
{
	while (*str && *str == sep)
		str++;
	return (str);
}

static char	**diralloc(const char *s, char c)
{
	size_t	size;

	if (!s)
		return (NULL);
	size = 1;
	s = next_word(s, c);
	while (*s)
	{
		s = next_sep(s, c);
		s = next_word(s, c);
		size++;
	}
	return (malloc(size * sizeof(char *)));
}

char	**ft_split(char const *s, char c)
{
	char		**dir;
	const char	*follower;
	long		i;

	dir = diralloc(s, c);
	if (!dir)
		return (NULL);
	s = next_word(s, c);
	follower = s;
	i = 0;
	while (*s)
	{
		s = next_sep(s, c);
		dir[i] = ft_strndup(follower, s - follower);
		if (!dir[i])
		{
			free_all(dir, i);
			return (NULL);
		}
		s = next_word(s, c);
		follower = s;
		i++;
	}
	dir[i] = NULL;
	return (dir);
}
