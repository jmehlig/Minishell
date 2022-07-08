/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:11:58 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/15 17:04:09 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_ast_node(t_astnodetype node_type)
{
	t_ast_node	*ast_node;

	ast_node = (t_ast_node *) malloc(sizeof(t_ast_node));
	if (ast_node == NULL)
		return (NULL);
	ast_node->type = node_type;
	return (ast_node);
}

bool	check_node_with_nodepair(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_JOB
		|| ast_node->type == NODETYPE_COMMAND
		|| ast_node->type == NODETYPE_SIMPLECOMMAND
		|| ast_node->type == NODETYPE_ARGUMENTS
		|| ast_node->type == NODETYPE_IOCHANGE);
}

bool	check_node_with_string(t_ast_node *ast_node)
{
	return (check_node_with_nodepair(ast_node) == false
		&& ast_node->type != NODETYPE_EMPTY);
}

bool	check_valid_ast_node(t_ast_node *node)
{
	if (node == NULL)
		return (false);
	if (node->type == NODETYPE_EMPTY)
		return (true);
	if (check_node_with_nodepair(node))
	{
		return (check_valid_ast_node(node->value.node_pair.right)
			&& check_valid_ast_node(node->value.node_pair.left));
	}
	return (node->value.string);
}

void	free_ast_node(t_ast_node *ast_node)
{
	if (ast_node == NULL)
		return ;
	if (check_node_with_nodepair(ast_node))
	{
		free_ast_node(ast_node->value.node_pair.left);
		free_ast_node(ast_node->value.node_pair.right);
	}
	if (check_node_with_string(ast_node))
		free(ast_node->value.string);
	free(ast_node);
	ast_node = NULL;
}
