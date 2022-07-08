/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:02:48 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/23 09:19:44 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_checker(char str, t_quotes *quotes)
{
	if (str == '\'')
	{
		if (quotes->dq != true)
		{
			if (quotes->sq == true)
				quotes->sq = false;
			else
				quotes->sq = true;
		}
	}
	if (str == '\"')
	{
		if (quotes->sq != true)
		{
			if (quotes->dq == true)
				quotes->dq = false;
			else
				quotes->dq = true;
		}
	}
}

static int	word_number(const char *str, t_quotes *quotes)
{
	int	i;
	int	words;
	int	size_accepted_symbol;

	words = 0;
	i = 0;
	while (str[i] != '\0')
	{
		quote_checker(str[i], quotes);
		size_accepted_symbol = check_accepted_symbols(&str[i]);
		if ((size_accepted_symbol > 0)
			&& (quotes->sq == false && quotes->dq == false))
		{
			i = i + size_accepted_symbol - 1;
			words++;
		}
		else if (((check_splitter(str[i + 1]) == true) || str[i + 1] == '\0')
			&& ((check_splitter(str[i]) == false) && str[i] != '\0')
			&& (quotes->sq == false && quotes->dq == false))
			words++;
		i++;
	}
	return (words);
}

static int	ft_no_closing_quote(char **split, int word)
{
	int	i;

	i = 0;
	errno = 1;
	while (i < word)
	{
		free(split[i]);
		i++;
	}
	errno = ERRNO_QUOTE;
	return (-1);
}

static int	write_split(char **split, char *str, t_quotes *quotes)
{
	int	i;
	int	inword_count;
	int	word;

	word = 0;
	i = -1;
	while (str[++i] != '\0')
	{
		if (ft_iswhitespace(str[i]) == false && str[i] != '\0')
		{
			inword_count = 0;
			inword_count = check_accepted_symbols(&str[i]);
			if (inword_count == 0)
				inword_count = count_word_chars(str, i, quotes);
			split[word++] = ft_substr(str, i, inword_count);
			i = i + inword_count - 1;
		}
	}
	if (quotes->dq == true || quotes->sq == true)
		return (ft_no_closing_quote(split, word));
	return (0);
}

char	**ft_split_quotes(char *str)
{
	char		**res;
	int			words;
	t_quotes	quotes;

	quotes.dq = false;
	quotes.sq = false;
	if (str == NULL)
		return (NULL);
	words = word_number(str, &quotes);
	quotes.dq = false;
	quotes.sq = false;
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[words] = 0;
	if (write_split(res, str, &quotes) == -1)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
