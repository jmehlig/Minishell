/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:06:15 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/15 13:32:11 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	bool	export_allocation(const char *argument, t_list **my_envp,
	t_list **local)
{
	bool	success_add_entry;
	char	*variable_name;
	char	*variable_value;

	variable_name = get_name_variable(argument);
	if (variable_name == NULL)
		return (false);
	variable_value = pull_value_variable(argument);
	if (variable_value == NULL)
	{
		free(variable_name);
		return (false);
	}
	delete_variable(variable_name, local);
	if (errno == ENOMEM)
	{
		free(variable_name);
		free(variable_value);
		return (false);
	}
	success_add_entry = add_entry(variable_name, variable_value, my_envp);
	free(variable_name);
	free(variable_value);
	return (success_add_entry);
}

static	bool	export_variable_name(const char *argument, t_list **my_envp,
	t_list **local)
{
	char	*variable_value;
	bool	success_add_entry;

	variable_value = get_value_variable(*local, argument);
	if (variable_value == NULL)
		return (false);
	if (ft_strlen(variable_value) == 0)
	{
		free(variable_value);
		return (true);
	}
	delete_variable(argument, local);
	if (errno == ENOMEM)
	{
		free(variable_value);
		return (false);
	}
	success_add_entry = add_entry(argument, variable_value, my_envp);
	free(variable_value);
	return (success_add_entry);
}

static	bool	export_argument(char *argument, t_list **my_envp,
	t_list **local, int *return_value)
{
	if (check_valid_variable_allocation(argument))
	{
		if (export_allocation(argument, my_envp, local) == false)
			return (false);
	}
	else if (check_valid_variable_name(argument, ft_strlen(argument)))
	{
		if (export_variable_name(argument, my_envp, local) == false)
			return (false);
	}
	else
	{
		errno = ERRNO_IDENTIFIER;
		write_errormessage_builtin("export", argument);
		*return_value = 1;
	}
	return (true);
}

int	my_export(char **arguments, t_list **my_envp, t_list **local)
{
	int	return_value;
	int	i;

	return_value = 0;
	if (arguments[1] == NULL)
	{
		my_env(*my_envp, arguments);
		return (return_value);
	}
	i = 1;
	while (arguments[i] != NULL)
	{
		if (!export_argument(arguments[i], my_envp, local, &return_value))
		{
			write_errormessage_builtin("export", NULL);
			return (1);
		}
		i++;
	}
	return (return_value);
}
