/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:02:27 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 14:55:35 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_errormessage_builtin(const char *builtin_name,
	const char *additional_message)
{
	write(STDERR_FILENO, builtin_name, ft_strlen(builtin_name));
	write(STDERR_FILENO, ": ", 2);
	write_errormessage(additional_message);
}

void	write_errormessage_createprogrampath(const char *program_name,
	t_list *envp)
{
	char	*path_value;
	bool	check_program_is_command;

	path_value = get_value_variable(envp, "PATH");
	if (path_value == NULL)
	{
		write_errormessage(NULL);
		return ;
	}
	check_program_is_command = (ft_strlen(path_value) != 0 && errno == ENOENT
			&& ft_strrchr(program_name, '/') == NULL);
	free(path_value);
	if (check_program_is_command)
		errno = ERRNO_COMMANDNOTFOUND;
	if (errno == ENOMEM)
		write_errormessage(NULL);
	else
		write_errormessage(program_name);
	if (errno == EACCES)
		errno = ERRNO_COMMANDNOACCESS;
	else
		errno = ERRNO_COMMANDNOTFOUND;
}
