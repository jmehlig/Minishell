/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:10:04 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:10:05 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*ptr;
	t_list	*new;

	ptr = 0;
	while (lst != 0)
	{
		new = ft_lstnew(f(lst -> content));
		if (new == 0)
			ft_lstclear(&new, del);
		else
			ft_lstadd_back(&ptr, new);
		lst = lst -> next;
	}
	return (ptr);
}
