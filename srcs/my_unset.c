/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:40:48 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/15 08:11:56 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	unset_variable(char *argument, t_list **my_envp, t_list **local)
{
	bool	check_deleted_from_myenvp;

	check_deleted_from_myenvp = delete_variable(argument, my_envp);
	if (errno == ENOMEM)
		return (false);
	if (check_deleted_from_myenvp == false)
	{
		delete_variable(argument, local);
		if (errno == ENOMEM)
			return (false);
	}
	return (true);
}

int	my_unset(char **arguments, t_list **my_envp, t_list **local)
{
	int		return_value;
	int		i;

	return_value = 0;
	i = 0;
	while (arguments[i] != NULL)
	{
		if (check_valid_variable_name(arguments[i], ft_strlen(arguments[i])))
		{
			if (unset_variable(arguments[i], my_envp, local) == false)
			{
				write_errormessage_builtin("unset", NULL);
				return (1);
			}
		}
		else
		{
			errno = ERRNO_IDENTIFIER;
			write_errormessage_builtin("unset", arguments[i]);
			return_value = 1;
		}
		i++;
	}
	return (return_value);
}
