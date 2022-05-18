/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:08:37 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:08:39 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_subs(char const *s, char c)
{
	int		i;
	int		del_flag;
	int		n_subs;

	i = 0;
	del_flag = 1;
	n_subs = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && del_flag == 1)
		{
			n_subs++;
			del_flag = 0;
		}
		else if (s[i] == c && del_flag == 0)
			del_flag = 1;
		i++;
	}
	return (n_subs);
}

static void	unwind(char **ptr_tab, int n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		free(ptr_tab[i]);
		i++;
	}
}

static int	fill_tab(char **ptr_tab, char *ptr, char c, int n_subs)
{
	int		i;
	int		j;
	int		start;

	i = 0;
	while (i < n_subs)
	{
		j = 0;
		while (ptr[j] == c)
			j++;
		start = j;
		while (ptr[j] != c && ptr[j] != 0)
			j++;
		ptr_tab[i] = (char *)malloc((j - start + 1) * sizeof(char));
		if (ptr_tab[i] == 0)
		{
			unwind(ptr_tab, i);
			return (0);
		}
		ft_strlcpy(ptr_tab[i], &ptr[start], j - start + 1);
		ptr = &ptr[j];
		i++;
	}
	ptr_tab[n_subs] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		n_subs;
	char	**ptr_tab;
	char	*ptr;

	n_subs = find_subs(s, c);
	ptr_tab = (char **)malloc((n_subs + 1) * sizeof(char *));
	if (ptr_tab == 0)
		return (ptr_tab);
	ptr = (char *)s;
	if (fill_tab(ptr_tab, ptr, c, n_subs) == 0)
	{
		free(ptr_tab);
		return (0);
	}
	return (ptr_tab);
}
