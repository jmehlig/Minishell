/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:45:11 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/10 14:50:26 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils parse io change
*/

#include "minishell.h"

bool	check_only_one_redirection(t_parse_range parse_range_front,
	t_parse_range parse_range_back)
{
	int			number_token_in_range_front;
	int			number_token_in_range_back;

	number_token_in_range_front = calc_number_token_in_range(parse_range_front);
	number_token_in_range_back = calc_number_token_in_range(parse_range_back);
	return (number_token_in_range_front + number_token_in_range_back
		== MINTOKEN_REDIRECTION);
}
