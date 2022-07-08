/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 09:59:45 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/16 08:33:42 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
parse job
*/

#include "minishell.h"

static	int	search_last_pipe_token(t_token *token, t_parse_range parse_range)
{
	int	number_token_in_range;
	int	i;

	number_token_in_range = calc_number_token_in_range(parse_range);
	i = parse_range.end - 1;
	while (i >= parse_range.begin)
	{
		if (token[i].type == TOKENTYPE_PIPE)
			return (i);
		i--;
	}
	return (PIPETOKEN_NOTFOUND);
}

static	bool	check_open_pipe(int index_pipe_token,
	t_parse_range parse_range)
{
	return (index_pipe_token == parse_range.end - 1);
}

static	bool	check_valid_pipe_token(t_token *token, int index_pipe_token,
	t_parser_error *parser_error)
{
	bool	check_valid_pipe_token_at_beginning_input;
	bool	check_word_token_before_pipe_token;

	check_valid_pipe_token_at_beginning_input = index_pipe_token == 0;
	if (check_valid_pipe_token_at_beginning_input)
	{
		*parser_error = PARSERERROR_PIPE;
		return (false);
	}
	check_word_token_before_pipe_token = token[index_pipe_token - 1].type
		== TOKENTYPE_WORD;
	if (check_word_token_before_pipe_token == false)
		*parser_error = PARSERERROR_PIPE;
	return (check_word_token_before_pipe_token);
}

static	t_ast_node	*create_job_node(t_token **token, t_parse_range parse_range,
	int index_pipe_token, t_parser_error *parser_error)
{
	t_ast_node		*job_node;
	t_parse_range	job_parse_range;
	t_parse_range	command_parse_range;

	job_node = create_ast_node(NODETYPE_JOB);
	if (job_node == NULL)
		return (NULL);
	job_parse_range = create_parse_range(parse_range.begin, index_pipe_token);
	command_parse_range = create_parse_range(index_pipe_token + 1,
			parse_range.end);
	job_node->value.node_pair.left = parse_job(token, job_parse_range,
			parser_error);
	job_node->value.node_pair.right = parse_command(*token, command_parse_range,
			parser_error);
	return (job_node);
}

t_ast_node	*parse_job(t_token **token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node		*ast_node;
	int				index_pipe_token;

	index_pipe_token = search_last_pipe_token(*token, parse_range);
	if (index_pipe_token == PIPETOKEN_NOTFOUND)
		ast_node = parse_command(*token, parse_range, parser_error);
	else
	{
		if (!check_valid_pipe_token(*token, index_pipe_token, parser_error))
			return (NULL);
		if (check_open_pipe(index_pipe_token, parse_range))
			ast_node = complete_input(token, parse_range.end);
		else
			ast_node = create_job_node(token, parse_range, index_pipe_token,
					parser_error);
		if (check_valid_ast_node(ast_node) == false)
		{
			if (*parser_error == PARSERERROR_NOERROR && errno != ENOMEM)
				*parser_error = PARSERERROR_OPENPIPE;
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}
