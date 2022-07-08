/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:37:34 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/10 10:47:57 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils parse command
*/

#include "minishell.h"

bool	check_redirection_token(t_token token)
{
	return (token.type == TOKENTYPE_INFILE || token.type == TOKENTYPE_OUTFILE
		|| token.type == TOKENTYPE_HEREDOC || token.type == TOKENTYPE_APPEND);
}

bool	check_io_change(t_token *token, t_parse_range parse_range)
{
	bool	check_io_change_front;
	bool	check_io_change_back;
	int		index_redirection_token_end;

	if (calc_number_token_in_range(parse_range) < MINTOKEN_REDIRECTION)
		return (false);
	check_io_change_front = check_redirection_token(token[parse_range.begin]);
	index_redirection_token_end = parse_range.end - MINTOKEN_REDIRECTION;
	check_io_change_back = check_redirection_token(
			token[index_redirection_token_end]);
	return (check_io_change_front || check_io_change_back);
}

bool	check_found_redirection(int index_last_redirection, int index_begin)
{
	return (index_last_redirection != index_begin);
}
