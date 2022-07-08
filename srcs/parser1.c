/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:50:58 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/23 09:15:27 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
parser grammar
<command_line>		::=	<job> <END>
<job>				::=	<command>
						<job> <PIPE> <command>
<command>			::=	<simple_command>
					|	<simple_command> <io_change>
<io_change>			:=	<redirection>
					|	<io_change> <redirection>
<redirection>		::=	<INFILE> <WORD>
					|	<OUTFILE> <WORD>
					|	<HEREDOC> <WORD>
					|	<APPEND> <WORD>
<simple_command>	::=	<pathname>
					|	<pathname> <arguments>
<pathname>			::= <WORD>
					|	EMPTY
<arguments>			::=	<arg>
					|	<arguments> <arg>
<arg>				::=	<WORD>
*/

#include "minishell.h"

static	bool	check_end_token(t_token *token, int number_of_tokens)
{
	return (token[number_of_tokens - 1].type == TOKENTYPE_END);
}

t_ast_node	*parse_command_line(t_token **token, int number_of_tokens)
{
	t_ast_node		*ast_tree;
	t_parse_range	job_parse_range;
	t_parser_error	parser_error;

	if (check_end_token(*token, number_of_tokens) == false)
	{
		errno = ERRNO_NOENDTOKEN;
		return (NULL);
	}
	parser_error = PARSERERROR_NOERROR;
	job_parse_range = create_parse_range(0, number_of_tokens - 1);
	ast_tree = parse_job(token, job_parse_range, &parser_error);
	if (check_valid_ast_node(ast_tree) == false)
	{
		free_ast_node(ast_tree);
		write_errormessage_parser(parser_error);
	}
	return (ast_tree);
}

t_command_table	*build_command_table(t_token **token, int number_of_tokens,
	t_list **my_envp, t_list **local)
{
	t_command_table		*command_table;
	t_ast_node			*ast_tree;
	t_collection_list	collect_list;

	ast_tree = parse_command_line(token, number_of_tokens);
	if (ast_tree == NULL)
	{
		write_errormessage(NULL);
		return (NULL);
	}
	collect_list.pointer_my_envp = my_envp;
	collect_list.pointer_local = local;
	command_table = create_command_table(ast_tree, collect_list);
	free_ast_node(ast_tree);
	return (command_table);
}
