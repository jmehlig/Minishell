/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2d_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 15:38:18 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/08 15:39:39 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2d_array(void **array, size_t size_array)
{
	size_t	i;

	if (array == NULL)
		return ;
	i = 0;
	while (i < size_array)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
