/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser12.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:07:14 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:25:46 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
fill command table
*/

#include "minishell.h"

static	bool	check_connection_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_JOB
		|| ast_node->type == NODETYPE_COMMAND
		|| ast_node->type == NODETYPE_IOCHANGE
		|| ast_node->type == NODETYPE_ARGUMENTS);
}

static	bool	check_redirection_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_INFILE
		|| ast_node->type == NODETYPE_OUTFILE
		|| ast_node->type == NODETYPE_HEREDOC
		|| ast_node->type == NODETYPE_APPEND);
}

static	bool	fill_command_table_empty(t_simple_command *simple_command)
{
	simple_command->arguments = (char **) malloc(sizeof(char *));
	if (simple_command->arguments == NULL)
		return (false);
	simple_command->arguments[0] = NULL;
	simple_command->number = 0;
	return (true);
}

static	void	fill_command_table_type(t_command_table **command_table,
	t_ast_node *ast_node, t_create_ct_utils *create_ct_utils,
	t_collection_list collect_list)
{
	t_simple_command	*command;
	bool				function_call;

	command = &((*command_table)->simple_command[create_ct_utils->index_sc]);
	function_call = true;
	if (ast_node->type == NODETYPE_PATHNAME)
	{
		function_call = fill_command_table_pathname(command,
				create_ct_utils->index_arg, ast_node->value.string);
		(create_ct_utils->index_arg)++;
	}
	else if (ast_node->type == NODETYPE_ARG)
	{
		function_call = fill_command_table_arg(command,
				create_ct_utils->index_arg, ast_node->value.string);
		(create_ct_utils->index_arg)++;
	}
	else if (check_redirection_node(ast_node))
		fill_command_table_redirection(command, ast_node, collect_list);
	else
		function_call = fill_command_table_empty(command);
	if (function_call == false)
		free_command_table(command_table);
}

void	fill_command_table(t_command_table **command_table,
	t_ast_node *ast_node, t_create_ct_utils *create_ct_utils,
	t_collection_list collect_list)
{
	if (*command_table == NULL)
		return ;
	if (check_connection_node(ast_node))
	{
		fill_command_table(command_table, ast_node->value.node_pair.left,
			create_ct_utils, collect_list);
		if (ast_node->type == NODETYPE_JOB)
			(create_ct_utils->index_sc)++;
		fill_command_table(command_table, ast_node->value.node_pair.right,
			create_ct_utils, collect_list);
	}
	else if (ast_node->type == NODETYPE_SIMPLECOMMAND)
	{
		fill_command_table_simple_command(command_table, ast_node,
			create_ct_utils, collect_list);
	}
	else
	{
		fill_command_table_type(command_table, ast_node, create_ct_utils,
			collect_list);
	}
}
