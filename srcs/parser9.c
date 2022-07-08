/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser9.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 09:13:49 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 18:22:41 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils for parser
*/

#include "minishell.h"

int	calc_number_token_in_range(t_parse_range parse_range)
{
	if (parse_range.begin < 0 || parse_range.end < 0)
		return (0);
	return (parse_range.end - parse_range.begin);
}

t_parse_range	create_parse_range(int range_begin, int range_end)
{
	t_parse_range	new_parse_range;

	new_parse_range.begin = range_begin;
	new_parse_range.end = range_end;
	return (new_parse_range);
}

t_parse_range	create_no_redirection_parse_range(void)
{
	t_parse_range	no_redirection_parse_range;

	no_redirection_parse_range.begin = PARSERANGE_NOREDIRECTION;
	no_redirection_parse_range.end = PARSERANGE_NOREDIRECTION;
	return (no_redirection_parse_range);
}

bool	check_no_redirection_parse_range(t_parse_range parse_range)
{
	return (parse_range.begin == PARSERANGE_NOREDIRECTION
		|| parse_range.end == PARSERANGE_NOREDIRECTION);
}

t_parse_range	create_error_redirection_parse_range(int index_wrong_token)
{
	t_parse_range	error_redirection_parse_range;

	error_redirection_parse_range.begin = PARSERANGE_ERROR;
	error_redirection_parse_range.end = index_wrong_token;
	return (error_redirection_parse_range);
}
