/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:07:59 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:08:00 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;
	size_t	j;

	i = 0;
	if (!src)
		return (0);
	srclen = ft_strlen(src);
	j = srclen;
	if (dstsize == 0)
	{
		return (ft_strlen(src));
	}
	while (i < dstsize - 1 && j != 0)
	{
		dst[i] = src[i];
		i++;
		j--;
	}
	dst[i] = 0;
	return (srclen);
}
