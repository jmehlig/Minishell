/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor10.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:19:10 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:42:37 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
single_built_in_check
*/

#include "minishell.h"

static	bool	set_local_variable(char *argument, t_list **local)
{
	char	*variable_name;
	char	*variable_value;
	bool	success_add_entry;

	variable_name = get_name_variable(argument);
	if (variable_name == NULL)
		return (false);
	variable_value = pull_value_variable(argument);
	if (variable_value == NULL)
	{
		free(variable_name);
		return (false);
	}
	success_add_entry = add_entry(variable_name, variable_value, local);
	free(variable_name);
	free(variable_value);
	return (success_add_entry);
}

static	bool	execute_allocation(char *argument, t_list **my_envp,
	t_list **local)
{
	char	*var_name;

	var_name = get_name_variable(argument);
	if (var_name == NULL)
	{
		write_errormessage(NULL);
		return (false);
	}
	if (check_already_there(&argument, *my_envp, ft_strlen(var_name), 0))
	{
		update_value_variable(var_name, &(argument[ft_strlen(var_name) + 1]),
			my_envp);
	}
	else
		set_local_variable(argument, local);
	free(var_name);
	if (errno == ENOMEM)
	{
		write_errormessage(NULL);
		return (false);
	}
	return (true);
}

static	int	go_over_allocations(char **arguments, t_list **my_envp,
	t_list **local)
{
	int	i;

	i = 0;
	while (arguments[i] != NULL)
	{
		if (execute_allocation(arguments[i], my_envp, local) == false)
			return (1);
		i++;
	}
	return (0);
}

static	int	called_single_builtin_function(char **arguments,
	t_command_table *command_table, t_list **my_envp, t_list **local)
{
	if (ft_strncmp(arguments[0], "cd", 3) == 0)
	{
		errno = my_cd(arguments, my_envp);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "export", 7) == 0)
	{
		errno = my_export(arguments, my_envp, local);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "unset", 6) == 0)
	{
		errno = my_unset(arguments, my_envp, local);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "exit", 5) == 0)
	{
		errno = my_exit(arguments, command_table, my_envp, local);
		return (1);
	}
	return (0);
}

int	called_single_builtin(char **arguments, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	if (check_only_allocations(arguments))
	{
		errno = go_over_allocations(arguments, my_envp, local);
		return (1);
	}
	return (called_single_builtin_function(arguments, command_table, my_envp,
			local));
}
