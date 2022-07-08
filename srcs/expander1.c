/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:33:25 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/23 09:18:18 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*delete_quote(char *argument, int j)
{
	int		i;
	char	*copy;
	int		j_space;

	i = 0;
	j_space = 0;
	copy = malloc(ft_strlen(argument) + 1);
	if (!copy)
		return (NULL);
	while (argument[i] != '\0')
	{
		if (i != j)
			copy[i - j_space] = argument[i];
		else
			j_space = 1;
		i++;
	}
	copy[i - j_space] = '\0';
	free (argument);
	return (copy);
}

static char	*quotes_del(char *argument, t_quotes *quotes, int *j)
{
	if ((argument[*j] == '\'' && quotes->dq == false)
		|| (argument[*j] == '\"' && quotes->sq == false))
	{
		quote_checker(argument[*j], quotes);
		argument = delete_quote(argument, *j);
		(*j)--;
	}
	return (argument);
}

char	*change_arguments(char *arguments, t_list *my_envp,
	t_list *local, t_quotes	quotes)
{
	int			j;
	char		*var;
	int			k;

	j = 0;
	while (arguments[j] != '\0')
	{
		arguments = quotes_del(arguments, &quotes, &j);
		if (arguments[j] == '$' && arguments[j + 1] != '\0' && !quotes.sq)
		{
			k = j + 1;
			var = get_v_name(arguments, &quotes, j, &k);
			if (var[0] == '\0')
				arguments = delete_quote(arguments, j);
			else if (variable_in_list(local, var) == true)
				arguments = get_v_value(arguments, local, &quotes, j);
			else
				arguments = get_v_value(arguments, my_envp, &quotes, j);
			free (var);
			j = 0;
		}
		j++;
	}
	return (arguments);
}

char	**get_quotes_changed(char **arguments, t_list *my_envp, t_list *local)
{
	int			i;
	t_quotes	quotes;

	quotes.dq = false;
	quotes.sq = false;
	i = 0;
	while (arguments[i] != NULL)
	{
		arguments[i] = change_arguments(arguments[i], my_envp, local, quotes);
		i++;
	}
	return (arguments);
}
