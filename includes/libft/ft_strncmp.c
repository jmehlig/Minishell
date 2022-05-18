/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:07:32 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:07:35 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i] || (s1[i] == 0 || s2[i] == 0))
		{
			return ((int)((unsigned char)s1[i] - (unsigned char)s2[i]));
		}
		else
		{
			i++;
		}
	}
	return (0);
}
