/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 13:44:16 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 10:48:51 by hkalyonc         ###   ########.fr       */
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

static	t_token	*send_input_through_lexer(char **input, int *number_of_tokens)
{
	t_token	*token;
	int		i;

	*number_of_tokens = count_tokens(input);
	token = (t_token *) malloc((*number_of_tokens) * sizeof(t_token));
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
	t_token	*token;
	char	**input_splitted;

	input_splitted = ft_split_quotes(input);
	if (!input_splitted)
		return (NULL);
	token = send_input_through_lexer(input_splitted, number_of_tokens);
	ft_free_2d_array_nullterminated((void **)input_splitted);
	return (token);
}

void	free_token(t_token *token)
{
	int	i;

	i = 0;
	while (token[i].type != TOKENTYPE_NOTOKEN && token[i].type != TOKENTYPE_END)
	{
		if (token[i].type == TOKENTYPE_WORD)
			free(token[i].string_value);
		i++;
	}
	free(token);
}
