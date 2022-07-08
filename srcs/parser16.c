/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser16.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:22:18 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 13:43:20 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
utils fill command table
*/

#include "minishell.h"

void	free_command_table(t_command_table **command_table)
{
	int	i;

	if (*command_table == NULL)
		return ;
	if ((*command_table)->simple_command)
	{
		i = 0;
		while (i < (*command_table)->number)
		{
			ft_free_2d_array_nullterminated(
				(void **)(*command_table)->simple_command[i].arguments);
			i++;
		}
	}
	ft_destroy((void **) &((*command_table)->simple_command));
	ft_destroy((void **) command_table);
}

t_ast_node	*create_leaf_node(t_astnodetype node_type, char *string_value)
{
	t_ast_node	*leaf_node;

	if (string_value == NULL)
		return (NULL);
	leaf_node = create_ast_node(node_type);
	if (leaf_node == NULL)
		return (NULL);
	leaf_node->value.string = ft_strdup(string_value);
	if (leaf_node->value.string == NULL)
	{
		free(leaf_node);
		return (NULL);
	}
	return (leaf_node);
}

bool	check_leaf_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_PATHNAME
		|| ast_node->type == NODETYPE_INFILE
		|| ast_node->type == NODETYPE_OUTFILE
		|| ast_node->type == NODETYPE_HEREDOC
		|| ast_node->type == NODETYPE_APPEND
		|| ast_node->type == NODETYPE_ARG
		|| ast_node->type == NODETYPE_EMPTY);
}
