/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 16:00:36 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/03 19:10:11 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_free_double(char ***word, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(**word);
		(*word)++;
		i++;
	}
	exit(1);
	free(*word);
	return (NULL);
}
