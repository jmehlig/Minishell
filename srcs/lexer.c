/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 13:44:16 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/05/26 09:32:46 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	count_tokens(char **input_splitted)
{
	int	i;

	i = 0;
	while (input_splitted[i])
		i++;
	return (i + 1);
}

static	bool	check_token_character(char *input, char identifier_tokentype)
{
	return (ft_strlen(input) == 1 && input[0] == identifier_tokentype);
}

static	bool	check_token_string(char *input, char *identifier_tokentype)
{
	size_t size_identifier;

	size_identifier = ft_strlen(identifier_tokentype);
	return (ft_strlen(input) == size_identifier
		&& ft_strncmp(input, identifier_tokentype, size_identifier) == 0);
}

static	t_token	fill_token(char *input)
{
	t_token token;

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

static	t_token	*send_input_through_lexer(char **input, int *number_of_tokens)
{
	t_token *token;
	int i;

	*number_of_tokens = count_tokens(input);
	token = (t_token *)malloc((*number_of_tokens) * sizeof(t_token));
	if (!token)
		return (NULL);
	i = 0;
	while (input[i])
	{
		token[i] = fill_token(input[i]);
		if (token[i].type == TOKENTYPE_NOTOKEN)
			free_token(token);
		i++;
	}
	token[i].type = TOKENTYPE_END;
	return (token);
}

t_token	*build_tokens(char *input, int *number_of_tokens)
{
	t_token *token;
	char **input_splitted;

	input_splitted = ft_split_quotes(input, ' ');
	if (!input_splitted)
		exit(EXIT_FAILURE);
	token = send_input_through_lexer(input_splitted, number_of_tokens);
	ft_free_2d_array_nullterminated((void **)input_splitted);
	return (token);
}

void	free_token(t_token *token)
{
	int i;

	i = 0;
	while (token[i].type != TOKENTYPE_NOTOKEN && token[i].type != TOKENTYPE_END)
	{
		if (token[i].type == TOKENTYPE_WORD)
			free(token[i].string_value);
		i++;
	}
	free(token);
}
