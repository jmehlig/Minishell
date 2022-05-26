/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:50:58 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/26 12:03:23 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//delete, just for debugging
/*
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
*/

//delete, just for debugging
static	void	print_ast_node_util(t_ast_node *ast_node, int space)
{
	bool		check_nodetype_with_nodepair;
	bool		check_nodetype_with_string;

	if (ast_node == NULL)
		return ;
	check_nodetype_with_nodepair = ast_node->type == NODETYPE_JOB
			|| ast_node->type == NODETYPE_COMMAND
			|| ast_node->type == NODETYPE_SIMPLECOMMAND
			|| ast_node->type == NODETYPE_IOCHANGE
			|| ast_node->type == NODETYPE_ARGUMENTS;
	check_nodetype_with_string = check_nodetype_with_nodepair == false
			&& ast_node->type != NODETYPE_EMPTY;
	if (ast_node->type == NODETYPE_EMPTY)
	{
		space += 4;
		for (int i = 0; i < space; i++)
			printf(" ");
		printf("empty\n");
		return ;
	}
	if (check_nodetype_with_nodepair)
	{
		space += 4;
		print_ast_node_util(ast_node->value.node_pair.left, space);
		for (int i = 0; i < space; i++)
			printf(" ");
		printf("%d\n", ast_node->type);
		print_ast_node_util(ast_node->value.node_pair.right, space);
	}
	else if (check_nodetype_with_string)
	{
		space += 4;
		for (int i = 0; i < space; i++)
			printf(" ");
		printf("(%d, %s)\n", ast_node->type, ast_node->value.string);
	}
}

//delete, just for debugging
void	print_ast_node(t_ast_node *ast_node)
{
	if (ast_node == NULL)
		return ;
	print_ast_node_util(ast_node, 0);
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
	printf("-------------------------------\nprint command_table\n\n");
	//printf("number: %d\n", command_table->number);
	while (i < command_table->number)
	{
		//printf("number: %d, ", command_table->simple_command[i].number);
		j = 0;
		printf("%d | ", i);
		while (command_table->simple_command[i].arguments[j])
		{
			printf("%s	", command_table->simple_command[i].arguments[j]);
			j++;
		}
		printf("%s", command_table->simple_command[i].arguments[j]);
		printf("	|| in: %d	| out: %d	| fail: %d\n", command_table->simple_command[i].fd_in,
			command_table->simple_command[i].fd_out, command_table->simple_command[i].fd_failed);
		i++;
	}
	printf("-------------------------------\n");
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
	ft_destroy((void *) command_table);
}

void	write_parser_errormessage(t_parser_error parser_error)
{
	if (parser_error == PARSERERROR_PIPE)
	{
		write(STDERR_FILENO, PARSERERRORMSG_PIPE,
			ft_strlen(PARSERERRORMSG_PIPE));
		write(STDERR_FILENO, "\n", 1);
		errno = ERRNO_PARSER;
	}
	else if (parser_error == PARSERERROR_GENERAL)
	{
		write(STDERR_FILENO, PARSERERRORMSG_GENERAL,
			ft_strlen(PARSERERRORMSG_GENERAL));
		write(STDERR_FILENO, "\n", 1);
		errno = ERRNO_PARSER;
	}
	else
		write_errormessage(NULL);
}

int	calc_number_token_in_range(t_parse_range parse_range)
{
	if (parse_range.begin < 0 || parse_range.end < 0)
		return (0);
	return (parse_range.end - parse_range.begin);
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
t_ast_node	*parse_arguments(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error);

t_ast_node	*create_arguments_node(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
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
			arguments_parse_range, parser_error);
	return (arguments_node);
}

t_ast_node	*parse_arguments(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;
	int			number_tokens_in_range;

	//node type check
	if (token[parse_range.begin].type != TOKENTYPE_WORD)
	{
		*parser_error = PARSERERROR_GENERAL;
		return (NULL);
	}
	number_tokens_in_range = calc_number_token_in_range(parse_range);
	if (number_tokens_in_range == 1)
	{
		ast_node = create_leaf_node(NODETYPE_ARG,
				token[parse_range.begin].string_value);
	}
	else
	{
		ast_node = create_arguments_node(token, parse_range, parser_error);
		if (check_ast_node(ast_node) == false)
		{
			free(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

t_ast_node	*create_simple_command_node(t_token *token,
	t_parse_range parse_range, t_parser_error *parser_error)
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
			arguments_parse_range, parser_error);
	return (simple_command_node);
}

t_ast_node	*parse_simple_command(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;
	int			number_token_in_range;

	number_token_in_range = calc_number_token_in_range(parse_range);
	if (number_token_in_range == 0)
	{
		ast_node = create_ast_node(NODETYPE_EMPTY);
		return (ast_node);
	}
	//node type check
	if (token[parse_range.begin].type != TOKENTYPE_WORD)
	{
		*parser_error = PARSERERROR_GENERAL;
		return (NULL);
	}
	if (number_token_in_range == 1)
	{
		ast_node = create_leaf_node(NODETYPE_PATHNAME,
				token[parse_range.begin].string_value);
	}
	else
	{
		ast_node = create_simple_command_node(token, parse_range, parser_error);
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

bool	check_io_change(t_token *token, t_parse_range parse_range)
{
	bool	check_io_change_front;
	bool	check_io_change_back;
	int		index_redirection_token_end;

	if (calc_number_token_in_range(parse_range) < MINTOKEN_REDIRECTION)
		return (false);
	check_io_change_front = check_redirection_token(
		token[parse_range.begin]);
	index_redirection_token_end = parse_range.end - MINTOKEN_REDIRECTION;
	check_io_change_back = check_redirection_token(
		token[index_redirection_token_end]);
	return (check_io_change_front || check_io_change_back);
}

bool	check_found_redirection(int index_last_redirection, int index_begin)
{
	return (index_last_redirection != index_begin);
}

//better function name?
t_parse_range	create_no_redirection_parse_range(void)
{
	t_parse_range	fail_io_change_parse_range;

	fail_io_change_parse_range.begin = PARSERANGE_NOREDIRECTION;
	fail_io_change_parse_range.end = PARSERANGE_NOREDIRECTION;
	return (fail_io_change_parse_range);
}

//better function name?
bool	check_no_redirection_parse_range(t_parse_range parse_range)
{
	return (parse_range.begin == PARSERANGE_NOREDIRECTION
		|| parse_range.end == PARSERANGE_NOREDIRECTION);
}

t_parse_range	create_io_change_front_parse_range(t_token *token,
	t_parse_range parse_range)
{
	t_parse_range	io_change_front_parse_range;
	int				index_last_redirection;

	index_last_redirection = parse_range.begin;
	while (check_redirection_token(token[index_last_redirection]))
	{
		index_last_redirection += MINTOKEN_REDIRECTION;
		//correct???
		if (index_last_redirection >= parse_range.end - 1)
			break ;
	}
	if (check_found_redirection(index_last_redirection, parse_range.begin))
	{
		index_last_redirection -= MINTOKEN_REDIRECTION;
		io_change_front_parse_range.begin = parse_range.begin;
		io_change_front_parse_range.end = index_last_redirection
				+ MINTOKEN_REDIRECTION;
	}
	else
		io_change_front_parse_range = create_no_redirection_parse_range();
	return (io_change_front_parse_range);
}

t_parse_range	create_io_change_back_parse_range(t_token *token,
	t_parse_range parse_range)
{
	t_parse_range	io_change_back_parse_range;
	int				index_last_redirection;

	index_last_redirection = parse_range.end - MINTOKEN_REDIRECTION;
	while (check_redirection_token(token[index_last_redirection]))
	{
		index_last_redirection -= MINTOKEN_REDIRECTION;
		//correct???
		if (index_last_redirection < parse_range.begin)
			break ;
	}
	if (check_found_redirection(index_last_redirection, parse_range.end
		- MINTOKEN_REDIRECTION))
	{
		index_last_redirection += MINTOKEN_REDIRECTION;
		io_change_back_parse_range.begin = index_last_redirection;
		io_change_back_parse_range.end = parse_range.end;
	}
	else
		io_change_back_parse_range = create_no_redirection_parse_range();
	return (io_change_back_parse_range);
}

t_parse_range	create_simple_command_parse_range(t_parse_range parse_range,
	t_parse_range io_change_front_parse_range,
	t_parse_range io_change_back_parse_range)
{
	t_parse_range	simple_command_parse_range;

	if (check_no_redirection_parse_range(io_change_front_parse_range))
		simple_command_parse_range.begin = parse_range.begin;
	else
		simple_command_parse_range.begin = io_change_front_parse_range.end;
	if (check_no_redirection_parse_range(io_change_back_parse_range))
		simple_command_parse_range.end = parse_range.end;
	else
		simple_command_parse_range.end = io_change_back_parse_range.begin;
	return (simple_command_parse_range);
}

//currently not checking if token are REDIRECTION TOKEN
bool	check_only_one_redirection(t_parse_range parse_range_front,
	t_parse_range parse_range_back)
{
	int			number_token_in_range_front;
	int			number_token_in_range_back;

	number_token_in_range_front = calc_number_token_in_range(parse_range_front);
	number_token_in_range_back = calc_number_token_in_range(parse_range_back);
	return (number_token_in_range_front + number_token_in_range_back
		== MINTOKEN_REDIRECTION);
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

//need a parse function for error handling???
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

t_parse_range	create_redirection_parse_range(t_parse_range *parse_range)
{
	t_parse_range	redirection_parse_range;

	redirection_parse_range.begin = parse_range->begin;
	redirection_parse_range.end = redirection_parse_range.begin
		+ MINTOKEN_REDIRECTION;
	parse_range->begin += MINTOKEN_REDIRECTION;
	if (parse_range->begin >= parse_range->end)
	{
		parse_range->begin = PARSERANGE_NOREDIRECTION;
		parse_range->end = PARSERANGE_NOREDIRECTION;
	}
	return (redirection_parse_range);
}

//just temporary
t_ast_node	*parse_io_change(t_token *token, t_parse_range parse_range_front,
	t_parse_range parse_range_back);

t_ast_node	*create_io_change_node(t_token *token,
	t_parse_range parse_range_front, t_parse_range parse_range_back)
{
	t_ast_node		*io_change_node;
	bool			check_front_io_changes;
	t_parse_range	redirection_parse_range;

	io_change_node = create_ast_node(NODETYPE_IOCHANGE);
	if (io_change_node == NULL)
		return (NULL);
	check_front_io_changes = check_no_redirection_parse_range(
			parse_range_front) == false;
	if (check_front_io_changes)
	{
		redirection_parse_range = create_redirection_parse_range(
			&parse_range_front);
	}
	else
	{
		redirection_parse_range = create_redirection_parse_range(
			&parse_range_back);
	}
	io_change_node->value.node_pair.left = create_redirection_node(token,
		redirection_parse_range);
	io_change_node->value.node_pair.right = parse_io_change(token,
		parse_range_front, parse_range_back);
	return (io_change_node);
}

//this can't fail?
t_ast_node	*parse_io_change(t_token *token, t_parse_range parse_range_front,
	t_parse_range parse_range_back)
{
	t_ast_node	*ast_node;
	bool		check_front_io_changes;

	check_front_io_changes = check_no_redirection_parse_range(
			parse_range_front) == false;
	if (check_only_one_redirection(parse_range_front, parse_range_back))
	{
		if (check_front_io_changes)
			ast_node = create_redirection_node(token, parse_range_front);
		else
			ast_node = create_redirection_node(token, parse_range_back);
	}
	else
	{
		ast_node = create_io_change_node(token, parse_range_front,
			parse_range_back);
		if (check_ast_node(ast_node) == false)
		{
			free(ast_node);
			return (NULL);
		}
	}
	return (ast_node);
}

t_ast_node	*create_command_node(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node		*command_node;
	t_parse_range	simple_command_parse_range;
	t_parse_range	io_change_front_parse_range;
	t_parse_range	io_change_back_parse_range;

	command_node = create_ast_node(NODETYPE_COMMAND);
	if (command_node == false)
		return (NULL);
	io_change_front_parse_range = create_io_change_front_parse_range(token,
		parse_range);
	if (calc_number_token_in_range(parse_range) == MINTOKEN_REDIRECTION)
		io_change_back_parse_range = create_no_redirection_parse_range();
	else
	{
		io_change_back_parse_range = create_io_change_back_parse_range(token,
				parse_range);
	}
	simple_command_parse_range = create_simple_command_parse_range(
		parse_range, io_change_front_parse_range, io_change_back_parse_range);
	command_node->value.node_pair.left = parse_simple_command(token,
		simple_command_parse_range, parser_error);
	command_node->value.node_pair.right = parse_io_change(token,
		io_change_front_parse_range, io_change_back_parse_range);
	return (command_node);
}

t_ast_node	*parse_command(t_token *token, t_parse_range parse_range,
	t_parser_error *parser_error)
{
	t_ast_node	*ast_node;

	//possibly crucial point for error handling!!!
	if (check_io_change(token, parse_range))
	{
		ast_node = create_command_node(token, parse_range, parser_error);
		if (check_ast_node(ast_node) == false)
		{
			free_ast_node(ast_node);
			return (NULL);
		}
	}
	else
		ast_node = parse_simple_command(token, parse_range, parser_error);
	return (ast_node);
}

int	search_pipe_token(t_token *token, t_parse_range parse_range)
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

//better function name?
bool	check_pipe_token(t_token *token, int index_pipe_token)
{
	if (index_pipe_token == 0)
		return (false);
	return (token[index_pipe_token - 1].type == TOKENTYPE_WORD);
}

//just for now
t_ast_node	*parse_job(t_token **token, t_parse_range parse_range,
	t_parser_error *parser_error);

t_token	dup_token(t_token token)
{
	t_token	dup_token;

	dup_token.type = token.type;
	if (dup_token.type == TOKENTYPE_WORD)
	{
		dup_token.string_value = ft_strdup(token.string_value);
		if (dup_token.string_value == NULL)
			dup_token.type = TOKENTYPE_NOTOKEN;
	}
	return (dup_token);
}

t_token	*combine_token(t_token *token, int number_of_token,
	t_token *additional_token, int number_additional_token)
{
	t_token	*combined_token;
	int		number_combined_token;
	int		i;

	number_combined_token = number_of_token + number_additional_token;
	combined_token = (t_token *) malloc(number_combined_token
		* sizeof(t_token));
	if (combined_token == NULL)
		return (NULL);
	//delete
	print_token(additional_token);
	fprintf(stderr, "number_token: %d, number_additional_token: %d\n", number_of_token, number_additional_token);
	//end delete
	i = 0;
	while (i < number_of_token)
	{
		combined_token[i] = dup_token(token[i]);
		if (combined_token[i].type == TOKENTYPE_NOTOKEN)
		{
			free_token(combined_token);
			return (NULL);
		}
		i++;
	}
	i = 0;
	while (i < number_additional_token)
	{
		combined_token[number_of_token + i] = dup_token(additional_token[i]);
		if (combined_token[i].type == TOKENTYPE_NOTOKEN)
		{
			free_token(combined_token);
			return (NULL);
		}
		i++;
	}
	return (combined_token);
}

//just for now
t_ast_node	*parse_command_line(t_token **token, int number_of_tokens);

t_ast_node	*complete_input(t_token **token, int number_of_token)
{
	t_ast_node	*ast_tree;
	t_token		*combined_token;
	t_token		*additional_token;
	int			number_additional_token;
	char		*input;

	write(STDIN_FILENO, "> ", 2);
	//!!! TODO repeat until input is not empty !!!
	input = get_next_line(0);
	if (input == NULL)															//important for signals???
		return (NULL);
	additional_token = build_tokens(input, &number_additional_token);
	free(input);
	if (additional_token == NULL)
		return (NULL);
	combined_token = combine_token(*token, number_of_token, additional_token,
		number_additional_token);
	free_token(additional_token);
	if (combined_token == NULL)
		return (NULL);
	//delete
	print_token(combined_token);
	free_token(*token);
	*token = combined_token;
	ast_tree = parse_command_line(token, number_of_token + number_additional_token);
	//delete
	print_token(*token);
	return (ast_tree);
}

t_ast_node	*create_job_node(t_token **token, t_parse_range parse_range,
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
	bool			check_pipe_last_token;

	index_pipe_token = search_pipe_token(*token, parse_range);
	if (index_pipe_token == PIPETOKEN_NOTFOUND)
		ast_node = parse_command(*token, parse_range, parser_error);
	else
	{
		if (check_pipe_token(*token, index_pipe_token) == false)
		{
			*parser_error = PARSERERROR_PIPE;
			return (NULL);
		}
		check_pipe_last_token = index_pipe_token == parse_range.end - 1;
		if (check_pipe_last_token)
			ast_node = complete_input(token, parse_range.end);
		else
		{
			ast_node = create_job_node(token, parse_range, index_pipe_token,
				parser_error);
		}
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

t_ast_node	*parse_command_line(t_token **token, int number_of_tokens)
{
	t_ast_node		*ast_tree;
	t_parse_range	job_parse_range;
	t_parser_error	parser_error;

	//can this happen?
	if (check_end_token(*token, number_of_tokens) == false)
		return (NULL);
	parser_error = PARSERERROR_NOERROR;
	job_parse_range = create_parse_range(0, number_of_tokens - 1);
	ast_tree = parse_job(token, job_parse_range, &parser_error);
	if (check_ast_node(ast_tree) == false)
	{
		free_ast_node(ast_tree);
		write_parser_errormessage(parser_error);
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
		//magic numbers???
		command_table->simple_command[i].fd_in = -1;
		command_table->simple_command[i].fd_out = -1;
		command_table->simple_command[i].fd_failed = false;
		i++;
	}
	return (command_table);
}

bool	check_connection_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_JOB
		|| ast_node->type == NODETYPE_COMMAND
		|| ast_node->type == NODETYPE_IOCHANGE
		|| ast_node->type == NODETYPE_ARGUMENTS);
}

bool	check_redirection_node(t_ast_node *ast_node)
{
	return (ast_node->type == NODETYPE_INFILE
		|| ast_node->type == NODETYPE_OUTFILE
		|| ast_node->type == NODETYPE_HEREDOC
		|| ast_node->type == NODETYPE_APPEND);
}

int	count_number_of_leafs(t_ast_node *ast_node)
{
	if (ast_node->type == NODETYPE_EMPTY)
		return (0);
	if (check_leaf_node(ast_node))
		return (1);
	return (count_number_of_leafs(ast_node->value.node_pair.left)
		+ count_number_of_leafs(ast_node->value.node_pair.right));
}

void	fill_command_table_simple_command(t_command_table *command_table,
	t_ast_node *ast_node, int *index_simple_command, int *index_argument)
{
	int	number_of_arguments;

	number_of_arguments = count_number_of_leafs(ast_node);
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
}

bool	fill_command_table_only_pathname(t_simple_command *simple_command,
	const char *path_name)
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

void	handle_error_here_doc(t_simple_command *simple_command)
{
	write_errormessage(NULL);
	unlink(FILENAME_HEREDOC);
	simple_command->fd_in = -1;
	simple_command->fd_failed = true;
}

bool	fill_here_doc(int heredoc_fd, const char *terminater)
{
	char	*next_line;

	write(STDOUT_FILENO, "> ", 2);
	next_line = get_next_line(STDIN_FILENO);
	while (next_line != NULL && ft_strncmp(next_line, terminater,
		ft_strlen(terminater) + 1) != 0)
	{
		write(heredoc_fd, next_line, ft_strlen(next_line));
		write(heredoc_fd, "\n", 1);
		free(next_line);
		write(STDOUT_FILENO, "> ", 2);
		next_line = get_next_line(STDIN_FILENO);								//do i need to change the signals?
	}
	close(heredoc_fd);
	if (next_line == NULL)
		return (false);
	free(next_line);
	return (true);
}

void	create_here_doc(t_simple_command *simple_command,
	const char *terminater)
{
	int		heredoc_fd;
	bool	success_fill_heredoc;

	if (simple_command->fd_in != -1)
		close(simple_command->fd_in);											//can this fail???
	heredoc_fd = open(FILENAME_HEREDOC, OFLAG_TRUNC, MODE_CREATEFILE);
	if (heredoc_fd == -1)
	{
		handle_error_here_doc(simple_command);
		return ;
	}
	success_fill_heredoc = fill_here_doc(heredoc_fd, terminater);
	if (success_fill_heredoc == false)
		handle_error_here_doc(simple_command);
}

void	handle_error_redirection(t_simple_command *simple_command,
	const char *file_path)
{
	write_errormessage(file_path);
	simple_command->fd_failed = true;
}

void	fill_command_table_input_redirection(t_simple_command *simple_command,
	t_ast_node *ast_node)
{
	if (ast_node->type == NODETYPE_INFILE)
	{
		if (simple_command->fd_in != -1)
			close(simple_command->fd_in);										//can this fail???
		simple_command->fd_in = open(ast_node->value.string, O_RDONLY);
		if (simple_command->fd_in == -1)
			handle_error_redirection(simple_command, ast_node->value.string);
	}
	else
	{
		simple_command->fd_in = open(FILENAME_HEREDOC, O_RDONLY);
		if (simple_command->fd_in == -1)
			handle_error_here_doc(simple_command);
	}
}

void	fill_command_table_output_redirection(t_simple_command *simple_command,
	t_ast_node *ast_node)
{
	if (simple_command->fd_out != -1)
		close(simple_command->fd_out);											//can this fail???
	if (ast_node->type == NODETYPE_OUTFILE)
	{
		simple_command->fd_out = open(ast_node->value.string,
			OFLAG_TRUNC, MODE_CREATEFILE);
	}
	else
	{
		simple_command->fd_out = open(ast_node->value.string,
			OFLAG_APPEND, MODE_CREATEFILE);
	}
	if (simple_command->fd_out == -1)
		handle_error_redirection(simple_command, ast_node->value.string);
}

void	fill_command_table_redirection(t_simple_command *simple_command,
	t_ast_node *ast_node)
{
	bool	check_input_redirection;

	if (ast_node->type == NODETYPE_HEREDOC)
		create_here_doc(simple_command, ast_node->value.string);				//can the string be NULL???
	if (simple_command->fd_failed == true)
		return ;
	check_input_redirection = ast_node->type == NODETYPE_INFILE
		|| ast_node->type == NODETYPE_HEREDOC;
	if (check_input_redirection)
		fill_command_table_input_redirection(simple_command, ast_node);
	else
		fill_command_table_output_redirection(simple_command, ast_node);
}

bool	fill_command_table_empty(t_simple_command *simple_command)
{
	simple_command->arguments = (char **) malloc(sizeof(char *));
	if (simple_command->arguments == NULL)
		return (false);
	simple_command->arguments[0] = NULL;
	simple_command->number = 0;
	return (true);
}

void	fill_command_table_type(t_command_table *command_table,
	t_ast_node *ast_node, int *index_simple_command, int *index_argument)
{
	t_simple_command	*simple_command;
	bool				function_call;

	simple_command = &(command_table->simple_command[*index_simple_command]);
	function_call = true;
	if (ast_node->type == NODETYPE_PATHNAME)
	{
		function_call = fill_command_table_pathname(simple_command,
			*index_argument, ast_node->value.string);
		(*index_argument)++;
	}
	else if (ast_node->type == NODETYPE_ARG)
	{
		function_call = fill_command_table_arg(simple_command, *index_argument,
			ast_node->value.string);
		(*index_argument)++;
	}
	else if (check_redirection_node(ast_node))
		fill_command_table_redirection(simple_command, ast_node);
	else
		function_call = fill_command_table_empty(simple_command);
	if (function_call == false)
		free_command_table(command_table);
}

void	fill_command_table(t_command_table *command_table, t_ast_node *ast_node,
	int *index_simple_command, int *index_argument)
{
	if (command_table == NULL)
		return ;
	if (check_connection_node(ast_node))
	{
		fill_command_table(command_table, ast_node->value.node_pair.left,
			index_simple_command, index_argument);
		if (ast_node->type == NODETYPE_JOB)
			(*index_simple_command)++;
		fill_command_table(command_table, ast_node->value.node_pair.right,
			index_simple_command, index_argument);
	}
	else if (ast_node->type == NODETYPE_SIMPLECOMMAND)
	{
		fill_command_table_simple_command(command_table, ast_node,
			index_simple_command, index_argument);
	}
	else
	{
		fill_command_table_type(command_table, ast_node, index_simple_command,
			index_argument);
	}
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
	//just for debugging
	print_command_table(command_table);
	return (command_table);
}

t_command_table	*build_command_table(t_token **token, int number_of_tokens)
{
	t_command_table	*command_table;
	t_ast_node		*ast_tree;

	ast_tree = parse_command_line(token, number_of_tokens);
	if (ast_tree == NULL)
	{
		write_errormessage(NULL);
		return (NULL);
	}
	printf("-------------------------------\nprint ast_tree\n\n");
	print_ast_node(ast_tree);
	printf("-------------------------------\n");
	/*
	//just for visualisation for now, handle differently
	if (ast_tree->type == NODETYPE_EMPTY)
	{
		printf("empty input\n");
		command_table = NULL;
	}
	else
	*/
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

bool	check_local_variable(char **arguments)
{
	bool	check_more_than_one_argument;
	bool	check_equal_sign_first_character;
	bool	check_local_variable;

	check_more_than_one_argument = arguments[1] != NULL;
	if (check_more_than_one_argument)
		return (false);
	check_equal_sign_first_character = arguments[0][0] == '=';
	if (check_equal_sign_first_character)
		return (false);
	check_local_variable = ft_strchr(&(arguments[0][1]), '=') != NULL;
	return (check_local_variable);
}

//!! not protected !!!
void	set_local_variable(char *argument, t_list **local)
{
	t_list	*new_variable;
	t_list	*iterate_local;
	int		size_variable_identifier;
	char	*dup_argument;

	size_variable_identifier = 0;
	while (argument[size_variable_identifier] != '=')
		size_variable_identifier++;
	size_variable_identifier++;
	iterate_local = *local;
	while (iterate_local != NULL)
	{
		if (ft_strncmp(argument, (char *) iterate_local->content, size_variable_identifier) == 0)
		{
			ft_lstdelkey(local, iterate_local->content, free_content);
			break ;
		}
		iterate_local = iterate_local->next;
	}
	//!!! protect missing !!!
	dup_argument = ft_strdup(argument);
	new_variable = ft_lstnew(dup_argument);
	ft_lstadd_back(local, new_variable);
}

int	built_in_check(char **arguments, t_list *my_envp)
{
	if (check_local_variable(arguments))
		return (1);
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

//better name?
//these built ins are not allowed to be in a pipe
int	non_pipe_built_in_check(int command_id, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	bool	check_only_one_command;
	char	**arguments;

	check_only_one_command = command_id == command_table->number - 1;
	if (check_only_one_command == false)
		return (0);
	arguments = command_table->simple_command[command_id].arguments;
	arguments = get_quotes_changed(arguments, *my_envp);
	while (*arguments != NULL)
	{
		printf("\n%s\n", *arguments);
		arguments++;
	}
	exit(1);
	if (arguments[0] == NULL)													//this check unnecessary because already checked with bool before?
		return (0);
	if (check_local_variable(arguments))
	{
		set_local_variable(arguments[0], local);
		return (1);
	}
	if (ft_strncmp(arguments[0], "cd", 3) == 0)
	{
		my_cd(arguments, my_envp);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "export", 7) == 0)
	{
		my_export(arguments, my_envp, *local);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "unset", 6) == 0)
	{
		my_unset(arguments, *my_envp);
		return (1);
	}
	else if (ft_strncmp(arguments[0], "exit", 5) == 0)
	{
		my_exit(my_envp, arguments, command_table, local);
		return (1);
	}
	return (0);
}
