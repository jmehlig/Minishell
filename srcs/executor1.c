/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 14:07:01 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:17:45 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_executor(t_command_table *command_table, t_list **my_envp,
	t_list **local)
{
	ft_lstclear(my_envp, free_content);
	ft_lstclear(local, free_content);
	free_command_table(&command_table);
	rl_clear_history();
}

void	exit_executor(int error_code, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	free_executor(command_table, my_envp, local);
	exit(error_code);
}

static	bool	check_one_command_failed_fd(t_command_table *command_table)
{
	bool	check_only_one_command_with_failed_fd;

	check_only_one_command_with_failed_fd = (command_table->number == 1
			&& command_table->simple_command[0].fd_failed);
	if (check_only_one_command_with_failed_fd)
		errno = 1;
	return (check_only_one_command_with_failed_fd);
}

void	execute(t_command_table *command_table, t_list **my_envp,
	t_list **local)
{
	t_fd_utils	fd_utils;
	int			status_code;

	if (check_one_command_failed_fd(command_table))
		return ;
	if (init_fd_utils(&fd_utils) == false)
		return ;
	if (set_first_command_input(&fd_utils) == false)
		return ;
	status_code = execute_commands(command_table, &fd_utils, my_envp,
			local);
	if (status_code != PROCESSID_MAIN)
		errno = WEXITSTATUS(status_code);
}
