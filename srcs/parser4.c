/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:15:33 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 15:43:42 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
parse command
*/

#include "minishell.h"

static	t_parse_range	create_io_change_front_parse_range(t_token *token,
	t_parse_range parse_range)
{
	t_parse_range	io_change_front_parse_range;
	int				index_last_redirect;

	index_last_redirect = parse_range.begin;
	while (check_redirection_token(token[index_last_redirect]))
	{
		index_last_redirect += MINTOKEN_REDIRECTION;
		if (index_last_redirect >= parse_range.end - 1)
			break ;
	}
	if (check_found_redirection(index_last_redirect, parse_range.begin))
	{
		index_last_redirect -= MINTOKEN_REDIRECTION;
		if (token[index_last_redirect + 1].type == TOKENTYPE_WORD)
			io_change_front_parse_range = create_parse_range(parse_range.begin,
					index_last_redirect + MINTOKEN_REDIRECTION);
		else
			io_change_front_parse_range = create_error_redirection_parse_range(
					index_last_redirect);
	}
	else
		io_change_front_parse_range = create_no_redirection_parse_range();
	return (io_change_front_parse_range);
}

static	t_parse_range	create_io_change_back_parse_range(t_token *token,
	t_parse_range parse_range)
{
	t_parse_range	io_change_back_parse_range;
	int				index_last_redirect;
	int				index_first_redirect;

	index_first_redirect = parse_range.end - MINTOKEN_REDIRECTION;
	index_last_redirect = index_first_redirect;
	io_change_back_parse_range = create_no_redirection_parse_range();
	while (check_redirection_token(token[index_last_redirect]))
	{
		index_last_redirect -= MINTOKEN_REDIRECTION;
		if (index_last_redirect < parse_range.begin)
			break ;
	}
	if (check_found_redirection(index_last_redirect, index_first_redirect))
	{
		index_last_redirect += MINTOKEN_REDIRECTION;
		if (token[index_last_redirect + 1].type == TOKENTYPE_WORD
			&& token[index_last_redirect - 1].type == TOKENTYPE_WORD)
			io_change_back_parse_range = create_parse_range(index_last_redirect,
					parse_range.end);
		else
			io_change_back_parse_range = create_error_redirection_parse_range(
					index_last_redirect);
	}
	return (io_change_back_parse_range);
}

static	t_parse_range	create_simple_command_parse_range(
	t_parse_range parse_range, t_parse_range io_change_front_parse_range,
	t_parse_range io_change_back_parse_range)
{
	t_parse_range	simple_command_parse_range;

	if (check_no_redirection_parse_range(io_change_front_parse_range))
		simple_command_parse_range.begin = parse_range.begin;
	else
		simple_command_parse_range.begin = io_change_front_parse_range.end;
	if (check_no_redirection_parse_range(io_change_back_parse_range))
		simple_command_parse_range.end = parse_range.end;
	else
		simple_command_parse_range.end = io_change_back_parse_range.begin;
	return (simple_command_parse_range);
}

static	t_ast_node	*create_command_node(t_token *token,
	t_parse_range parse_range, t_parser_error *parser_error)
{
	t_ast_node		*command_node;
	t_parse_range	simple_command_parse_range;
	t_parse_range	io_change_front_parse_range;
	t_parse_range	io_change_back_parse_range;

	command_node = create_ast_node(NODETYPE_COMMAND);
	if (command_node == false)
		return (NULL);
	io_change_front_parse_range = create_io_change_front_parse_range(token,
			parse_range);
	if (calc_number_token_in_range(parse_range) == MINTOKEN_REDIRECTION)
		io_change_back_parse_range = create_no_redirection_parse_range();
	else
		io_change_back_parse_range = create_io_change_back_parse_range(token,
				parse_range);
	if (check_error_redirection(io_change_front_parse_range,
			io_change_back_parse_range, token, parser_error))
		return (NULL);
	simple_command_parse_range = create_simple_command_parse_range(parse_range,
			io_change_front_parse_range, io_change_back_parse_range);
	command_node->value.node_pair.left = parse_simple_command(token,
			simple_command_parse_range, parser_error);
	command_node->value.node_pair.right = parse_io_change(token,
			io_change_front_parse_range, io_change_back_parse_range);
	return (command_node);
}

t_ast_node	*parse_command(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;

	if (check_io_change(token, parse_range))
	{
		ast_node = create_command_node(token, parse_range, parser_error);
		if (check_valid_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	else
		ast_node = parse_simple_command(token, parse_range, parser_error);
	return (ast_node);
}
