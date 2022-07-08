/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:03:25 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/14 14:23:49 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	bool	check_token_character(char *input, char identifier_tokentype)
{
	return (ft_strlen(input) == 1 && input[0] == identifier_tokentype);
}

static	bool	check_token_string(char *input, char *identifier_tokentype)
{
	size_t	size_identifier;

	size_identifier = ft_strlen(identifier_tokentype);
	return (ft_strlen(input) == size_identifier
		&& ft_strncmp(input, identifier_tokentype, size_identifier) == 0);
}

t_token	fill_token(char *input)
{
	t_token	token;

	token.string_value = NULL;
	if (check_token_character(input, '\n'))
		token.type = TOKENTYPE_NEWLINE;
	else if (check_token_character(input, '<'))
		token.type = TOKENTYPE_INFILE;
	else if (check_token_character(input, '>'))
		token.type = TOKENTYPE_OUTFILE;
	else if (check_token_string(input, "<<"))
		token.type = TOKENTYPE_HEREDOC;
	else if (check_token_string(input, ">>"))
		token.type = TOKENTYPE_APPEND;
	else if (check_token_character(input, '|'))
		token.type = TOKENTYPE_PIPE;
	else
	{
		token.type = TOKENTYPE_WORD;
		token.string_value = ft_strdup(input);
		if (token.string_value == NULL)
			token.type = TOKENTYPE_NOTOKEN;
	}
	return (token);
}
