/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:05:41 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 12:45:08 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
fd utils
*/

#include "minishell.h"

bool	init_fd_utils(t_fd_utils *fd_utils)
{
	fd_utils->default_fd.in = dup(STDIN_FILENO);
	if (fd_utils->default_fd.in == -1)
		return (false);
	fd_utils->default_fd.out = dup(STDOUT_FILENO);
	if (fd_utils->default_fd.in == -1)
	{
		close(fd_utils->default_fd.in);
		write_errormessage(NULL);
		return (false);
	}
	return (true);
}

bool	set_first_command_input(t_fd_utils *fd_utils)
{
	bool	success_dup_fd;

	fd_utils->command_fd.in = dup(fd_utils->default_fd.in);
	success_dup_fd = fd_utils->command_fd.in != -1;
	if (success_dup_fd == false)
		write_errormessage(NULL);
	return (success_dup_fd);
}

bool	restore_default_fds(t_fd_pair default_fd)
{
	bool	success_restore_default_fds;
	bool	success_first_dup2;
	bool	success_second_dup2;

	success_first_dup2 = dup2(default_fd.in, STDIN_FILENO) != -1;
	success_second_dup2 = dup2(default_fd.out, STDOUT_FILENO) != -1;
	close(default_fd.in);
	close(default_fd.out);
	success_restore_default_fds = (success_first_dup2 && success_second_dup2);
	if (success_restore_default_fds == false)
		write_errormessage(NULL);
	return (success_restore_default_fds);
}
