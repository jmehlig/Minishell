/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser14.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:13:34 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 08:28:29 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
create here doc
*/

#include "minishell.h"

void	handle_error_here_doc(t_simple_command *simple_command)
{
	write_errormessage(NULL);
	unlink(FILENAME_HEREDOC);
	simple_command->fd_in = -1;
	simple_command->fd_failed = true;
}

static	bool	fill_here_doc(int heredoc_fd, const char *terminater)
{
	char	*next_line;

	signal_handling(HEREDOC);
	next_line = readline("> ");
	while (next_line != NULL && ft_strncmp(next_line, terminater,
			ft_strlen(terminater) + 1) != 0)
	{
		write(heredoc_fd, next_line, ft_strlen(next_line));
		write(heredoc_fd, "\n", 1);
		free(next_line);
		next_line = readline("> ");
	}
	close(heredoc_fd);
	if (errno == ENOMEM)
		return (false);
	free(next_line);
	return (true);
}

void	create_here_doc(t_simple_command *simple_command,
	const char *terminater)
{
	int		heredoc_fd;
	bool	success_fill_heredoc;

	if (simple_command->fd_in != -1)
		close(simple_command->fd_in);
	heredoc_fd = open(FILENAME_HEREDOC, O_CREAT | O_WRONLY | O_TRUNC,
			MODE_CREATEFILE);
	if (heredoc_fd == -1)
	{
		handle_error_here_doc(simple_command);
		return ;
	}
	success_fill_heredoc = fill_here_doc(heredoc_fd, terminater);
	if (success_fill_heredoc == false)
		handle_error_here_doc(simple_command);
}
