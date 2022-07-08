/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:53:33 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 08:25:48 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	write_errormessage_exit(void)
{
	if (errno == ERRNO_EXIT)
		write(2, ERRMSG_EXIT, ft_strlen(ERRMSG_EXIT));
	else
		write(2, ERRMSG_EXITNUMERIC, ft_strlen(ERRMSG_EXITNUMERIC));
}

static	void	write_errormessage_buildtoken(void)
{
	if (errno == ERRNO_NOENDTOKEN)
		write(2, ERRMSG_NOENDTOKEN, ft_strlen(ERRMSG_NOENDTOKEN));
	else
		write(2, ERRMSG_QUOTE, ft_strlen(ERRMSG_QUOTE));
	errno = ERRNO_PARSER;
}

static	void	write_custom_errormessage(const char *additional_message)
{
	if (additional_message != NULL)
	{
		write(2, additional_message, ft_strlen(additional_message));
		write(2, ": ", 2);
	}
	if (errno == ERRNO_COMMANDNOACCESS)
		write(2, ERRMSG_COMMANDNOACCESS, ft_strlen(ERRMSG_COMMANDNOACCESS));
	else if (errno == ERRNO_COMMANDNOTFOUND)
		write(2, ERRMSG_COMMANDNOTFOUND, ft_strlen(ERRMSG_COMMANDNOTFOUND));
	else if (errno == ERRNO_EXIT || errno == ERRNO_EXITNUMERIC)
		write_errormessage_exit();
	else if (errno == ERRNO_HOMENOTFOUND)
		write(2, ERRMSG_HOMENOTFOUND, ft_strlen(ERRMSG_HOMENOTFOUND));
	else if (errno == ERRNO_OLDPWDNOTFOUND)
		write(2, ERRMSG_OLDPWDNOTFOUND, ft_strlen(ERRMSG_OLDPWDNOTFOUND));
	else if (errno == ERRNO_IDENTIFIER)
		write(2, ERRMSG_IDENTIFIER, ft_strlen(ERRMSG_IDENTIFIER));
	else if (errno >= ERRNO_NOENDTOKEN)
		write_errormessage_buildtoken();
	else
	{
		write(2, "Unknown error: ", 15);
		ft_putnbr_fd(errno, 2);
	}
	write(2, "\n", 1);
}

void	write_errormessage(const char *additional_message)
{
	bool	check_already_wrote_errormessage;

	check_already_wrote_errormessage = errno == ERRNO_PARSER;
	if (check_already_wrote_errormessage)
		return ;
	if (errno > LASTSYSTEMERRNO)
		write_custom_errormessage(additional_message);
	else
		perror(additional_message);
}

void	write_errormessage_parser(t_parser_error parser_error)
{
	errno = ERRNO_PARSER;
	if (parser_error == PARSERERROR_OPENPIPE)
	{
		ft_putendl_fd(PARSERERRMSG_OPENPIPE, STDERR_FILENO);
		return ;
	}
	ft_putstr_fd(PARSERERRMSG_START, STDERR_FILENO);
	if (parser_error == PARSERERROR_GENERAL)
		ft_putendl_fd(PARSERERRMSG_GENERAL, STDERR_FILENO);
	else if (parser_error == PARSERERROR_PIPE)
		ft_putendl_fd(PARSERERRMSG_PIPE, STDERR_FILENO);
	else if (parser_error == PARSERERROR_INFILE)
		ft_putendl_fd(PARSERERRMSG_INFILE, STDERR_FILENO);
	else if (parser_error == PARSERERROR_OUTFILE)
		ft_putendl_fd(PARSERERRMSG_OUTFILE, STDERR_FILENO);
	else if (parser_error == PARSERERROR_HEREDOC)
		ft_putendl_fd(PARSERERRMSG_HEREDOC, STDERR_FILENO);
	else
		ft_putendl_fd(PARSERERRMSG_APPEND, STDERR_FILENO);
}
