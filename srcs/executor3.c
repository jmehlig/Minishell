/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:07:38 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 12:45:01 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
redirect_command_fd
*/

#include "minishell.h"

static	bool	redirect_command_input(t_simple_command simple_command,
	t_fd_utils *fd_utils)
{
	int		return_dup2;
	bool	check_command_input_redirection;

	check_command_input_redirection = simple_command.fd_in != -1;
	if (check_command_input_redirection)
	{
		return_dup2 = dup2(simple_command.fd_in, STDIN_FILENO);
		close(simple_command.fd_in);
	}
	else
		return_dup2 = dup2(fd_utils->command_fd.in, STDIN_FILENO);
	close(fd_utils->command_fd.in);
	return (return_dup2 != -1);
}

static	bool	set_last_command_output(t_simple_command simple_command,
	t_fd_utils *fd_utils)
{
	bool	check_command_output_redirection;

	check_command_output_redirection = simple_command.fd_out != -1;
	if (check_command_output_redirection)
		fd_utils->command_fd.out = simple_command.fd_out;
	else
		fd_utils->command_fd.out = dup(fd_utils->default_fd.out);
	return (fd_utils->command_fd.out != -1);
}

static	void	set_command_output(t_simple_command simple_command,
	t_fd_utils *fd_utils, int pipe_fd_out)
{
	bool	check_command_output_redirection;

	check_command_output_redirection = simple_command.fd_out != -1;
	if (check_command_output_redirection)
	{
		close(pipe_fd_out);
		fd_utils->command_fd.out = simple_command.fd_out;
	}
	else
		fd_utils->command_fd.out = pipe_fd_out;
}

static	bool	redirect_command_output(t_fd_utils *fd_utils)
{
	int	success_dup2;

	success_dup2 = dup2(fd_utils->command_fd.out, STDOUT_FILENO) != -1;
	close(fd_utils->command_fd.out);
	return (success_dup2);
}

bool	redirect_command_fd(int command_id, t_simple_command simple_command,
	t_fd_utils *fd_utils, int number_of_commands)
{
	bool	check_last_command;
	int		pipe_fd[2];

	if (redirect_command_input(simple_command, fd_utils) == false)
		return (false);
	check_last_command = command_id == number_of_commands - 1;
	if (check_last_command)
	{
		if (set_last_command_output(simple_command, fd_utils) == false)
			return (false);
	}
	else
	{
		if (pipe(pipe_fd) == -1)
			return (false);
		fd_utils->command_fd.in = pipe_fd[STDIN_FILENO];
		set_command_output(simple_command, fd_utils, pipe_fd[STDOUT_FILENO]);
	}
	return (redirect_command_output(fd_utils));
}
