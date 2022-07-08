/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:54:45 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/23 09:15:21 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
create command table
*/

#include "minishell.h"

static	int	count_root_nodes(t_ast_node *ast_node, t_astnodetype root_node_type)
{
	int	found_node;

	found_node = 0;
	if (ast_node->type == root_node_type)
		found_node = 1;
	if (check_leaf_node(ast_node))
		return (found_node);
	return (count_root_nodes(ast_node->value.node_pair.left, root_node_type)
		+ count_root_nodes(ast_node->value.node_pair.right, root_node_type)
		+ found_node);
}

static	int	count_number_simple_commands(t_ast_node *ast_tree)
{
	t_astnodetype	root_node_type;
	int				number_root_nodes;

	root_node_type = ast_tree->type;
	number_root_nodes = count_root_nodes(ast_tree, root_node_type);
	if (root_node_type == NODETYPE_JOB)
		return (number_root_nodes + 1);
	return (number_root_nodes);
}

static	t_command_table	*init_command_table(int number_simple_commands)
{
	t_command_table	*command_table;
	int				i;

	command_table = (t_command_table *) malloc(sizeof(t_command_table));
	if (command_table == NULL)
		return (NULL);
	command_table->number = number_simple_commands;
	command_table->simple_command = (t_simple_command *) malloc((
				command_table->number) * sizeof(t_simple_command));
	if (command_table->simple_command == NULL)
	{
		free(command_table);
		return (NULL);
	}
	i = 0;
	while (i < command_table->number)
	{
		command_table->simple_command[i].arguments = NULL;
		command_table->simple_command[i].fd_in = -1;
		command_table->simple_command[i].fd_out = -1;
		command_table->simple_command[i].fd_failed = false;
		i++;
	}
	return (command_table);
}

t_command_table	*create_command_table(t_ast_node *ast_tree,
	t_collection_list collect_list)
{
	t_command_table		*command_table;
	int					number_simple_commands;
	t_create_ct_utils	create_ct_utils;

	number_simple_commands = count_number_simple_commands(ast_tree);
	command_table = init_command_table(number_simple_commands);
	if (!command_table)
		return (NULL);
	create_ct_utils.index_sc = 0;
	create_ct_utils.index_arg = 0;
	fill_command_table(&command_table, ast_tree, &create_ct_utils,
		collect_list);
	return (command_table);
}
