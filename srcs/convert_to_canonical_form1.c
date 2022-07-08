/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_canonical_form1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:56:20 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/15 13:32:44 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_dot_dot_component(const char *path_cleaned)
{
	char	*curpath_canonical;
	int		index_previous_slash;
	int		amount_to_remove;
	int		i;

	curpath_canonical = ft_strdup(path_cleaned);
	if (curpath_canonical == NULL)
		return (NULL);
	i = 0;
	while (curpath_canonical[i])
	{
		if (check_dot_dot_component(i, curpath_canonical))
		{
			curpath_canonical = while_dot_dot(curpath_canonical,
					&index_previous_slash, &amount_to_remove, &i);
		}
		else
			i++;
	}
	return (curpath_canonical);
}

static	char	*remove_trailing_slash(const char *curpath)
{
	char	*curpath_without_trailing_slash;
	int		size_curpath;

	size_curpath = ft_strlen(curpath);
	if (size_curpath > 1 && curpath[size_curpath - 1] == '/')
	{
		curpath_without_trailing_slash = ft_substr(curpath, 0,
				size_curpath - 1);
	}
	else
		curpath_without_trailing_slash = ft_strdup(curpath);
	return (curpath_without_trailing_slash);
}

char	*convert_to_canonical_form(const char *path)
{
	char	*path_canonical;
	char	*path_cleaned;
	char	*path_without_dot_dot;

	if (path == NULL)
		return (NULL);
	path_cleaned = remove_dot_component_and_repeating_slashes(path);
	if (path_cleaned == NULL)
		return (NULL);
	path_without_dot_dot = remove_dot_dot_component(path_cleaned);
	free(path_cleaned);
	if (path_without_dot_dot == NULL)
		return (NULL);
	path_canonical = remove_trailing_slash(path_without_dot_dot);
	free(path_without_dot_dot);
	return (path_canonical);
}
