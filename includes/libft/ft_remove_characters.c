/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_characters.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:41:03 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 18:41:19 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_remove_characters(const char *str, int index_start,
	int amount_to_remove)
{
	char	*new_str;
	char	*first_half;
	char	*second_half;

	if (str == NULL)
		return (NULL);
	if (amount_to_remove == 0)
		return (ft_strdup(str));
	first_half = ft_substr(str, 0, index_start);
	if (first_half == NULL)
		return (NULL);
	second_half = ft_substr(str, index_start + amount_to_remove,
			ft_strlen(str) - index_start - amount_to_remove);
	if (second_half == NULL)
	{
		free(first_half);
		return (NULL);
	}
	new_str = ft_strjoin(first_half, second_half);
	free(first_half);
	free(second_half);
	return (new_str);
}
