/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:49:58 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/03 17:31:39 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

void	*get_result(char **readbuffer, char *result)
{
	free(*readbuffer);
	*readbuffer = NULL;
	return (result);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return (&((char *)s)[i]);
		else
			i++;
	}
	if (s[i] == (unsigned char)c)
		return (&((char *)s)[i]);
	return (NULL);
}

char	*ft_strdup_gnl(const char *s)
{
	int		index;
	int		length;
	char	*target;

	if (!s)
		return (0);
	index = 0;
	length = ft_strlen_gnl(s);
	target = (char *)malloc((length + 1) * sizeof(char));
	if (target == 0)
		return (0);
	while (index < length)
	{
		target[index] = s[index];
		index++;
	}
	target[index] = '\0';
	return (target);
}

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	index;
// 	char	*sub;

// 	index = 0;
// 	if (s == 0)
// 		return (0);
// 	sub = (char *)malloc((len + 1) * sizeof(char));
// 	if (sub == 0)
// 		return (0);
// 	if (start >= ft_strlen(s))
// 	{
// 		sub[0] = '\0';
// 		return (sub);
// 	}
// 	while (index < len)
// 	{
// 		sub[index] = s[start + index];
// 		index++;
// 	}
// 	sub[index] = '\0';
// 	return (sub);
// }

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*ptr;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == 0 || s2 == 0)
		return (0);
	ptr = malloc((ft_strlen_gnl(s1) + ft_strlen_gnl(s2)) * sizeof(char) + 1);
	if (!ptr)
		return (NULL);
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		ptr[i] = s2[j];
		i++;
		j++;
	}
	ptr[i] = '\0';
	free(s1);
	return (ptr);
}

size_t	ft_strlen_gnl(const char *s)
{
	int	index;

	index = 0;
	while (s[index] != '\0')
		index++;
	return (index);
}
