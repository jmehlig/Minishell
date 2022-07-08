/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 16:54:54 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/23 09:57:41 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
create create_child_process
*/

#include "minishell.h"

static	bool	check_call_signal_handling(t_simple_command simple_command)
{
	return (simple_command.arguments[0] != NULL
		&& ft_strncmp(simple_command.arguments[0], "./minishell", 12) != 0);
}

static	bool	check_dont_execute(t_simple_command simple_command)
{
	bool	check_empty_command;
	bool	check_zero_command;

	if (simple_command.fd_failed)
	{
		errno = EXIT_FAILURE;
		return (true);
	}
	check_empty_command = simple_command.arguments[INDEX_COMMANDNAME] == NULL;
	if (check_empty_command)
		return (true);
	check_zero_command = (ft_strlen(simple_command.arguments[INDEX_COMMANDNAME])
			== 0);
	if (check_zero_command)
	{
		errno = ERRNO_COMMANDNOTFOUND;
		write_errormessage("");
	}
	return (check_zero_command);
}

int	create_child_process(int command_id, t_command_table *command_table,
	t_collection_list collect_list, int fd_to_close)
{
	int	process_id;
	int	errno_buffer;

	errno_buffer = errno;
	process_id = fork();
	if (check_call_signal_handling(
			command_table->simple_command[command_id]))
		signal_handling(BLOCKING);
	if (process_id == -1)
		write_errormessage(NULL);
	else if (process_id == 0)
	{
		close(fd_to_close);
		errno = errno_buffer;
		if (check_dont_execute(command_table->simple_command[command_id]))
		{
			exit_executor(errno, command_table, collect_list.pointer_my_envp,
				collect_list.pointer_local);
		}
		execute_simple_command(command_id, command_table,
			collect_list.pointer_my_envp, collect_list.pointer_local);
	}
	return (process_id);
}
