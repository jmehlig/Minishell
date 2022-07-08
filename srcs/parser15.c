/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:19:17 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 12:07:04 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
remainder of fill command table
*/

#include "minishell.h"

static	int	count_number_of_leafs(t_ast_node *ast_node)
{
	if (ast_node->type == NODETYPE_EMPTY)
		return (0);
	if (check_leaf_node(ast_node))
		return (1);
	return (count_number_of_leafs(ast_node->value.node_pair.left)
		+ count_number_of_leafs(ast_node->value.node_pair.right));
}

void	fill_command_table_simple_command(t_command_table **command_table,
	t_ast_node *ast_node, t_create_ct_utils *create_ct_utils,
	t_collection_list collect_list)
{
	int	number_of_arguments;
	int	index_simplecommand;

	index_simplecommand = create_ct_utils->index_sc;
	number_of_arguments = count_number_of_leafs(ast_node);
	(*command_table)->simple_command[index_simplecommand].arguments
		= malloc((number_of_arguments + 1) * sizeof(char *));
	if ((*command_table)->simple_command[index_simplecommand].arguments == NULL)
	{
		free_command_table(command_table);
		return ;
	}
	(*command_table)->simple_command[index_simplecommand].number
		= number_of_arguments;
	create_ct_utils->index_arg = 0;
	fill_command_table(command_table, ast_node->value.node_pair.left,
		create_ct_utils, collect_list);
	fill_command_table(command_table, ast_node->value.node_pair.right,
		create_ct_utils, collect_list);
	(*command_table)->simple_command[index_simplecommand].arguments[
		create_ct_utils->index_arg] = NULL;
}

static	bool	fill_command_table_only_pathname(
	t_simple_command *simple_command, const char *path_name)
{
	simple_command->arguments = (char **) malloc (2 * sizeof(char *));
	if (simple_command->arguments == NULL)
		return (false);
	simple_command->arguments[0] = ft_strdup(path_name);
	if (simple_command->arguments[0] == NULL)
		return (false);
	simple_command->arguments[1] = NULL;
	simple_command->number = 1;
	return (true);
}

bool	fill_command_table_pathname(t_simple_command *simple_command,
	int index_argument, const char *path_name)
{
	bool	check_command_no_arguments;

	check_command_no_arguments = simple_command->arguments == NULL;
	if (check_command_no_arguments)
		return (fill_command_table_only_pathname(simple_command, path_name));
	simple_command->arguments[index_argument] = ft_strdup(path_name);
	if (simple_command->arguments[index_argument] == NULL)
		return (false);
	return (true);
}

bool	fill_command_table_arg(t_simple_command *simple_command,
	int index_argument, const char *arg)
{
	simple_command->arguments[index_argument] = ft_strdup(arg);
	if (simple_command->arguments[index_argument] == NULL)
		return (false);
	return (true);
}
