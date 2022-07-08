/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 15:28:51 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/13 16:41:08 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
utils for builtins
*/

bool	check_valid_variable_name(char *variable, int size_variable_name)
{
	bool	check_valid_beginning;
	bool	check_valid_character;
	int		i;

	if (size_variable_name < 1)
		return (false);
	check_valid_beginning = (ft_isalpha(variable[0]) || variable[0] == '_');
	if (check_valid_beginning == false)
		return (false);
	i = 1;
	while (i < size_variable_name)
	{
		check_valid_character = (ft_isalnum(variable[i]) || variable[i] == '_');
		if (check_valid_character == false)
			return (false);
		i++;
	}
	return (true);
}

bool	check_valid_variable_allocation(char *s)
{
	int	size;
	int	i;

	size = ft_strlen(s);
	if (size == 0)
		return (false);
	if (s[0] == '=')
		return (false);
	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (check_valid_variable_name(s, i));
		i++;
	}
	return (false);
}

static t_list	*free_and_return(char *var_name, char *join, t_list *temp)
{
	free (var_name);
	free (join);
	return (temp);
}

t_list	*check_already_there(char **arguments, t_list *my_envp, int i, int j)
{
	t_list	*temp;
	char	*var_name;
	char	*join;

	temp = my_envp;
	var_name = ft_substr(arguments[j], 0, i);
	join = ft_strjoin(var_name, "=");
	while (temp->next)
	{
		if (ft_strncmp(temp->content, join,
				ft_strlen(var_name) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp != 0)
	{
		if (ft_strncmp(temp->content, join,
				ft_strlen(var_name) + 1) == 0)
			return (free_and_return(var_name, join, temp));
	}
	return (free_and_return(var_name, join, NULL));
}
