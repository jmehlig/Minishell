/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:27:13 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/15 13:39:13 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **arguments)
{
	int	i;
	int	option_n;

	i = 0;
	if (!arguments[i] || !arguments[i + 1])
		return (0);
	i++;
	option_n = 0;
	while (arguments[i] != NULL && (option_input(arguments[i]) == 0))
	{
		i++;
		option_n = 1;
	}
	while (arguments[i] != NULL)
	{
		if (arguments[i][0] == '#')
			break ;
		ft_putstr_fd(arguments[i], 1);
		if (arguments[i][0] != '\0' && arguments[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (option_n == 0)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	option_input(char *builtin)
{
	int	i;
	int	just_n;

	just_n = 0;
	if (ft_strncmp(builtin, "-n", 2) != 0)
		return (1);
	i = 2;
	while (builtin[i] != '\0')
	{
		if (builtin[i] != 'n')
			just_n = 1;
		i++;
	}
	return (just_n);
}
