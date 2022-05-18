/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:09:42 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:09:43 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	c = (unsigned char)c;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == c)
			return ((unsigned char *) &s[i]);
		i++;
	}
	return (0);
}
