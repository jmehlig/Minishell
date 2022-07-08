/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser10.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:22:44 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 07:21:36 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils for parser
*/

#include "minishell.h"

static	t_parser_error	lookup_redirection_error(t_token token)
{
	if (token.type == TOKENTYPE_INFILE)
		return (PARSERERROR_INFILE);
	else if (token.type == TOKENTYPE_OUTFILE)
		return (PARSERERROR_OUTFILE);
	else if (token.type == TOKENTYPE_HEREDOC)
		return (PARSERERROR_HEREDOC);
	else
		return (PARSERERROR_APPEND);
}

bool	check_error_redirection(t_parse_range io_change_front,
	t_parse_range io_change_back, t_token *token, t_parser_error *parser_error)
{
	if (io_change_front.begin == PARSERANGE_ERROR)
	{
		*parser_error = lookup_redirection_error(token[io_change_front.end]);
		return (true);
	}
	if (io_change_back.begin == PARSERANGE_ERROR)
	{
		*parser_error = lookup_redirection_error(token[io_change_back.end]);
		return (true);
	}
	return (false);
}
