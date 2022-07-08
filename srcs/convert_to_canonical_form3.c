/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_canonical_form3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:48:05 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 18:40:37 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_dot_dot_component(int index, const char *curpath)
{
	bool	previous_slash;
	bool	current_dot;
	bool	following_dot;
	bool	following_slash;

	if (index == 0)
		previous_slash = true;
	else
		previous_slash = curpath[index - 1] == '/';
	current_dot = curpath[index] == '.';
	following_dot = curpath[index + 1] == '.';
	if (curpath[index + 1] == '\0')
		following_slash = false;
	else
	{
		following_slash
			= (curpath[index + 2] == '\0' || curpath[index + 2] == '/');
	}
	return (previous_slash && current_dot && following_dot && following_slash);
}

static	int	find_index_previous_slash(int index, const char *curpath)
{
	int	i;

	i = index - 2;
	if (i < 0)
		return (0);
	while (curpath[i])
	{
		if (curpath[i] == '/')
			return (i);
		i--;
	}
	return (-1);
}

static	int	calc_remove_dot_dot_component(int *index, int index_previous_slash)
{
	int		amount_to_remove;
	int		index_last_char;

	index_last_char = (*index) + 1;
	amount_to_remove = index_last_char - index_previous_slash + 1;
	*index = index_previous_slash;
	return (amount_to_remove);
}

static	void	handle_edge_case(int *index, int *index_previous_slash,
	int *amount_to_remove, const char *curpath)
{
	bool	check_only_dot_dot_at_root;

	check_only_dot_dot_at_root
		= (*index_previous_slash == 0
			&& curpath[(*index_previous_slash) + 3] == '\0');
	if (check_only_dot_dot_at_root)
	{
		(*index_previous_slash)++;
		(*amount_to_remove)--;
		*index = 0;
	}
}

char	*while_dot_dot(char *curpath_canonical, int *index_previous_slash,
			int *amount_to_remove, int *i)
{
	char	*buffer;

	*index_previous_slash
		= find_index_previous_slash(*i, curpath_canonical);
	*amount_to_remove = calc_remove_dot_dot_component(i,
			*index_previous_slash);
	handle_edge_case(i, index_previous_slash, amount_to_remove,
		curpath_canonical);
	buffer = ft_remove_characters(curpath_canonical,
			*index_previous_slash, *amount_to_remove);
	free(curpath_canonical);
	if (buffer == NULL)
		return (NULL);
	return (buffer);
}
