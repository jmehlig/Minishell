/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_canonical_form2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 12:00:05 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 18:40:32 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	bool	check_dot_component(int index, const char *path)
{
	bool	previous_slash;
	bool	current_dot;
	bool	following_slash;

	if (index == 0)
		previous_slash = true;
	else
		previous_slash = path[index - 1] == '/';
	current_dot = path[index] == '.';
	if (path[index + 1] == '\0')
		following_slash = true;
	else
		following_slash = path[index + 1] == '/';
	return (previous_slash && current_dot && following_slash);
}

static	int	size_dot_component(int index, char following_character)
{
	if (index == 0 || following_character == '\0')
		return (1);
	return (2);
}

static	bool	check_repeating_slash(int index, const char *path)
{
	if (index == 0)
		return (false);
	return (path[index - 1] == '/' && path[index] == '/');
}

static	int	count_slashes(int index, const char *path)
{
	int	amount_slashes;

	amount_slashes = 0;
	while (path[index + amount_slashes] == '/')
		amount_slashes++;
	return (amount_slashes);
}

char	*remove_dot_component_and_repeating_slashes(const char *path)
{
	char	*path_cleaned;
	char	*removed_chars;
	int		amount_to_remove;
	int		i;

	path_cleaned = ft_strdup(path);
	if (path_cleaned == NULL)
		return (NULL);
	i = 0;
	while (path_cleaned[i])
	{
		amount_to_remove = 0;
		if (check_dot_component(i, path_cleaned))
			amount_to_remove = size_dot_component(i, path_cleaned[i + 1]);
		else if (check_repeating_slash(i, path_cleaned))
			amount_to_remove = count_slashes(i, path_cleaned);
		removed_chars = ft_remove_characters(path_cleaned, i, amount_to_remove);
		free(path_cleaned);
		if (removed_chars == NULL)
			return (NULL);
		path_cleaned = removed_chars;
		if (amount_to_remove == 0)
			i++;
	}
	return (path_cleaned);
}
