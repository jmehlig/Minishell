/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 11:12:07 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/11 14:13:34 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstfree(t_list **lst)
{
	t_list	*temp;

	while ((*lst)->content != NULL)
	{
		write(1, temp->content, 15);
		write(1, "\n", 1);
		temp = *lst;
		*lst = (*lst)->next;
		ft_lstdelnode(lst, temp->content);
	}
}
