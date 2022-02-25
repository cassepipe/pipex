/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:57 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:57 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	if (s)
	{
		while (*s)
		{
			s++;
			len++;
		}
	}
	return (len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i])
		{
			if (i + 1 == size)
				break ;
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && dst[i])
		i++;
	i += ft_strlcpy(dst + i, src, size - i);
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2, size_t *len)
{
	char	*joined;

	if (!s1 || !s2)
		return (NULL);
	*len = ft_strlen(s1) + ft_strlen(s2);
	joined = malloc(*len + 1);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, *len + 1);
	ft_strlcat(joined, s2, *len + 1);
	return (joined);
}

char	*ft_empty_string(void)
{
	char	*ret;

	ret = malloc(1 * sizeof(char));
	if (!ret)
		return (NULL);
	*ret = '\0';
	return (ret);
}
