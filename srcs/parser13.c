/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser13.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:12:34 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/06/17 14:18:34 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
fill command table redirection
*/

#include "minishell.h"

static	void	handle_error_redirection(t_simple_command *simple_command,
	const char *file_path)
{
	write_errormessage(file_path);
	simple_command->fd_failed = true;
}

static	char	*parse_filename_through_expander(const char *filename,
	t_list *my_envp, t_list *local)
{
	char		*cleaned_filename;
	t_quotes	quotes;

	cleaned_filename = ft_strdup(filename);
	if (cleaned_filename == NULL)
		return (NULL);
	quotes.dq = false;
	quotes.sq = false;
	cleaned_filename = change_arguments(cleaned_filename, my_envp, local,
			quotes);
	return (cleaned_filename);
}

static	void	fill_command_table_input_redirection(
	t_simple_command *simple_command, t_ast_node *ast_node,
	t_list *my_envp, t_list *local)
{
	char	*file_name;

	if (ast_node->type == NODETYPE_INFILE)
	{
		if (simple_command->fd_in != -1)
			close(simple_command->fd_in);
		file_name = parse_filename_through_expander(ast_node->value.string,
				my_envp, local);
		if (file_name == NULL)
		{
			handle_error_redirection(simple_command, file_name);
			return ;
		}
		simple_command->fd_in = open(file_name, O_RDONLY);
		if (simple_command->fd_in == -1)
			handle_error_redirection(simple_command, file_name);
		free(file_name);
	}
	else
	{
		simple_command->fd_in = open(FILENAME_HEREDOC, O_RDONLY);
		if (simple_command->fd_in == -1)
			handle_error_here_doc(simple_command);
	}
}

static	void	fill_command_table_output_redirection(
	t_simple_command *simple_command, t_ast_node *ast_node, t_list *my_envp,
	t_list *local)
{
	char	*file_name;

	if (simple_command->fd_out != -1)
		close(simple_command->fd_out);
	file_name = parse_filename_through_expander(ast_node->value.string,
			my_envp, local);
	if (file_name == NULL)
	{
		handle_error_redirection(simple_command, file_name);
		return ;
	}
	if (ast_node->type == NODETYPE_OUTFILE)
	{
		simple_command->fd_out = open(file_name, O_CREAT | O_WRONLY | O_TRUNC,
				MODE_CREATEFILE);
	}
	else
	{
		simple_command->fd_out = open(file_name, O_CREAT | O_WRONLY | O_APPEND,
				MODE_CREATEFILE);
	}
	if (simple_command->fd_out == -1)
		handle_error_redirection(simple_command, ast_node->value.string);
	free(file_name);
}

void	fill_command_table_redirection(t_simple_command *simple_command,
	t_ast_node *ast_node, t_collection_list collect_list)
{
	bool	check_input_redirection;

	if (ast_node->type == NODETYPE_HEREDOC)
		create_here_doc(simple_command, ast_node->value.string);
	if (simple_command->fd_failed == true)
		return ;
	check_input_redirection = (ast_node->type == NODETYPE_INFILE
			|| ast_node->type == NODETYPE_HEREDOC);
	if (check_input_redirection)
	{
		fill_command_table_input_redirection(simple_command, ast_node,
			*(collect_list.pointer_my_envp), *(collect_list.pointer_local));
	}
	else
	{
		fill_command_table_output_redirection(simple_command, ast_node,
			*(collect_list.pointer_my_envp), *(collect_list.pointer_local));
	}
}
