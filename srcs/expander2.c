/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 11:56:06 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/15 16:09:05 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_v_name(char *a, t_quotes *quotes, int j, int *i1)
{
	char	*var;

	var = ft_malloc(ft_strlen(&(a[*i1])));
	if (quotes->dq == true)
	{
		while (a[*i1] != '\"' && a[*i1] != '$' && a[*i1] != '\'')
		{
			if (a[*i1] != '\'')
				var[*i1 - j - 1] = a[*i1];
			(*i1)++;
		}
	}
	else
	{
		while (a[*i1] != '\0' && a[*i1]
			!= '$' && a[*i1] != '\'' && a[(*i1)] != '\"')
		{
			var[*i1 - j - 1] = a[*i1];
			(*i1)++;
		}
	}
	var[*i1 - j - 1] = '\0';
	if (quotes->dq == true)
		(*i1)--;
	return (var);
}

char	*copy_first(int *i, char *argument, int j)
{
	char	*arg_var;

	arg_var = ft_malloc(ft_strlen(argument));
	while (++(*i) < j)
		arg_var[*i] = argument[*i];
	arg_var[*i] = '\0';
	return (arg_var);
}

static char	*get_value2(char *arg_var, char *var, t_list *my_envp)
{
	t_list	*var_list;
	char	*temp;

	if (var[0] == '?')
	{
		free (arg_var);
		arg_var = ft_strjoin_free(ft_itoa(errno), &(var[1]));
	}
	var_list = check_already_there(&var, my_envp, ft_strlen(var), 0);
	if (var_list)
	{
		temp = get_value_variable(my_envp, var);
		arg_var = ft_strjoin_free(arg_var, temp);
		free (temp);
	}
	return (arg_var);
}

char	*get_v_value(char *argument, t_list *my_envp, t_quotes *quotes, int j)
{
	char	*arg_var;
	char	*var;
	int		i;

	i = -1;
	quotes->space = false;
	if (quotes->sq == true)
		return (argument);
	arg_var = copy_first(&i, argument, j);
	i++;
	var = get_v_name(argument, quotes, j, &i);
	if (var[0] == '\0')
		arg_var = ft_strjoin_free(arg_var, &argument[j + 1]);
	arg_var = get_value2(arg_var, var, my_envp);
	free(var);
	if (argument[i] == '\'' || argument[i] == '$')
	{
		var = ft_substr(argument, i, 1);
		arg_var = ft_strjoin_free(arg_var, var);
		free (var);
	}
	if (argument[i + 1] != '\0')
		arg_var = ft_strjoin_free(arg_var, &(argument[i + 1]));
	free (argument);
	return (arg_var);
}

void	expander(t_command_table *command_table, t_list *my_envp, t_list *local)
{
	int	i;

	i = 0;
	while (i < command_table->number)
	{
		command_table->simple_command[i].arguments = get_quotes_changed(
				command_table->simple_command[i].arguments, my_envp, local);
		i++;
	}
}
