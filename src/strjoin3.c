#include "libft.h"

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	size_t	len;
	char	*joined;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = malloc(len + 1);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len + 1);
	ft_strlcat(joined, s2, len + 1);
	ft_strlcat(joined, s3, len + 1);
	return (joined);
}
