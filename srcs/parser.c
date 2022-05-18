/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:50:58 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/17 15:15:59 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
parser grammar
<command_line>		::=	<job> <END>
					|	<END>
<job>				::=	<command>
						<job> <PIPE> <command>
<command>			::=	<simple_command>
					|	<simple_command> <infile>
					|	<simple_command> <outfile>
					|	<simple_command> <heredoc>
					|	<simple_command> <append>
<infile>			::= <INFILE> <WORD>
<outfile>			::= <OUTFILE> <WORD>
<heredoc>			::= <HEREDOC> <WORD>
<append>			::= <APPEND> <WORD>
<simple_command>	::=	<pathname>
					|	<pathname> <arguments>
<pathname>			::= <WORD>
<arguments>			::=	<arg>
					|	<arguments> <arg>
<arg>				::=	<WORD>
*/

//delete, just for debugging
void	print_ast_node(t_ast_node *ast_node)
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
	if (ast_node->type == NODETYPE_EMPTY)
	{
		printf("empty tree");
		return ;
	}
	if (check_nodetype_with_nodepair)
	{
		print_ast_node(ast_node->value.node_pair.left);
		printf(" - %d - ", ast_node->type);
		print_ast_node(ast_node->value.node_pair.right);
	}
	else if (check_nodetype_with_string)
		printf("(%d, %s)", ast_node->type, ast_node->value.string);
}

//delete, just for debugging
void	print_parse_range(t_parse_range parse_range)
{
	printf("parse_range: %d - %d\n", parse_range.begin, parse_range.end);
}

//delete, just for debugging
void	print_command_table(t_command_table *command_table)
{
	int	i;
	int	j;

	i = 0;
	//delete
	printf("number: %d\n", command_table->number);
	while (i < command_table->number)
	{
		//delete
		printf("number: %d, ", command_table->simple_command[i].number);
		j = 0;
		printf("%d | ", i);
		while (command_table->simple_command[i].arguments[j])
		{
			printf("%s	", command_table->simple_command[i].arguments[j]);
			j++;
		}
		printf("%s\n", command_table->simple_command[i].arguments[j]);
		i++;
	}
	printf("infile: %s, outfile: %s, here_doc: %s, append: %s\n", command_table->infile, command_table->outfile, command_table->here_doc, command_table->append);
}

void	free_command_table(t_command_table *command_table)
{
	int	i;

	if (command_table == NULL)
		return ;
	if (command_table->simple_command)
	{
		i = 0;
		while (i < command_table->number)
		{
			ft_free_2d_array_nullterminated(
				(void **) command_table->simple_command[i].arguments);
			i++;
		}
	}
	ft_destroy((void *) command_table->simple_command);
	ft_destroy((void *) command_table->infile);
	ft_destroy((void *) command_table->outfile);
	ft_destroy((void *) command_table->here_doc);
	ft_destroy((void *) command_table->append);
	ft_destroy((void *) command_table);
}

int	calc_number_token_in_range(t_parse_range parser_range)
{
	return (parser_range.end - parser_range.begin);
}

t_parse_range	create_parse_range(int range_begin, int range_end)
{
	t_parse_range	new_parse_range;

	new_parse_range.begin = range_begin;
	new_parse_range.end = range_end;
	return (new_parse_range);
}

//terrible function name
bool	check_ast_node(t_ast_node *node)
{
	bool	check_nodetype_with_nodepair;

	if (node == NULL)
		return (false);
	check_nodetype_with_nodepair = node->type == NODETYPE_JOB
			|| node->type == NODETYPE_COMMAND
			|| node->type == NODETYPE_SIMPLECOMMAND
			|| node->type == NODETYPE_ARGUMENTS;
	if (node->type == NODETYPE_EMPTY)
		return (true);
	if (check_nodetype_with_nodepair)
	{
		return (node->value.node_pair.left != NULL
			&& node->value.node_pair.right != NULL);
	}
	return (node->value.string);
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

//just for now
t_ast_node	*parse_arguments(t_token *token, t_parse_range parse_range);

t_ast_node	*create_arguments_node(t_token *token, t_parse_range parse_range)
{
	t_ast_node		*arguments_node;
	t_parse_range	arguments_parse_range;

	arguments_node = create_ast_node(NODETYPE_ARGUMENTS);
	if (arguments_node == NULL)
		return (NULL);
	arguments_parse_range = create_parse_range(parse_range.begin + 1,
		parse_range.end);
	arguments_node->value.node_pair.left = create_leaf_node(NODETYPE_ARG,
			token[parse_range.begin].string_value);
	arguments_node->value.node_pair.right = parse_arguments(token,
			arguments_parse_range);
	return (arguments_node);
}

t_ast_node	*parse_arguments(t_token *token, t_parse_range parse_range)
{
	t_ast_node	*ast_node;
	int			number_tokens_in_range;

	number_tokens_in_range = calc_number_token_in_range(parse_range);
	if (number_tokens_in_range == 1)
	{
		ast_node = create_leaf_node(NODETYPE_ARG,
				token[parse_range.begin].string_value);
	}
	else
	{
		ast_node = create_arguments_node(token, parse_range);
		if (check_ast_node(ast_node) == false)
		{
			free(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

t_ast_node	*create_simple_command_node(t_token *token,
	t_parse_range parse_range)
{
	t_ast_node		*simple_command_node;
	t_parse_range	arguments_parse_range;

	simple_command_node = create_ast_node(NODETYPE_SIMPLECOMMAND);
	if (simple_command_node == NULL)
		return (NULL);
	arguments_parse_range = create_parse_range(parse_range.begin + 1,
		parse_range.end);
	simple_command_node->value.node_pair.left = create_leaf_node(
			NODETYPE_PATHNAME, token[parse_range.begin].string_value);
	simple_command_node->value.node_pair.right = parse_arguments(token,
			arguments_parse_range);
	return (simple_command_node);
}

t_ast_node	*parse_simple_command(t_token *token, t_parse_range parse_range)
{
	t_ast_node	*ast_node;
	int			number_token_in_range;

	number_token_in_range = calc_number_token_in_range(parse_range);
	if (number_token_in_range == 1)
	{
		ast_node = create_leaf_node(NODETYPE_PATHNAME,
				token[parse_range.begin].string_value);
	}
	else
	{
		ast_node = create_simple_command_node(token, parse_range);
		if (check_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

bool	check_redirection_token(t_token token)
{
	return (token.type == TOKENTYPE_INFILE || token.type == TOKENTYPE_OUTFILE
		|| token.type == TOKENTYPE_HEREDOC || token.type == TOKENTYPE_APPEND);
}

bool	check_redirection(t_token *token, int index_redirection,
	int number_token_in_range)
{
	if (number_token_in_range < MINTOKEN_REDIRECTION)
		return (false);
	return (check_redirection_token(token[index_redirection]));
}

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

t_ast_node	*create_command_node(t_token *token, t_parse_range parse_range,
	int index_redirection)
{
	t_ast_node		*command_node;
	t_parse_range	simple_command_parse_range;
	t_astnodetype	redirection_type;

	command_node = create_ast_node(NODETYPE_COMMAND);
	if (command_node == false)
		return (NULL);
	simple_command_parse_range = create_parse_range(parse_range.begin,
			index_redirection);
	redirection_type = look_up_redirection_type(token[index_redirection]);
	command_node->value.node_pair.left = parse_simple_command(token,
		simple_command_parse_range);
	command_node->value.node_pair.right = create_leaf_node(redirection_type,
		token[parse_range.end - 1].string_value);
	return (command_node);
}

t_ast_node	*parse_command(t_token *token, t_parse_range parse_range)
{
	t_ast_node	*ast_node;
	int			number_token_in_range;
	int			index_redirection;

	number_token_in_range = calc_number_token_in_range(parse_range);
	index_redirection = parse_range.end - RANGE_FINDREDIRECTION;
	if (check_redirection(token, index_redirection, number_token_in_range))
	{
		ast_node = create_command_node(token, parse_range, index_redirection);
		if (check_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	else
		ast_node = parse_simple_command(token, parse_range);
	return (ast_node);
}

int	search_pipe_token(t_token *token, t_parse_range parse_range)
{
	int	number_token_in_range;
	int	i;

	number_token_in_range = calc_number_token_in_range(parse_range);
	if (number_token_in_range < MINTOKEN_PIPE)
		return (PIPETOKEN_NOTFOUND);
	i = parse_range.end - 1;
	while (i >= parse_range.begin)
	{
		if (token[i].type == TOKENTYPE_PIPE)
			return (i);
		i--;
	}
	return (PIPETOKEN_NOTFOUND);
}

//just for now
t_ast_node	*parse_job(t_token *token, t_parse_range parse_range);

t_ast_node	*create_job_node(t_token *token, t_parse_range parse_range,
	int index_pipe_token)
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
	job_node->value.node_pair.left = parse_job(token, job_parse_range);
	job_node->value.node_pair.right = parse_command(token, command_parse_range);
	return (job_node);
}

t_ast_node	*parse_job(t_token *token, t_parse_range parse_range)
{
	t_ast_node		*ast_node;
	int				index_pipe_token;

	index_pipe_token = search_pipe_token(token, parse_range);
	if (index_pipe_token == PIPETOKEN_NOTFOUND)
		ast_node = parse_command(token, parse_range);
	else
	{
		ast_node = create_job_node(token, parse_range, index_pipe_token);
		if (check_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

bool	check_end_token(t_token *token, int number_of_tokens)
{
	return (token[number_of_tokens - 1].type == TOKENTYPE_END);
}

t_ast_node	*parse_command_line(t_token *token, int number_of_tokens)
{
	t_ast_node		*ast_tree;
	t_parse_range	job_parse_range;

	if (check_end_token(token, number_of_tokens) == false)
		return (NULL);
	if (number_of_tokens == 1)
		ast_tree = create_ast_node(NODETYPE_EMPTY);
	else
	{
		job_parse_range = create_parse_range(0, number_of_tokens - 1);
		ast_tree = parse_job(token, job_parse_range);
	}
	return (ast_tree);
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

int	count_root_nodes(t_ast_node *ast_node, t_astnodetype root_node_type)
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

int	count_number_simple_commands(t_ast_node *ast_tree)
{
	t_astnodetype	root_node_type;
	int				number_root_nodes;

	root_node_type = ast_tree->type;
	number_root_nodes = count_root_nodes(ast_tree, root_node_type);
	if (root_node_type == NODETYPE_JOB)
		return (number_root_nodes + 1);
	return (number_root_nodes);
}

t_command_table	*init_command_table(int number_simple_commands)
{
	t_command_table *command_table;
	int				i;

	command_table = (t_command_table *) malloc(sizeof(t_command_table));
	if (command_table == NULL)
		return (NULL);
	command_table->number = number_simple_commands;
	command_table->simple_command = (t_simple_command *)
			malloc((command_table->number) * sizeof(t_simple_command));
	if (command_table->simple_command == NULL)
	{
		free(command_table);
		return (NULL);
	}
	i = 0;
	while (i < command_table->number)
	{
		command_table->simple_command[i].arguments = NULL;
		i++;
	}
	command_table->infile = NULL;
	command_table->outfile = NULL;
	command_table->here_doc = NULL;
	command_table->append = NULL;
	return (command_table);
}

//terrible name
bool	check_higher_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_JOB
		|| ast_node->type == NODETYPE_COMMAND
		|| ast_node->type == NODETYPE_ARGUMENTS);
}

bool	check_redirection_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_INFILE
		|| ast_node->type == NODETYPE_OUTFILE
		|| ast_node->type == NODETYPE_HEREDOC
		|| ast_node->type == NODETYPE_APPEND);
}

int	count_number_of_arguments(t_ast_node *ast_node)
{
	if (check_leaf_node(ast_node))
		return (1);
	return (count_number_of_arguments(ast_node->value.node_pair.left)
		+ count_number_of_arguments(ast_node->value.node_pair.right));
}

void	fill_command_table_simple_command(t_command_table *command_table,
	t_ast_node *ast_node, int *index_simple_command, int *index_argument)
{
	int	number_of_arguments;

	number_of_arguments = count_number_of_arguments(ast_node);
	command_table->simple_command[*index_simple_command].arguments
		= malloc((number_of_arguments + 1) * sizeof(char *));
	if (command_table->simple_command[*index_simple_command].arguments
		== NULL)
	{
		free_command_table(command_table);
		return ;
	}
	command_table->simple_command[*index_simple_command].number
		= number_of_arguments;
	*index_argument = 0;
	fill_command_table(command_table, ast_node->value.node_pair.left,
		index_simple_command, index_argument);
	fill_command_table(command_table, ast_node->value.node_pair.right,
		index_simple_command, index_argument);
	command_table->simple_command[*index_simple_command].arguments[
		*index_argument] = NULL;
	(*index_simple_command)++;
}

bool	check_command_no_arguments(t_command_table *command_table,
	int index_simple_command)
{
	return (command_table->simple_command[index_simple_command].arguments
		== NULL);
}

void	fill_command_table_only_pathname(t_command_table *command_table,
	t_ast_node *ast_node, int *index_simple_command)
{
	command_table->simple_command[*index_simple_command].arguments
		= (char **) malloc (2 * sizeof(char *));
	if (command_table->simple_command[*index_simple_command].arguments
		== NULL)
	{
		free_command_table(command_table);
		return ;
	}
	command_table->simple_command[*index_simple_command].arguments[0]
		= ft_strdup(ast_node->value.string);
	if (command_table->simple_command[*index_simple_command].arguments[0]
		== NULL)
	{
		free_command_table(command_table);
		return ;
	}
	command_table->simple_command[*index_simple_command].arguments[1]
		= NULL;
	command_table->simple_command[*index_simple_command].number = 1;
	(*index_simple_command)++;
}

void	fill_command_table_pathname(t_command_table *command_table,
	t_ast_node *ast_node, int *index_simple_command, int *index_argument)
{
	if (check_command_no_arguments(command_table, *index_simple_command))
	{
		fill_command_table_only_pathname(command_table, ast_node,
			index_simple_command);
	}
	else
	{
		command_table->simple_command[*index_simple_command].arguments[
			*index_argument] = ft_strdup(ast_node->value.string);
		(*index_argument)++;
	}
}

void	overwrite_redirection(char **redirection_ptr, const char *file_name,
	t_command_table *command_table)
{
	free(*redirection_ptr);
	*redirection_ptr = ft_strdup(file_name);
	if (*redirection_ptr == NULL)
		free_command_table(command_table);
}

//not the cleanest way!
void	fill_command_table_redirection(t_command_table *command_table,
	t_ast_node *ast_node)
{
	if (ast_node->type == NODETYPE_INFILE)
	{
		overwrite_redirection(&command_table->infile, ast_node->value.string,
			command_table);
	}
	else if (ast_node->type == NODETYPE_OUTFILE)
	{
		overwrite_redirection(&command_table->outfile, ast_node->value.string,
			command_table);
	}
	else if (ast_node->type == NODETYPE_HEREDOC)
	{
		overwrite_redirection(&command_table->here_doc, ast_node->value.string,
			command_table);
	}
	else
	{
		overwrite_redirection(&command_table->append, ast_node->value.string,
			command_table);
	}
}

void	fill_command_table(t_command_table *command_table, t_ast_node *ast_node,
	int *index_simple_command, int *index_argument)
{
	if (command_table == NULL)
		return ;
	if (check_higher_node(ast_node))
	{
		fill_command_table(command_table, ast_node->value.node_pair.left,
			index_simple_command, index_argument);
		fill_command_table(command_table, ast_node->value.node_pair.right,
			index_simple_command, index_argument);
	}
	else if (ast_node->type == NODETYPE_SIMPLECOMMAND)
	{
		fill_command_table_simple_command(command_table, ast_node,
			index_simple_command, index_argument);
	}
	else if (ast_node->type == NODETYPE_PATHNAME)
	{
		fill_command_table_pathname(command_table, ast_node,
			index_simple_command, index_argument);
	}
	else if (ast_node->type == NODETYPE_ARG)
	{
		command_table->simple_command[*index_simple_command].arguments[
			*index_argument] = ft_strdup(ast_node->value.string);
		(*index_argument)++;
	}
	else if (check_redirection_node(ast_node))
		fill_command_table_redirection(command_table, ast_node);
}

t_command_table *create_command_table(t_ast_node *ast_tree)
{
	t_command_table	*command_table;
	int				number_simple_commands;
	int				index_simple_command;
	int				index_argument;

	number_simple_commands = count_number_simple_commands(ast_tree);
	command_table = init_command_table(number_simple_commands);
	if (!command_table)
		return (NULL);
	index_simple_command = 0;
	index_argument = 0;
	fill_command_table(command_table, ast_tree, &index_simple_command,
		&index_argument);
	//delete, just for debugging
	printf("-------------------------------\nprint command_table\n");
	print_command_table(command_table);
	printf("-------------------------------\n");
	return (command_table);
}

t_command_table	*build_command_table(t_token *token, int number_of_tokens)
{
	t_command_table	*command_table;
	t_ast_node		*ast_tree;

	ast_tree = parse_command_line(token, number_of_tokens);
	if (check_ast_node(ast_tree) == false)
	{
		free_ast_node(ast_tree);
		free_token(token);
		exit(EXIT_FAILURE);
	}
	//delete, just for debugging
	printf("-------------------------------\nprint ast_tree\n");
	print_ast_node(ast_tree);
	printf("\n-------------------------------\n");
	//just for visualisation for now, handle differently
	if (ast_tree->type == NODETYPE_EMPTY)
	{
		printf("empty input\n");
		command_table = NULL;
	}
	else
		command_table = create_command_table(ast_tree);
	free_ast_node(ast_tree);
	return (command_table);
}

/*
int	built_in_check(char **arguments, t_list *my_envp)
{
	if (ft_strncmp(arguments[0], "echo", 5) == 0)
		ft_echo(arguments);
	if (ft_strncmp(arguments[0], "cd", 3) == 0)
		my_cd(arguments, &my_envp);
	if (ft_strncmp(arguments[0], "pwd", 4) == 0)
		my_pwd(my_envp, arguments);
	if (ft_strncmp(arguments[0], "export", 7) == 0)
		my_export(arguments, &my_envp, local);
	if (ft_strncmp(arguments[0], "unset", 6) == 0)
		my_unset(arguments, my_envp);
	if (ft_strncmp(arguments[0], "env", 4) == 0)
		my_env(my_envp, arguments);
	if (ft_strncmp(arguments[0], "exit", 5) == 0)
		my_exit(arguments);
	return (0);
}
*/

int	built_in_check(char **arguments, t_list *my_envp)
{
	if (ft_strncmp(arguments[0], "echo", 5) == 0)
	{
		ft_echo(arguments);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "cd", 3) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(arguments[0], "pwd", 4) == 0)
	{
		my_pwd(my_envp, arguments);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "export", 7) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(arguments[0], "unset", 6) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(arguments[0], "env", 4) == 0)
	{
		my_env(my_envp, arguments);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "exit", 5) == 0)
	{
		return (1);
	}
	return (0);
}

//these built ins are not allowed to be in a pipe
int	non_pipe_built_in_check(int command_id, char **arguments, t_list *my_envp,
	t_list *local, t_command_table *command_table)
{
	bool	check_only_command;

	check_only_command = command_id == 0;
	if (check_only_command == false)
		return (0);
	if (ft_strncmp(arguments[0], "cd", 3) == 0)
	{
		my_cd(arguments, &my_envp);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "export", 7) == 0)
	{
		my_export(arguments, &my_envp, local);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "unset", 6) == 0)
	{
		my_unset(arguments, my_envp);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "exit", 5) == 0)
	{
		my_exit(my_envp, arguments, command_table);
		return (1);
	}
	return (0);
}
