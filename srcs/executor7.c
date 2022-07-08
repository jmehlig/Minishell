/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:50:30 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 15:37:29 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
create_programpath
*/

#include "minishell.h"

static	char	**split_path_value(t_list *my_envp)
{
	char	*path_value;
	char	**env_path_splitted;

	path_value = get_value_variable(my_envp, "PATH");
	if (path_value == NULL)
		return (NULL);
	env_path_splitted = ft_split_modified(path_value, PATH_DELIMITER);
	free(path_value);
	return (env_path_splitted);
}

static	char	*put_together_binary(const char *path_entry,
	const char *command_name)
{
	char	*directory;
	char	*binary_path;

	if (ft_strlen(path_entry) == 0)
		directory = ft_strdup("");
	else
		directory = ft_strjoin(path_entry, "/");
	if (directory == NULL)
		return (NULL);
	binary_path = ft_strjoin(directory, command_name);
	free(directory);
	return (binary_path);
}

static	char	*try_out_path_entry(const char *path_entry,
	const char *command_name)
{
	char	*binary_path;
	bool	check_binary_exists;

	binary_path = put_together_binary(path_entry, command_name);
	if (binary_path == NULL)
		return (NULL);
	check_binary_exists = access(binary_path, X_OK) == 0;
	if (check_binary_exists)
		return (binary_path);
	free(binary_path);
	return (NULL);
}

static	char	*search_command_binary(const char *command_name,
	t_list *my_envp)
{
	char	**env_path_splitted;
	char	*binary_path;
	int		i;

	if (ft_strrchr(command_name, '/') != NULL)
		return (NULL);
	env_path_splitted = split_path_value(my_envp);
	if (env_path_splitted == NULL)
		return (NULL);
	i = 0;
	while (env_path_splitted[i] != NULL)
	{
		binary_path = try_out_path_entry(env_path_splitted[i], command_name);
		if (errno == ENOMEM || binary_path != NULL)
		{
			if (binary_path != NULL)
				errno = 0;
			ft_free_2d_array_nullterminated((void **) env_path_splitted);
			return (binary_path);
		}
		free(binary_path);
		i++;
	}
	ft_free_2d_array_nullterminated((void **) env_path_splitted);
	return (NULL);
}

char	*create_programpath(const char *program_name, t_list *my_envp)
{
	char	*program_path;

	if (check_path_is_directory(program_name))
	{
		errno = ERRNO_COMMANDNOACCESS;
		write_errormessage(program_name);
		return (NULL);
	}
	program_path = search_command_binary(program_name, my_envp);
	if (check_found_programpath(program_path))
		return (program_path);
	if (check_path_is_executable(program_name))
	{
		program_path = create_executable_path(program_name);
		return (program_path);
	}
	write_errormessage_createprogrampath(program_name, my_envp);
	return (NULL);
}
