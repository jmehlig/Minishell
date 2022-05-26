/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:11:58 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/05/20 10:17:29 by hkalyonc         ###   ########.fr       */
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

void	free_ast_node(t_ast_node *ast_node)
{
	bool	check_nodetype_with_nodepair;
	bool	check_nodetype_with_string;

	if (ast_node == NULL)
		return ;
	check_nodetype_with_nodepair = ast_node->type == NODETYPE_JOB
			|| ast_node->type == NODETYPE_COMMAND
			|| ast_node->type == NODETYPE_SIMPLECOMMAND
			|| ast_node->type == NODETYPE_ARGUMENTS;
	check_nodetype_with_string = check_nodetype_with_nodepair == false
			&& ast_node->type != NODETYPE_EMPTY;
	if (check_nodetype_with_nodepair)
	{
		free_ast_node(ast_node->value.node_pair.left);
		free_ast_node(ast_node->value.node_pair.right);
	}
	if (check_nodetype_with_string)
		free(ast_node->value.string);
	free(ast_node);
	ast_node = NULL;
}
