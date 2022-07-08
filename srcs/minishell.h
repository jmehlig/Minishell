/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:36:09 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/23 09:19:55 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PIPETOKEN_NOTFOUND -1
# define MINTOKEN_REDIRECTION 2
# define PARSERANGE_NOREDIRECTION -1
# define PARSERANGE_ERROR -2

# define FILENAME_HEREDOC "heredoc_tmp"

# define PATH_DELIMITER ':'

# define LASTSYSTEMERRNO 106
# define ERRMSG_COMMANDNOACCESS "is a directory"
# define ERRMSG_COMMANDNOTFOUND "command not found"
# define ERRMSG_HOMENOTFOUND "HOME not set"
# define ERRMSG_OLDPWDNOTFOUND "OLDPWD not set"
# define ERRMSG_IDENTIFIER "not a valid identifier"
# define ERRMSG_EXIT "too many arguments"
# define ERRMSG_EXITNUMERIC "numeric argument required"
# define ERRMSG_NOENDTOKEN "couldn't find END TOKEN"
# define ERRMSG_QUOTE "there is no closing quote"

# define PARSERERRMSG_START "syntax error near unexpected token "
# define PARSERERRMSG_GENERAL "`newline'"
# define PARSERERRMSG_PIPE "`|'"
# define PARSERERRMSG_INFILE "`<'"
# define PARSERERRMSG_OUTFILE "`>'"
# define PARSERERRMSG_HEREDOC "`<<'"
# define PARSERERRMSG_APPEND "`>>'"
# define PARSERERRMSG_OPENPIPE "syntax error: unexpected end of file"

# define MODE_CREATEFILE 0644

# define INDEX_COMMANDNAME 0
# define PROCESSID_MAIN -1
# define CALL_MYEXIT 2

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <string.h>
# include <sgtty.h>
# include <setjmp.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <sys/socket.h>

# include "../includes/libft/libft.h"

# include <dirent.h>

typedef struct s_simple_command
{
	int		number;
	char	**arguments;
	int		fd_in;
	int		fd_out;
	bool	fd_failed;
	int		process_id;
}	t_simple_command;

typedef struct s_command_table
{
	int					number;
	t_simple_command	*simple_command;
}	t_command_table;

typedef enum e_tokentype
{
	TOKENTYPE_NEWLINE = 0,
	TOKENTYPE_INFILE = 1,
	TOKENTYPE_OUTFILE = 2,
	TOKENTYPE_HEREDOC = 3,
	TOKENTYPE_APPEND = 4,
	TOKENTYPE_PIPE = 5,
	TOKENTYPE_WORD = 6,
	TOKENTYPE_NOTOKEN = 7,
	TOKENTYPE_END = 8
}	t_tokentype;

typedef struct s_token
{
	t_tokentype	type;
	char		*string_value;
}	t_token;

typedef enum e_astnodetype
{
	NODETYPE_JOB = 0,
	NODETYPE_COMMAND = 1,
	NODETYPE_SIMPLECOMMAND = 2,
	NODETYPE_IOCHANGE = 3,
	NODETYPE_ARGUMENTS = 4,
	NODETYPE_PATHNAME = 5,
	NODETYPE_INFILE = 6,
	NODETYPE_OUTFILE = 7,
	NODETYPE_HEREDOC = 8,
	NODETYPE_APPEND = 9,
	NODETYPE_ARG = 10,
	NODETYPE_EMPTY = 11
}	t_astnodetype;

typedef struct s_ast_node	t_ast_node;

typedef struct s_nodepair
{
	t_ast_node	*left;
	t_ast_node	*right;
}	t_nodepair;

typedef union u_astnodevalue
{
	t_nodepair	node_pair;
	char		*string;
}	t_astnodevalue;

struct s_ast_node
{
	t_astnodetype	type;
	t_astnodevalue	value;
};

typedef struct s_parse_range
{
	int	begin;
	int	end;
}	t_parse_range;

typedef struct s_fd_pair
{
	int	in;
	int	out;
}	t_fd_pair;

typedef struct s_fd_utils
{
	t_fd_pair	command_fd;
	t_fd_pair	default_fd;
}	t_fd_utils;

typedef struct s_quotes
{
	bool	sq;
	bool	dq;
	bool	space;
}	t_quotes;

typedef enum e_errno_custom
{
	ERRNO_COMMANDNOACCESS = 126,
	ERRNO_COMMANDNOTFOUND = 127,
	ERRNO_PARSER = 258,
	ERRNO_HOMENOTFOUND = 259,
	ERRNO_OLDPWDNOTFOUND = 260,
	ERRNO_SIGNALB = 130,
	ERRNO_CALLEXIT = 299,
	ERRNO_IDENTIFIER = 300,
	ERRNO_EXIT = 301,
	ERRNO_EXITNUMERIC = 302,
	ERRNO_NOENDTOKEN = 303,
	ERRNO_QUOTE = 304,
}	t_errno_custom;

typedef enum e_parser_error
{
	PARSERERROR_NOERROR = 0,
	PARSERERROR_GENERAL = 1,
	PARSERERROR_PIPE = 2,
	PARSERERROR_INFILE = 3,
	PARSERERROR_OUTFILE = 4,
	PARSERERROR_HEREDOC = 5,
	PARSERERROR_APPEND = 6,
	PARSERERROR_OPENPIPE = 7,
}	t_parser_error;

typedef enum e_signal
{
	BEGIN,
	BLOCKING,
	HEREDOC,
	OPENPIPE
}	t_signal;

typedef struct s_collection_list
{
	t_list	**pointer_my_envp;
	t_list	**pointer_local;
}	t_collection_list;

typedef struct s_create_ct_utils
{
	int	index_sc;
	int	index_arg;
}	t_create_ct_utils;

//debug delete!!!
void			print_token(t_token *token);
void			print_linkedlist(t_list *linked_list);
void			print_ast_tree(t_ast_node *ast_tree);
void			print_ast_node(t_ast_node *ast_node);
void			print_parse_range(t_parse_range parse_range);
void			print_command_table(t_command_table *command_table);

//ast_node
t_ast_node		*create_ast_node(t_astnodetype node_type);
bool			check_node_with_nodepair(t_ast_node *ast_node);
bool			check_node_with_string(t_ast_node *ast_node);
bool			check_valid_ast_node(t_ast_node *node);
void			free_ast_node(t_ast_node *ast_node);

//built_ins
t_list			*check_already_there(char **arguments, t_list *my_envp,
					int i, int j);
int				my_export(char **arguments, t_list **my_envp, t_list **local);
int				my_exit(char **arguments, t_command_table *command_table,
					t_list **my_envp, t_list **local);
int				my_pwd(void);
int				my_env(t_list *my_envp, char **arguments);
bool			check_valid_variable_name(char *variable,
					int size_variable_name);
bool			check_valid_variable_allocation(char *s);
int				my_unset(char **arguments, t_list **my_envp, t_list **local);

//check_path
bool			check_path_is_absolute(const char *path);
char			*create_absolute_path_not_canonical(const char *path);
char			*create_absolute_path(const char *path);
bool			check_path_is_directory(const char *path);
bool			check_path_is_executable(const char *path);

//convert_to_canonical_form
char			*convert_to_canonical_form(const char *absolute_path);
char			*remove_dot_dot_component(const char *path_cleaned);
char			*remove_dot_component_and_repeating_slashes(const char *path);
char			*while_dot_dot(char *curpath_canonical,
					int *index_previous_slash, int *amount_to_remove, int *i);
bool			check_dot_dot_component(int index, const char *curpath);

//echo
int				ft_echo(char **arguments);
int				option_input(char *builtin);

//error
void			write_errormessage(const char *additional_message);
void			write_errormessage_parser(t_parser_error parser_error);
void			write_errormessage_builtin(const char *builtin_name,
					const char *additional_message);
void			write_errormessage_createprogrampath(const char *program_name,
					t_list *envp);
bool			check_found_slash(const char *str);
char			*get_name_variable(const char *variable);
bool			variable_in_list(t_list *list, char *variable_name);

//executor
void			execute(t_command_table *command_table, t_list **my_envp,
					t_list **local);
void			free_executor(t_command_table *command_table, t_list **my_envp,
					t_list **local);
void			exit_executor(int error_code, t_command_table *command_table,
					t_list **my_envp, t_list **local);
bool			init_fd_utils(t_fd_utils *fd_utils);
bool			set_first_command_input(t_fd_utils *fd_utils);
bool			restore_default_fds(t_fd_pair default_fd);
bool			redirect_command_fd(int command_id,
					t_simple_command simple_command, t_fd_utils *fd_utils,
					int number_of_commands);
int				execute_commands(t_command_table *command_table,
					t_fd_utils *fd_utils, t_list **my_envp, t_list **local);
int				create_child_process(int command_id,
					t_command_table *command_table,
					t_collection_list collect_list, int fd_to_close);
void			execute_simple_command(int command_id,
					t_command_table *command_table, t_list **my_envp,
					t_list **local);
char			*create_programpath(const char *program_name, t_list *my_envp);
bool			check_found_programpath(const char *program_path);
char			*create_executable_path(const char *program_name);
bool			check_only_allocations(char **arguments);
int				built_in_check(char **arguments, t_command_table *command_table,
					t_list **my_envp, t_list **local);
int				called_single_builtin(char **arguments,
					t_command_table *command_table, t_list **my_envp,
					t_list **local);
int				single_built_in_check(int command_id,
					t_command_table *command_table, t_list **my_envp,
					t_list **local);

//expander
char			*delete_quote(char *argument, int j);
char			*change_arguments(char *arguments, t_list *my_envp,
					t_list *local, t_quotes	quotes);
char			**get_quotes_changed(char **arguments,
					t_list *my_envp, t_list *local);
char			*get_v_name(char *a, t_quotes *quotes, int j, int *i1);
char			*get_v_value(char *argument, t_list *my_envp,
					t_quotes *quotes, int j);
void			expander(t_command_table *command_table, t_list *my_envp,
					t_list *local);

//ft_split_quotes
char			**ft_split_quotes(char *str);
void			quote_checker(char str, t_quotes *quotes);
bool			check_splitter(char c);
int				check_accepted_symbols(const char *s);
int				count_word_chars(char *str, int i, t_quotes *quotes);

//lexer
t_token			*build_tokens(char *input, int *number_of_tokens);
t_token			fill_token(char *input);
void			free_token(t_token *token);

//list_utils
int				check_found_variable_identifier(const char *str_to_check,
					const char *variable_name);
char			*create_variable(const char *variable_name,
					const char *variable_value);
char			*get_value_variable(t_list *list, const char *variable_name);
char			*get_variable(t_list *list, const char *variable_name);
bool			update_value_variable(const char *variable_name,
					const char *variable_value, t_list **list);
bool			append_variable(const char *variable, t_list **list);
bool			append_entry(const char *variable_name,
					const char *variable_value, t_list **list);
bool			add_entry(const char *variable_name, const char *variable_value,
					t_list **list);
bool			delete_variable(const char *variable, t_list **list);
bool			check_variable_has_no_value(const char *variable_value);
char			*pull_value_variable(const char *variable);
void			free_content(void *content);

//my_cd
int				my_cd(char **arguments, t_list **my_envp);

//parser
t_command_table	*build_command_table(t_token **token, int number_of_tokens,
					t_list **my_envp, t_list **local);
t_ast_node		*parse_command_line(t_token **token, int number_of_tokens);
t_ast_node		*parse_job(t_token **token, t_parse_range parse_range,
					t_parser_error *parser_error);
t_ast_node		*complete_input(t_token **token, int number_of_token);
t_ast_node		*parse_command(t_token *token, t_parse_range parse_range,
					t_parser_error *parser_error);
bool			check_redirection_token(t_token token);
bool			check_io_change(t_token *token, t_parse_range parse_range);
bool			check_found_redirection(int index_last_redirection,
					int index_begin);
t_ast_node		*parse_io_change(t_token *token, t_parse_range parse_rang_front,
					t_parse_range parse_range_back);
bool			check_only_one_redirection(t_parse_range parse_range_front,
					t_parse_range parse_range_back);
t_ast_node		*parse_simple_command(t_token *token, t_parse_range parse_range,
					t_parser_error *parser_error);
t_ast_node		*parse_arguments(t_token *token, t_parse_range parse_range,
					t_parser_error *parser_error);
t_parse_range	create_parse_range(int range_begin, int range_end);
int				calc_number_token_in_range(t_parse_range parse_range);
t_parse_range	create_no_redirection_parse_range(void);
bool			check_no_redirection_parse_range(t_parse_range parse_range);
t_parse_range	create_error_redirection_parse_range(int index_wrong_token);
bool			check_error_redirection(t_parse_range io_change_front,
					t_parse_range io_change_back, t_token *token,
					t_parser_error *parser_error);
t_command_table	*create_command_table(t_ast_node *ast_tree,
					t_collection_list collect_list);
void			fill_command_table(t_command_table **command_table,
					t_ast_node *ast_node, t_create_ct_utils *create_ct_utils,
					t_collection_list collect_list);
void			fill_command_table_redirection(t_simple_command *simple_command,
					t_ast_node *ast_node, t_collection_list collect_list);
void			handle_error_here_doc(t_simple_command *simple_command);
void			create_here_doc(t_simple_command *simple_command,
					const char *terminater);
bool			fill_command_table_arg(t_simple_command *simple_command,
					int index_argument, const char *arg);
bool			fill_command_table_pathname(t_simple_command *simple_command,
					int index_argument, const char *path_name);
void			fill_command_table_simple_command(
					t_command_table **command_table, t_ast_node *ast_node,
					t_create_ct_utils *create_ct_utils,
					t_collection_list collect_list);
t_ast_node		*create_leaf_node(t_astnodetype node_type, char *string_value);
void			free_command_table(t_command_table **command_table);
bool			check_leaf_node(t_ast_node *ast_node);

//signals
void			handle_heredoc(int signal);
void			change_termios(bool del_written);
void			handle_begin(int signal);
void			signal_handling(t_signal signal);
void			handle_blocking(int signal);

#endif
