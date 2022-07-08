/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_modified.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 15:31:55 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/08 16:24:09 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	calc_splits(char const *s, char c)
{
	int	splits;
	int	i;

	splits = 0;
	i = 0;
	while (s[i] != '\0' && s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			splits++;
		i++;
	}
	return (splits);
}

static	char	*get_word(char const *s, char c)
{
	char	*word;
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			break ;
		i++;
	}
	word = ft_substr(s, 0, i);
	return (word);
}

static	char	**handle_empty_str(void)
{
	char	**str_splitted;

	str_splitted = (char **) malloc(2 * sizeof(char *));
	if (!str_splitted)
		return (NULL);
	str_splitted[0] = ft_strdup("");
	if (!str_splitted[0])
	{
		free(str_splitted);
		return (NULL);
	}
	str_splitted[1] = NULL;
	return (str_splitted);
}

static	char	**create_str_splitted(char const *s, char c)
{
	char	**str_splitted;
	int		index_word;

	str_splitted = (char **) malloc((calc_splits(s, c) + 2) * sizeof(char *));
	if (!str_splitted)
		return (NULL);
	index_word = 0;
	while (*s)
	{
		if (*s != c)
		{
			str_splitted[index_word] = get_word(s, c);
			if (!str_splitted[index_word])
			{
				ft_free_2d_array((void **) str_splitted, index_word);
				return (NULL);
			}
			s += ft_strlen(str_splitted[index_word]);
			index_word++;
		}
		else
			s++;
	}
	str_splitted[index_word] = NULL;
	return (str_splitted);
}

char	**ft_split_modified(char const *s, char c)
{
	char	**str_splitted;

	if (!s)
		return (NULL);
	if (ft_strlen(s) == 0)
		str_splitted = handle_empty_str();
	else
		str_splitted = create_str_splitted(s, c);
	return (str_splitted);
}
