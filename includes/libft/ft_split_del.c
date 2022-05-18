/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_del.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:17:13 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/03 19:22:19 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_memdel(void **ap)
{
	free(*ap);
	*ap = (NULL);
}

int	ft_splitdel(char ***split)
{
	char	**str;
	int		i;

	i = 0;
	str = *split;
	while (*str)
	{
		ft_memdel((void **)str);
		i++;
		str = &(*split)[i];
	}
	ft_memdel((void **)split);
	return (0);
}
