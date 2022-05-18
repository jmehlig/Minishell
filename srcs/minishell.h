/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:36:09 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/17 15:12:44 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PIPETOKEN_NOTFOUND -1
# define MINTOKEN_PIPE 3
# define MINTOKEN_REDIRECTION 3
# define RANGE_FINDREDIRECTION 2

# define PATH_IDENTIFIER "PATH="
# define PATH_DELIMITER ':'
# define PATH_NOTFOUND -1

# define ERRORMESSAGE_NOPATH "no path in environment"
# define ERRORMESSAGE_COMMANDNOTFOUND "command not found"
# define ERRORMESSAGE_EXIT "exit: too many arguments"

# define MODE_OPENOUTFILE 0644

# define INDEX_COMMANDNAME 0
# define LASTPROCESSID_NOCHILDPROCESS -1

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>

# include <string.h>

# include <errno.h>
# include "../includes/libft/libft.h"
# include "../includes/get_next_line/get_next_line.h"

typedef struct s_simple_command
{
	//dont really need this, since arguments array is null terminated
	int		number;
	char	**arguments;
}		t_simple_command;

typedef struct s_command_table
{
	int					number;
	t_simple_command	*simple_command;
	char				*infile;
	char				*outfile;
	char				*here_doc;
	char				*append;
}		t_command_table;

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
	NODETYPE_ARGUMENTS = 3,
	NODETYPE_PATHNAME = 4,
	NODETYPE_INFILE = 5,
	NODETYPE_OUTFILE = 6,
	NODETYPE_HEREDOC = 7,
	NODETYPE_APPEND = 8,
	NODETYPE_ARG = 9,
	NODETYPE_EMPTY = 10
}	t_astnodetype;

typedef struct s_ast_node t_ast_node;

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

typedef enum e_errno_custom
{
	ERRNO_COMMANDNOTFOUND = 127,
	ERRNO_BUILTIN = 258
}	t_errno_custom;

//minishell.c
void			ft_free_2d_array_nullterminated(void **array);
void			ft_destroy(void *ptr);
t_token			*ft_build_tokens(char *input);
void			local_variables(char *arguments, t_list **local);
void			del_lst_linked_env(void *content);
void			write_errormessage(const char *additional_message);

//lexer.c
t_token			*build_tokens(char *input, int *number_of_tokens);
void			free_token(t_token *token);

// t_token			*lexer(char *input, int len);
// int				needs_lex(char *arg);
// t_token			*change_arg(char **input, int i, int j, t_token *token);
// int				count_spaces(char *input);
// void			free_token(t_token *token);

//parser.c
int				built_in_check(char **arguments, t_list *my_envp);
int				non_pipe_built_in_check(int command_id, char **arguments,
					t_list *my_envp, t_list *local, t_command_table *command_table);
/*
t_command_table	parser(t_token *token);
t_token			*ft_built_in(t_token *token);
int				amount_commands(t_token *token);
*/
t_command_table	*build_command_table(t_token *token, int number_of_tokens);
void			free_command_table(t_command_table *command_table);
void			fill_command_table(t_command_table *command_table,
					t_ast_node *ast_node, int *index_simple_command,
					int *index_argument);

//ast_node.c
t_ast_node		*create_ast_node(t_astnodetype node_type);
void			free_ast_node(t_ast_node *ast_node);

//executor.c
void			execute(t_command_table *command_table, char *envp[],
					t_list *lst_env, t_list *local);

//echo.c
int				ft_echo(char **arguments);
int				option_input(char *builtin);

//built_ins1.h
int				ft_env(char *envp[]);
char			*ft_pwd(char *envp[]);
int				my_unset(char **arguments, t_list *my_envp);
int				my_env(t_list *my_envp, char **arguments);
t_list			*linked_envp(char *envp[]);
void			my_pwd(t_list *my_envp, char **arguments);
int				my_export(char **arguments, t_list **my_envp, t_list *local);
int				my_cd(char **arguments, t_list **my_envp);
void			my_exit(t_list *my_envp, char **arguments,
					t_command_table *command_table);
t_list			*check_already_there(char **arguments, t_list *my_envp, int i, int j);
char			**check_local(char **arguments, t_list *local);


//ft_split_quotes.c
char			**ft_split_quotes(const char *str, char c);

#endif
