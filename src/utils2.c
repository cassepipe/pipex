/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "pipex.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	value;
	char			*ptr;

	value = (unsigned char)c;
	ptr = (char *)s;
	while (*ptr)
	{
		if (*ptr == value)
			return (ptr);
		ptr++;
	}
	if (!c)
		return (ptr);
	else
		return (NULL);
}

int	ft_strncmp(const char *l, const char *r, size_t n)
{
	size_t			i;
	int				diff;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	diff = 0;
	s1 = (unsigned char *)l;
	s2 = (unsigned char *)r;
	while (i < n)
	{
		diff = s1[i] - s2[i];
		if (diff)
			return (diff);
		if (!s1[i] || !s2[i])
			return (diff);
		i++;
	}
	return (diff);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	size_t	len;
	char	*duplicate;

	i = 0;
	len = 0;
	while (len < n && s[len])
		len++;
	duplicate = malloc(len + 1);
	if (!duplicate)
		return (NULL);
	while (i < len)
	{
		duplicate[i] = s[i];
		i++;
	}
	duplicate[len] = '\0';
	return (duplicate);
}

void	ft_puts_stderr(char *str)
{
	int	i;

	i = ft_strlen(str);
	if (*str == '\0')
		write(STDERR_FILENO, "\"\"", 2);
	else
		write(STDERR_FILENO, str, i);
	write(STDERR_FILENO, "\n", 1);
}
