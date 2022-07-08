/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:58:03 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 13:59:19 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
execute simple command
*/

#include "minishell.h"

static	char	**duplicate_arguments(t_simple_command simple_command)
{
	char	**dup_arguments;
	int		i;

	dup_arguments = (char **) malloc((simple_command.number + 1)
			* sizeof(char *));
	if (dup_arguments == NULL)
		return (NULL);
	i = 0;
	while (i < simple_command.number)
	{
		dup_arguments[i] = ft_strdup(simple_command.arguments[i]);
		if (dup_arguments[i] == NULL)
		{
			ft_free_2d_array_nullterminated((void **) dup_arguments);
			return (NULL);
		}
		i++;
	}
	dup_arguments[i] = NULL;
	return (dup_arguments);
}

static	char	**create_env_array(t_list *my_envp)
{
	char	**env;
	int		size_envp;
	int		i;

	size_envp = ft_lstsize(my_envp);
	env = (char **) malloc((size_envp + 1) * sizeof(char *));
	if (env == NULL)
		return (NULL);
	i = 0;
	while (i < size_envp)
	{
		env[i] = ft_strdup((char *) my_envp->content);
		if (env[i] == NULL)
		{
			ft_free_2d_array_nullterminated((void **) env);
			return (NULL);
		}
		my_envp = my_envp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

static	void	exit_before_execve(t_command_table *command_table,
	t_list **my_envp, t_list **local, char **arguments)
{
	ft_free_2d_array_nullterminated((void **) arguments);
	exit_executor(errno, command_table, my_envp, local);
}

void	execute_simple_command(int command_id, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	char	**arguments;
	char	*program_path;
	char	**env;

	arguments = duplicate_arguments(command_table->simple_command[command_id]);
	if (arguments == NULL)
		exit_executor(errno, command_table, my_envp, local);
	if (built_in_check(arguments, command_table, my_envp, local) == 1)
		exit_before_execve(command_table, my_envp, local, arguments);
	program_path = create_programpath(arguments[INDEX_COMMANDNAME], *my_envp);
	if (program_path == NULL)
		exit_before_execve(command_table, my_envp, local, arguments);
	env = create_env_array(*my_envp);
	if (env == NULL)
	{
		free(program_path);
		exit_before_execve(command_table, my_envp, local, arguments);
	}
	free_executor(command_table, my_envp, local);
	execve(program_path, arguments, env);
	free(program_path);
	ft_free_2d_array_nullterminated((void **) env);
	ft_free_2d_array_nullterminated((void **) arguments);
	write_errormessage(NULL);
	exit(errno);
}
