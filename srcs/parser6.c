/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 09:12:38 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 15:43:42 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
parse io change
*/

#include "minishell.h"

t_astnodetype	look_up_redirection_type(t_token token)
{
	if (token.type == TOKENTYPE_INFILE)
		return (NODETYPE_INFILE);
	else if (token.type == TOKENTYPE_OUTFILE)
		return (NODETYPE_OUTFILE);
	else if (token.type == TOKENTYPE_HEREDOC)
		return (NODETYPE_HEREDOC);
	return (NODETYPE_APPEND);
}

t_ast_node	*create_redirection_node(t_token *token, t_parse_range parse_range)
{
	t_ast_node		*redirection_node;
	t_astnodetype	redirection_type;

	redirection_type = look_up_redirection_type(token[parse_range.begin]);
	redirection_node = create_ast_node(redirection_type);
	if (redirection_node == NULL)
		return (NULL);
	redirection_node->value.string
		= ft_strdup(token[parse_range.end - 1].string_value);
	if (redirection_node->value.string == NULL)
	{
		free(redirection_node);
		redirection_node = NULL;
	}
	return (redirection_node);
}

t_parse_range	create_redirection_parse_range(t_parse_range *parse_range)
{
	t_parse_range	redirection_parse_range;

	redirection_parse_range.begin = parse_range->begin;
	redirection_parse_range.end = redirection_parse_range.begin
		+ MINTOKEN_REDIRECTION;
	parse_range->begin += MINTOKEN_REDIRECTION;
	if (parse_range->begin >= parse_range->end)
	{
		parse_range->begin = PARSERANGE_NOREDIRECTION;
		parse_range->end = PARSERANGE_NOREDIRECTION;
	}
	return (redirection_parse_range);
}

t_ast_node	*create_io_change_node(t_token *token,
	t_parse_range parse_range_front, t_parse_range parse_range_back)
{
	t_ast_node		*io_change_node;
	bool			check_front_io_changes;
	t_parse_range	redirection_parse_range;

	io_change_node = create_ast_node(NODETYPE_IOCHANGE);
	if (io_change_node == NULL)
		return (NULL);
	check_front_io_changes = check_no_redirection_parse_range(
			parse_range_front) == false;
	if (check_front_io_changes)
	{
		redirection_parse_range = create_redirection_parse_range(
				&parse_range_front);
	}
	else
	{
		redirection_parse_range = create_redirection_parse_range(
				&parse_range_back);
	}
	io_change_node->value.node_pair.left = create_redirection_node(token,
			redirection_parse_range);
	io_change_node->value.node_pair.right = parse_io_change(token,
			parse_range_front, parse_range_back);
	return (io_change_node);
}

t_ast_node	*parse_io_change(t_token *token, t_parse_range parse_range_front,
	t_parse_range parse_range_back)
{
	t_ast_node	*ast_node;
	bool		check_front_io_changes;

	check_front_io_changes = check_no_redirection_parse_range(
			parse_range_front) == false;
	if (check_only_one_redirection(parse_range_front, parse_range_back))
	{
		if (check_front_io_changes)
			ast_node = create_redirection_node(token, parse_range_front);
		else
			ast_node = create_redirection_node(token, parse_range_back);
	}
	else
	{
		ast_node = create_io_change_node(token, parse_range_front,
				parse_range_back);
		if (check_valid_ast_node(ast_node) == false)
		{
			free(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}
