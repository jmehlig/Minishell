/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:06:34 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:06:36 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = ft_strlen(s);
	if (c == 0)
	{
		return ((char *) &s[i]);
	}
	while (i != 0)
	{
		if (s[i - 1] == c)
		{
			return ((char *) &s[i - 1]);
		}
		i--;
	}
	return (0);
}
