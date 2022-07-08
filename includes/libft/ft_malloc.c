/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 20:25:30 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/30 20:26:58 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_malloc(int size)
{
	char	*new_str;

	new_str = malloc(size + 1);
	if (!new_str)
		return (NULL);
	return (new_str);
}
