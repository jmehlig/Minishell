/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:02:48 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/26 09:28:50 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// Bei keinem zweiten Quote, wird momentan eine Fehlermeldung zurueckgegeben, es muss zwar nicht wie in bash agieren, sollte aber "nicht interpretiert" werden?
// Wenn ich in Doublequotes bin, sind Singlequotes egal!!!! Und umgekehrt!!!

void	quote_checker(char str, bool *dq, bool *sq)
{
	if (str == '\'')
	{
		if (*dq != true)
		{
			if (*sq == true)
				*sq = false;
			else
				*sq = true;
		}
	}
	if (str == '\"')
	{
		if (*sq != true)
		{
			if (*dq == true)
				*dq = false;
			else
				*dq = true;
		}
	}
}

static int	word_number(const char *str, char c, bool *dq, bool *sq)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	while (str[i] != '\0')
	{
		quote_checker(str[i], dq, sq);
		if ((str[i + 1] == c || str[i + 1] == '\0')
			&& (str[i] != c && str[i] != '\0')
			&& (*sq == false && *dq == false))
			words++;
		i++;
	}
	return (words);
}

static int	write_split(char **split, const char *str, char c, bool *dq, bool *sq)
{
	int	i;
	int	inword_count;
	int	word;

	word = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c || str[i] == '\0')
			i++;
		else
		{
			inword_count = 0;
			while (((str[i + inword_count] != c
					|| (*dq == true || *sq == true))
					&& (str[i + inword_count] != '\0')))
			{
				quote_checker(str[i + inword_count], dq, sq);
				inword_count++;
			}
			split[word] = (char *)malloc(sizeof(char) * (inword_count + 1));
			if (!split)
				return (-1);
			split[word] = ft_substr(str, i, inword_count);
			//printf("%s\n%i\n%i\n", split[word], i, inword_count);
			i = i + inword_count;
			word++;
		}
	}
	if (*dq == true || *sq == true)
	{
		perror("There is no closing quote");
		exit(1);
	}
	return (0);
}

char	**ft_split_quotes(const char *str, char c)
{
	char	**res;
	int		words;
	bool	dq;
	bool	sq;

	dq = false;
	sq = false;
	if (str == NULL)
		return (NULL);
	words = word_number(str, c, &dq, &sq);
	dq = false;
	sq = false;
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[words] = 0;
	if (write_split(res, str, c, &dq, &sq) == -1)
		return (NULL);
	return (res);
}
