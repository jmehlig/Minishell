/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 14:07:59 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 14:09:26 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	append_entry(const char *variable_name, const char *variable_value,
	t_list **list)
{
	char	*variable_identifier;
	char	*variable;
	bool	success_append_variable;

	variable_identifier = ft_strjoin(variable_name, "=");
	if (variable_identifier == NULL)
		return (false);
	variable = ft_strjoin(variable_identifier, variable_value);
	free(variable_identifier);
	if (variable == NULL)
		return (false);
	success_append_variable = append_variable(variable, list);
	free(variable);
	return (success_append_variable);
}

bool	add_entry(const char *variable_name, const char *variable_value,
	t_list **list)
{
	if (update_value_variable(variable_name, variable_value, list) == false)
	{
		if (errno == ENOMEM)
			return (false);
		if (append_entry(variable_name, variable_value, list) == false)
			return (false);
	}
	return (true);
}

bool	check_variable_has_no_value(const char *variable_value)
{
	if (errno == ENOMEM)
		return (false);
	return (variable_value == NULL || ft_strlen(variable_value) == 0);
}

bool	update_value_variable(const char *variable_name,
	const char *variable_value, t_list **list)
{
	char	*variable;
	t_list	*iterate_list;
	bool	found_env_variable;

	if (variable_name == NULL || ft_strlen(variable_name) == 0)
		return (false);
	iterate_list = *list;
	while (iterate_list != NULL)
	{
		found_env_variable = check_found_variable_identifier(
				(char *) iterate_list->content, variable_name) != -1;
		if (found_env_variable)
		{
			variable = create_variable(variable_name, variable_value);
			if (variable == NULL)
				return (false);
			free(iterate_list->content);
			iterate_list->content = (void *) variable;
			return (true);
		}
		iterate_list = iterate_list->next;
	}
	return (false);
}

char	*pull_value_variable(const char *variable)
{
	char	*value_variable;
	int		index_start_value;

	if (variable == NULL || ft_strchr(variable, '=') == NULL)
		return (NULL);
	index_start_value = 0;
	while (variable[index_start_value] != '=')
		index_start_value++;
	index_start_value++;
	value_variable = ft_substr(variable, index_start_value,
			ft_strlen(variable) - index_start_value);
	return (value_variable);
}
