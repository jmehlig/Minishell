/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:47:07 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:16:54 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
exeucute commands
*/

#include "minishell.h"

static	t_collection_list	init_collection_list(t_list **my_envp,
	t_list **local)
{
	t_collection_list	collect_list;

	collect_list.pointer_my_envp = my_envp;
	collect_list.pointer_local = local;
	return (collect_list);
}

static	bool	prep_childs(int command_id,
	t_command_table *command_table, t_fd_utils *fd_utils,
	t_collection_list collect_list)
{
	if (redirect_command_fd(command_id,
			command_table->simple_command[command_id], fd_utils,
			command_table->number) == false)
	{
		write_errormessage(NULL);
		return (false);
	}
	if (single_built_in_check(command_id, command_table,
			collect_list.pointer_my_envp, collect_list.pointer_local))
	{
		return (false);
	}
	return (true);
}

int	wait_child_processes(t_command_table *command_table)
{
	int	last_statuscode;
	int	command_id;

	command_id = 0;
	last_statuscode = PROCESSID_MAIN;
	while (command_id < command_table->number)
	{
		if (command_table->simple_command[command_id].process_id
			== PROCESSID_MAIN)
		{
			last_statuscode = PROCESSID_MAIN;
			break ;
		}
		waitpid(command_table->simple_command[command_id].process_id,
			&last_statuscode, 0);
		command_id++;
	}
	return (last_statuscode);
}

int	execute_commands(t_command_table *command_table, t_fd_utils *fd_utils,
	t_list **my_envp, t_list **local)
{
	int					command_id;
	t_collection_list	collect_list;

	command_id = 0;
	while (command_id < command_table->number)
	{
		collect_list = init_collection_list(my_envp, local);
		if (!prep_childs(command_id, command_table, fd_utils, collect_list))
		{
			command_table->simple_command[command_id].process_id
				= PROCESSID_MAIN;
			break ;
		}
		command_table->simple_command[command_id].process_id
			= create_child_process(command_id, command_table, collect_list,
				fd_utils->command_fd.in);
		if (command_table->simple_command[command_id].process_id
			== PROCESSID_MAIN)
			break ;
		command_id++;
	}
	if (restore_default_fds(fd_utils->default_fd) == false)
		write_errormessage(NULL);
	return (wait_child_processes(command_table));
}
