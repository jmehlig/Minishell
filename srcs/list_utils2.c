/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 12:09:11 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 07:31:10 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
if variable isn't valid (doesn't have equal sign),
return values:
- true if variable is deleted from list
- false if variable is not in list or alloc error (ENOMEM)
*/

static bool	check_var_and_delete(t_list **list,
	char *variable_name, t_list *iterate_list)
{
	if (check_found_variable_identifier((char *) iterate_list->content,
			variable_name) != -1)
	{
		ft_lstdelkey(list, iterate_list->content, free_content);
		free(variable_name);
		return (true);
	}
	return (false);
}

bool	delete_variable(const char *variable, t_list **list)
{
	char	*variable_name;
	t_list	*iterate_list;

	variable_name = get_name_variable(variable);
	if (variable_name == NULL)
	{
		if (errno == ENOMEM)
			return (false);
		variable_name = ft_strdup(variable);
		if (variable_name == NULL)
			return (false);
	}
	iterate_list = *list;
	while (iterate_list != NULL)
	{
		if (check_var_and_delete(list, variable_name, iterate_list) == true)
			return (true);
		iterate_list = iterate_list->next;
	}
	free(variable_name);
	return (false);
}

bool	append_variable(const char *variable, t_list **list)
{
	t_list	*new_variable;
	char	*dup_variable;

	dup_variable = ft_strdup(variable);
	if (dup_variable == NULL)
		return (false);
	new_variable = ft_lstnew(dup_variable);
	if (new_variable == NULL)
	{
		free(dup_variable);
		return (false);
	}
	delete_variable(variable, list);
	if (errno == ENOMEM)
		return (false);
	ft_lstadd_back(list, new_variable);
	return (true);
}

bool	variable_in_list(t_list *list, char *variable_name)
{
	bool	is_there;
	char	*temp;

	is_there = false;
	temp = ft_strjoin(variable_name, "=");
	while (list != 0 && list->next)
	{
		if (ft_strncmp(temp, list->content, ft_strlen(temp)) == 0)
			is_there = true;
		list = list->next;
	}
	if (list != 0)
	{
		if (ft_strncmp(temp, list->content, ft_strlen(temp)) == 0)
			is_there = true;
	}
	free (temp);
	return (is_there);
}

void	free_content(void *content)
{
	free(content);
}
