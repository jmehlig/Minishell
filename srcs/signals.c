/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 15:32:56 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/09 15:36:55 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_openpipe(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		ft_putstr_fd("my_minishell >> ", 1);
		errno = ERRNO_SIGNALB;
	}
}

void	signal_handling(t_signal sig_place)
{
	change_termios(true);
	if (sig_place == BEGIN)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, &handle_begin);
	}
	else if (sig_place == HEREDOC)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, &handle_begin);
	}
	else if (sig_place == BLOCKING)
	{
		signal(SIGQUIT, &handle_blocking);
		signal(SIGINT, &handle_blocking);
	}
	else if (sig_place == OPENPIPE)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, &handle_openpipe);
	}
}

void	handle_begin(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// void	handle_heredoc(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		write(1, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

void	handle_blocking(int signal)
{
	if (signal == SIGQUIT)
	{
		write(2, "^\\Quit: 3\n", 10);
		errno = 0;
	}
	if (signal == SIGINT)
	{
		write(1, "^C\n", 3);
		errno = ERRNO_SIGNALB;
	}
}

void	change_termios(bool del_written)
{
	struct termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	if (del_written)
		term.c_lflag = term.c_lflag & ~(ECHOCTL);
	else
	{
		term.c_lflag = term.c_lflag | ECHOCTL;
	}
	tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}
