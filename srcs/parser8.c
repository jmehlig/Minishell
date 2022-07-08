/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:43:17 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 15:43:42 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
parse simple command + parse arguments
*/

#include "minishell.h"

static	bool	check_valid_token(t_token token, t_parser_error *parser_error)
{
	bool	valid_token;

	valid_token = token.type == TOKENTYPE_WORD;
	if (valid_token == false)
		*parser_error = PARSERERROR_GENERAL;
	return (valid_token);
}

static	t_ast_node	*create_arguments_node(t_token *token,
	t_parse_range parse_range, t_parser_error *parser_error)
{
	t_ast_node		*arguments_node;
	t_parse_range	arguments_parse_range;

	arguments_node = create_ast_node(NODETYPE_ARGUMENTS);
	if (arguments_node == NULL)
		return (NULL);
	arguments_parse_range = create_parse_range(parse_range.begin + 1,
			parse_range.end);
	arguments_node->value.node_pair.left = create_leaf_node(NODETYPE_ARG,
			token[parse_range.begin].string_value);
	arguments_node->value.node_pair.right = parse_arguments(token,
			arguments_parse_range, parser_error);
	return (arguments_node);
}

t_ast_node	*parse_arguments(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;
	int			number_tokens_in_range;

	if (check_valid_token(token[parse_range.begin], parser_error) == false)
		return (NULL);
	number_tokens_in_range = calc_number_token_in_range(parse_range);
	if (number_tokens_in_range == 1)
	{
		ast_node = create_leaf_node(NODETYPE_ARG,
				token[parse_range.begin].string_value);
	}
	else
	{
		ast_node = create_arguments_node(token, parse_range, parser_error);
		if (check_valid_ast_node(ast_node) == false)
		{
			free(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

static	t_ast_node	*create_simple_command_node(t_token *token,
	t_parse_range parse_range, t_parser_error *parser_error)
{
	t_ast_node		*simple_command_node;
	t_parse_range	arguments_parse_range;

	simple_command_node = create_ast_node(NODETYPE_SIMPLECOMMAND);
	if (simple_command_node == NULL)
		return (NULL);
	arguments_parse_range = create_parse_range(parse_range.begin + 1,
			parse_range.end);
	simple_command_node->value.node_pair.left = create_leaf_node(
			NODETYPE_PATHNAME, token[parse_range.begin].string_value);
	simple_command_node->value.node_pair.right = parse_arguments(token,
			arguments_parse_range, parser_error);
	return (simple_command_node);
}

t_ast_node	*parse_simple_command(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;
	int			number_token_in_range;

	number_token_in_range = calc_number_token_in_range(parse_range);
	if (number_token_in_range == 0)
	{
		ast_node = create_ast_node(NODETYPE_EMPTY);
		return (ast_node);
	}
	if (check_valid_token(token[parse_range.begin], parser_error) == false)
		return (NULL);
	if (number_token_in_range == 1)
		ast_node = create_leaf_node(NODETYPE_PATHNAME,
				token[parse_range.begin].string_value);
	else
	{
		ast_node = create_simple_command_node(token, parse_range, parser_error);
		if (check_valid_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}
