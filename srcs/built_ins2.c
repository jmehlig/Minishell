/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 21:16:18 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/16 14:58:01 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	uint8_t	get_exitcode_from_argument(char *argument)
{
	int	i;

	i = 0;
	if (argument[0] == '-')
		i++;
	while (argument[i])
	{
		if (ft_isdigit(argument[i]) == 0)
		{
			errno = ERRNO_EXITNUMERIC;
			write_errormessage_builtin("exit", argument);
			return (255);
		}
		i++;
	}
	return ((uint8_t) ft_atoi(argument));
}

int	my_exit(char **arguments, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	bool	check_no_arguments;
	bool	check_one_argument;
	uint8_t	exit_code;

	check_no_arguments = arguments[1] == NULL;
	check_one_argument = (check_no_arguments == false && arguments[2] == NULL);
	write(STDERR_FILENO, "exit\n", 5);
	if (check_no_arguments)
		exit_code = 0;
	if (check_one_argument)
		exit_code = get_exitcode_from_argument(arguments[1]);
	if (check_no_arguments == true || check_one_argument == true)
		exit_executor(exit_code, command_table, my_envp, local);
	else
	{
		errno = ERRNO_EXIT;
		write_errormessage_builtin("exit", NULL);
		return (1);
	}
	return (0);
}

int	my_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		write_errormessage_builtin("pwd", NULL);
		return (1);
	}
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(path);
	return (0);
}

int	my_env(t_list *my_envp, char **arguments)
{
	if (!my_envp)
		return (0);
	if (arguments[1] != NULL)
	{
		errno = 7;
		write_errormessage("env");
		return (1);
	}
	while (my_envp->next)
	{
		if (!ft_strncmp(arguments[0], "export", 7))
			ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(my_envp->content, 1);
		ft_putchar_fd('\n', 1);
		my_envp = my_envp->next;
	}
	if (!ft_strncmp(arguments[0], "export", 7))
		ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(my_envp->content, 1);
	ft_putchar_fd('\n', 1);
	return (0);
}
