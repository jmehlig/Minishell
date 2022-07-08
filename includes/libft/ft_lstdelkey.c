/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelkey.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:42:26 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 18:43:00 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelkey(t_list **lst, void *key, void (*del)(void *))
{
	t_list	*temp;
	t_list	*prev;

	temp = *lst;
	prev = NULL;
	if (temp != NULL && (ft_strncmp(temp->content, key,
				ft_strlen(temp->content)) == 0))
	{
		*lst = temp->next;
	}
	else
	{
		while (temp != NULL && (ft_strncmp(temp->content, key,
					ft_strlen(temp->content)) != 0))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return ;
		prev->next = temp->next;
	}
	temp->next = NULL;
	ft_lstclear(&temp, del);
}
