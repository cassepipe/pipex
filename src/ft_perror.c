/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassepipe <cassepipe@ymail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:50:34 by cassepipe         #+#    #+#             */
/*   Updated: 2022/01/06 22:21:37 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "pipex.h"

static char	*strjoin(char const *s1, char const *s2, char const *s3,
																	size_t *len)
{
	char	*joined;

	*len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = malloc(*len + 1);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, *len + 1);
	ft_strlcat(joined, s2, *len + 1);
	ft_strlcat(joined, s3, *len + 1);
	return (joined);
}

void	ft_perror(const char *prg_name, const char *about, const char *err_msg)
{
	char				*s;
	size_t				len;
	static const char	empty_string[] = "\'\'";

	if (*about == '\0')
		about = empty_string;
	s = strjoin(prg_name, about, err_msg, &len);
	write(STDERR_FILENO, s, len);
	free(s);
}
