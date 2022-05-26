/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 11:56:06 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/26 13:38:43 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable_value(char *argument, t_list *my_envp, bool *sq, bool *dq, int j)
{
	char	*arg_var;
	char	*var;
	int		i;
	t_list	*var_list;
	bool	space;

	i = 0;
	space = false;
	if (*sq == true)
		return (argument);
	arg_var = malloc(ft_strlen(argument));
	if (!arg_var)
		return (NULL);
	while (i < j)
	{
		arg_var[i] = argument[i];
		i++;
	}
	arg_var[i] = '\0';
	i++;
	var = malloc(ft_strlen(&(argument[i]) + 1));
	if (!var)
		return (NULL);
	if (*dq == true)
	{
		while (argument[i] != '\"' && argument[i] != '$')
		{
			if (argument[i] != '\'')
				var[i - j - 1] = argument[i];
			i++;
		}
		if (argument[i] == '\"')
		{
			if (argument[i - 1] == '\'')
				*sq = true;
			i++;
			space = true;
		}
	}
	else
	{
		while (argument[i] != '\0' && argument[i] != '$')
		{
			var[i - j - 1] = argument[i];
			i++;
		}
	}
	var[i - j] = '\0';
	var_list = check_already_there(&var, my_envp, ft_strlen(var), 0);
	if (var_list && space == true)
		arg_var = ft_strjoin(arg_var, &(var_list->content[i - j - 1]));
	else if (var_list && space != true)
		arg_var = ft_strjoin(arg_var, &(var_list->content[i - j]));
	else
	{
		//Hier macht bash nur eine neue Zeile???
		// Oder besser loescht einfach $ und alles danach
	}
	free(var);
	if (*sq == true && *dq == true)
	{
		arg_var = ft_strjoin(arg_var, "\'");
		*sq = false;
	}
	if (argument[i] != '\0')
		arg_var = ft_strjoin(arg_var, &(argument[i]));
	return (arg_var);
}

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
	return (copy);
}

char	**get_quotes_changed(char **arguments, t_list *my_envp)
{
	int		i;
	int		j;
	bool	dq;
	bool	sq;
	//char	*temp;

	i = 0;
	dq = false;
	sq = false;
	while (arguments[i] != NULL)
	{
		j = 0;
		while (arguments[i][j] != '\0')
		{
			if (arguments[i][j] == '\'' && dq == false)
			{
				quote_checker(arguments[i][j], &dq, &sq);
				arguments[i] = delete_quote(arguments[i], j);
			}
			else if (arguments[i][j] == '\"' && sq == false)
			{
				quote_checker(arguments[i][j], &dq, &sq);
				arguments[i] = delete_quote(arguments[i], j);
			}
			if (arguments[i][j] == '$' && sq == false)
				arguments[i] = get_variable_value(&(arguments[i][0]), my_envp, &sq, &dq, j);
			j++;
		}
		i++;
	}
	return (arguments);
}
