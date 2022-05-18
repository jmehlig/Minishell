/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:07:06 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/17 19:54:59 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_check(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i] != 0)
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_strtrim_cpy(char *ptr, char const *s1, size_t start, size_t end)
{
	size_t	i;

	i = 0;
	while (start < end)
	{
		ptr[i] = s1[start];
		i++;
		start++;
	}
	ptr[i] = 0;
	free (s1);
	return (ptr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	char	*ptr;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (0);
	i = 0;
	while (ft_check(s1[i], set) == 1 && s1[i] != 0)
		i++;
	start = i;
	i = ft_strlen(s1);
	while (i > start && ft_check(s1[i - 1], set) == 1)
		i--;
	end = i;
	ptr = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!ptr)
		return (0);
	return (ft_strtrim_cpy(ptr, s1, start, end));
}
