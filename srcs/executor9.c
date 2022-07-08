/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor9.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 08:56:09 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:34:12 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
builtins
*/

#include "minishell.h"

bool	check_only_allocations(char **arguments)
{
	int	i;

	i = 0;
	while (arguments[i] != NULL)
	{
		if (check_valid_variable_allocation(arguments[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

int	built_in_check(char **arguments, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	if (arguments[0] == NULL)
		return (0);
	if (called_single_builtin(arguments, command_table, my_envp, local))
		return (1);
	else if (ft_strncmp(arguments[0], "echo", 5) == 0)
	{
		errno = ft_echo(arguments);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "pwd", 4) == 0)
	{
		errno = my_pwd();
		return (1);
	}
	else if (ft_strncmp(arguments[0], "env", 4) == 0)
	{
		errno = my_env(*my_envp, arguments);
		return (1);
	}
	return (0);
}

/*
these built ins are not allowed to be in a pipe,
because the change my_envp or local pointer
*/
int	single_built_in_check(int command_id, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	char	**arguments;

	if (command_table->number != 1)
		return (0);
	arguments = command_table->simple_command[command_id].arguments;
	if (arguments[0] == NULL)
		return (0);
	return (called_single_builtin(arguments, command_table, my_envp, local));
}
