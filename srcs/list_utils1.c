/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 11:11:29 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 07:23:22 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- variable_name:		string before equal sign
- variable_identifier:	name + equal sign
- variable_value:		string after equal sign
- variable:				variable_name + equal sign + variable_value
*/

int	check_found_variable_identifier(const char *str_to_check,
	const char *variable_name)
{
	bool	check_compare_strings;
	bool	check_found_equal_sign;
	size_t	size_variable_name;
	int		index_begin_value;

	size_variable_name = ft_strlen(variable_name);
	check_compare_strings = ft_strncmp(str_to_check, variable_name,
			size_variable_name) == 0;
	if (check_compare_strings == false)
		return (-1);
	check_found_equal_sign = str_to_check[size_variable_name] == '=';
	if (check_found_equal_sign == false)
		return (-1);
	index_begin_value = size_variable_name + 1;
	return (index_begin_value);
}

char	*create_variable(const char *variable_name, const char *variable_value)
{
	char	*variable;
	char	*variable_identifier;

	variable_identifier = ft_strjoin(variable_name, "=");
	if (variable_identifier == NULL)
		return (NULL);
	variable = ft_strjoin(variable_identifier, variable_value);
	free(variable_identifier);
	return (variable);
}

char	*get_name_variable(const char *variable)
{
	char	*var_name;
	int		size_variable_name;

	if (variable == NULL || ft_strlen(variable) == 0)
		return (NULL);
	size_variable_name = 0;
	while (variable[size_variable_name])
	{
		if (variable[size_variable_name] == '=')
		{
			var_name = ft_substr(variable, 0, size_variable_name);
			return (var_name);
		}
		size_variable_name++;
	}
	return (NULL);
}

char	*get_value_variable(t_list *list, const char *variable_name)
{
	char	*value_variable;
	char	*entry_to_check;
	int		index_begin_value;

	if (variable_name == NULL || ft_strlen(variable_name) == 0)
		return (NULL);
	while (list != NULL)
	{
		entry_to_check = (char *) list->content;
		index_begin_value = check_found_variable_identifier(entry_to_check,
				variable_name);
		if (index_begin_value != -1)
		{
			value_variable = ft_substr(entry_to_check, index_begin_value,
					ft_strlen(entry_to_check) - index_begin_value);
			return (value_variable);
		}
		list = list->next;
	}
	value_variable = ft_strdup("");
	return (value_variable);
}

char	*get_variable(t_list *list, const char *variable_name)
{
	char	*variable;
	char	*entry_to_check;
	bool	found_env_variable;

	if (variable_name == NULL || ft_strlen(variable_name) == 0)
		return (NULL);
	variable = NULL;
	while (list != NULL)
	{
		entry_to_check = (char *) list->content;
		found_env_variable = check_found_variable_identifier(entry_to_check,
				variable_name) != -1;
		if (found_env_variable)
		{
			variable = ft_strdup(entry_to_check);
			break ;
		}
		list = list->next;
	}
	return (variable);
}
