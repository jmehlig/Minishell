/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 09:20:03 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 13:37:55 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_path_is_absolute(const char *path)
{
	if (path == NULL || ft_strlen(path) == 0)
		return (false);
	return (path[0] == '/');
}

char	*create_absolute_path_not_canonical(const char *path)
{
	char	*absolute_path_not_canonical;
	char	*current_working_directory;
	char	*current_working_directory_slash;

	current_working_directory = getcwd(NULL, 0);
	if (current_working_directory == NULL)
		return (NULL);
	current_working_directory_slash = ft_strjoin(current_working_directory,
			"/");
	free(current_working_directory);
	if (current_working_directory_slash == NULL)
		return (NULL);
	absolute_path_not_canonical = ft_strjoin(current_working_directory_slash,
			path);
	free(current_working_directory_slash);
	return (absolute_path_not_canonical);
}

char	*create_absolute_path(const char *path)
{
	char	*absolute_path;
	char	*absolute_path_not_canonical;

	if (path == NULL)
		return (NULL);
	if (check_path_is_absolute(path))
		absolute_path_not_canonical = ft_strdup(path);
	else
		absolute_path_not_canonical = create_absolute_path_not_canonical(path);
	if (absolute_path_not_canonical == NULL)
		return (NULL);
	absolute_path = convert_to_canonical_form(absolute_path_not_canonical);
	free(absolute_path_not_canonical);
	return (absolute_path);
}

bool	check_path_is_directory(const char *path)
{
	bool	check_found_directory;
	DIR		*directory;
	char	*absolute_path;

	if (path == NULL || ft_strrchr(path, '/') == NULL)
		return (false);
	if (check_path_is_absolute(path))
		absolute_path = ft_strdup(path);
	else
		absolute_path = create_absolute_path(path);
	if (absolute_path == NULL)
		return (false);
	directory = opendir(absolute_path);
	free(absolute_path);
	check_found_directory = directory != NULL;
	if (check_found_directory)
		closedir(directory);
	return (check_found_directory);
}

bool	check_path_is_executable(const char *path)
{
	bool	check_found_executable;
	char	*absolute_path;

	if (path == NULL || ft_strrchr(path, '/') == NULL)
	{
		errno = ENOENT;
		return (false);
	}
	if (check_path_is_directory(path) == true)
		return (false);
	if (errno == ENOMEM)
		return (false);
	absolute_path = create_absolute_path(path);
	if (absolute_path == NULL)
		return (false);
	check_found_executable = access(absolute_path, X_OK) == 0;
	free(absolute_path);
	return (check_found_executable);
}
