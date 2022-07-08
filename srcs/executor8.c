/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:55:45 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/14 13:17:55 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils create programpath
*/

#include "minishell.h"

bool	check_found_programpath(const char *program_path)
{
	if (errno == ENOMEM)
	{
		write_errormessage(NULL);
		return (true);
	}
	return (program_path != NULL);
}

char	*create_executable_path(const char *program_name)
{
	char	*program_path;

	program_path = ft_strdup(program_name);
	if (program_name == NULL)
		write_errormessage(NULL);
	return (program_path);
}
