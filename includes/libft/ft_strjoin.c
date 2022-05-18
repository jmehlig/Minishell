/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:08:12 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:08:14 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	lens1;
	size_t	lens2;
	size_t	i;

	if (!s1 || !s2)
		return (0);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	i = 0;
	ptr = (char *)malloc(sizeof(char) * (lens1 + lens2 + 1));
	if (!ptr)
		return (0);
	while (i < lens1)
	{
		ptr[i] = s1[i];
		i++;
	}
	while (i < lens1 + lens2)
	{
		ptr[i] = s2[i - lens1];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}
