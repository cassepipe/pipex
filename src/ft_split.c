/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

static void	free_dir_and_set_to_null(char ***dir, long i)
{
	while (i-- >= 0)
		free((*dir)[i]);
	free(*dir);
	*dir = NULL;
}

static const char	*next_sep(const char *str, char sep)
{
	while (str && *str && *str != sep)
		str++;
	return (str);
}

static const char	*next_word(const char *str, char sep)
{
	while (str && *str && *str == sep)
		str++;
	return (str);
}

static char	**diralloc(const char *s, char c)
{
	size_t	size;
	char	**ret;

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
	ret = malloc(size * sizeof(char *));
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	char		**dir;
	const char	*follower;
	long		i;

	dir = diralloc(s, c);
	s = next_word(s, c);
	follower = s;
	i = 0;
	if (dir && !*s)
		dir[i++] = ft_empty_string();
	if (!dir)
		return (NULL);
	while (dir && *s)
	{
		s = next_sep(s, c);
		dir[i] = ft_strndup(follower, s - follower);
		if (!dir[i])
			free_dir_and_set_to_null(&dir, i);
		s = next_word(s, c);
		follower = s;
		i++;
	}
	if (dir)
		dir[i] = NULL;
	return (dir);
}
