/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 09:10:49 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 15:03:25 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
complete input
*/

#include "minishell.h"

static	t_token	dup_token(t_token token)
{
	t_token	dup_token;

	dup_token.type = token.type;
	if (dup_token.type == TOKENTYPE_WORD)
	{
		dup_token.string_value = ft_strdup(token.string_value);
		if (dup_token.string_value == NULL)
			dup_token.type = TOKENTYPE_NOTOKEN;
	}
	return (dup_token);
}

static	bool	fill_combined_token(t_token *token, int number_of_token,
	t_token *combined_token, int index_start)
{
	int	index_combined_token;
	int	i;

	index_combined_token = index_start;
	i = 0;
	while (i < number_of_token)
	{
		combined_token[index_combined_token] = dup_token(token[i]);
		if (combined_token[index_combined_token].type == TOKENTYPE_NOTOKEN)
			return (false);
		index_combined_token++;
		i++;
	}
	return (true);
}

static	t_token	*combine_token(t_token *token, int number_of_token,
	t_token *additional_token, int number_additional_token)
{
	t_token	*combined_token;
	int		number_combined_token;

	number_combined_token = number_of_token + number_additional_token;
	combined_token = (t_token *) malloc(number_combined_token
			* sizeof(t_token));
	if (combined_token == NULL)
		return (NULL);
	if (fill_combined_token(token, number_of_token, combined_token, 0) == false)
	{
		free_token(combined_token);
		return (NULL);
	}
	if (fill_combined_token(additional_token, number_additional_token,
			combined_token, number_of_token) == false)
	{
		free_token(combined_token);
		return (NULL);
	}
	return (combined_token);
}

t_ast_node	*complete_input(t_token **token, int number_of_token)
{
	t_ast_node	*ast_tree;
	t_token		*combined_token;
	t_token		*additional_token;
	int			number_additional_token;
	char		*input;

	signal_handling(OPENPIPE);
	input = readline("> ");
	if (input == NULL)
		return (NULL);
	additional_token = build_tokens(input, &number_additional_token);
	free(input);
	if (additional_token == NULL)
		return (NULL);
	combined_token = combine_token(*token, number_of_token, additional_token,
			number_additional_token);
	free_token(additional_token);
	if (combined_token == NULL)
		return (NULL);
	free_token(*token);
	*token = combined_token;
	ast_tree = parse_command_line(token,
			number_of_token + number_additional_token);
	return (ast_tree);
}
