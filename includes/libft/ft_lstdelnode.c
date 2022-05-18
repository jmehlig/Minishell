/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelnode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 19:03:30 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/10 19:33:28 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelnode(t_list **lst, void *key)
{
	t_list	*temp;
	t_list	*prev;

	temp = *lst;
	prev = NULL;
	if (temp != NULL && (ft_strncmp(temp->content, key, ft_strlen(temp->content)) == 0))
	{
		*lst = temp->next;
		free(temp);
	}
	else
	{
		while (temp != NULL && (ft_strncmp(temp->content, key, ft_strlen(temp->content)) != 0))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return;
		prev->next = temp->next;
		free(temp);
	}
}
