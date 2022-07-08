/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 09:06:36 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/23 09:19:23 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_splitter(char c)
{
	return (ft_iswhitespace(c) || c == '|' || c == '<' || c == '>');
}

int	check_accepted_symbols(const char *s)
{
	if (ft_strncmp(s, "<<", 2) == 0)
		return (2);
	else if (ft_strncmp(s, ">>", 2) == 0)
		return (2);
	else if (ft_strncmp(s, "<", 1) == 0)
		return (1);
	else if (ft_strncmp(s, ">", 1) == 0)
		return (1);
	else if (ft_strncmp(s, "|", 1) == 0)
		return (1);
	else
		return (0);
}

int	count_word_chars(char *str, int i, t_quotes *quotes)
{
	int	inword_count;

	inword_count = 0;
	while (((check_splitter(str[i + inword_count]) == false
				|| (quotes->dq == true || quotes->sq == true))
			&& (str[i + inword_count] != '\0')))
	{
		quote_checker(str[i + inword_count], quotes);
		inword_count++;
	}
	return (inword_count);
}
