/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 15:42:34 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 14:43:34 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	errno_and_destroy_path(char **current_path, char *where)
{
	if (ft_strncmp(where, "HOME", 5) == 0)
	{
		if (errno == 0)
			errno = ERRNO_HOMENOTFOUND;
	}
	else if (ft_strncmp(where, "OLDPWD", 7) == 0)
	{
		if (errno == 0)
			errno = ERRNO_OLDPWDNOTFOUND;
	}
	ft_destroy((void **) current_path);
}

static	char	*init_current_path(char *directory, t_list *my_envp,
	bool *print_path)
{
	char	*current_path;

	if (directory == NULL)
	{
		current_path = get_value_variable(my_envp, "HOME");
		if (check_variable_has_no_value(current_path))
			errno_and_destroy_path(&current_path, "HOME");
	}
	else if (ft_strncmp(directory, "-", 2) == 0)
	{
		current_path = get_value_variable(my_envp, "OLDPWD");
		if (check_variable_has_no_value(current_path))
			errno_and_destroy_path(&current_path, "OLDPWD");
		*print_path = true;
	}
	else
		current_path = ft_strdup(directory);
	return (current_path);
}

static	bool	update_pwd_and_oldpwd(const char *absolute_path,
	t_list **my_envp)
{
	char	*current_pwd;
	bool	success_add_pwd;
	bool	success_add_oldpwd;

	current_pwd = get_value_variable(*my_envp, "PWD");
	free(current_pwd);
	current_pwd = getcwd(NULL, 0);
	if (current_pwd == NULL)
		return (false);
	success_add_pwd = add_entry("PWD", absolute_path, my_envp);
	if (success_add_pwd == false)
	{
		free(current_pwd);
		return (false);
	}
	success_add_oldpwd = add_entry("OLDPWD", current_pwd, my_envp);
	free(current_pwd);
	return (success_add_oldpwd);
}

static int	error_message_my_cd(char *absolute_path, char *argument)
{
	write_errormessage_builtin("cd", argument);
	free(absolute_path);
	return (1);
}

int	my_cd(char **arguments, t_list **my_envp)
{
	int		return_chdir;
	char	*current_path;
	char	*absolute_path;
	bool	print_path;

	print_path = false;
	current_path = init_current_path(arguments[1], *my_envp, &print_path);
	absolute_path = create_absolute_path(current_path);
	free(current_path);
	if (absolute_path != NULL && !check_path_is_directory(absolute_path))
		return (error_message_my_cd(absolute_path, arguments[1]));
	if (update_pwd_and_oldpwd(absolute_path, my_envp) == false)
		return (error_message_my_cd(absolute_path, NULL));
	return_chdir = chdir(absolute_path);
	if (return_chdir == -1)
		return (error_message_my_cd(absolute_path, absolute_path));
	if (print_path)
		ft_putendl_fd(absolute_path, STDOUT_FILENO);
	free(absolute_path);
	return (0);
}
