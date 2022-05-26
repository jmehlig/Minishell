/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:52:13 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/26 11:56:31 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	write_custom_errormessage(const char *additional_message)
{
	if (additional_message != NULL)
	{
		write(STDERR_FILENO, additional_message, ft_strlen(additional_message));
		write(STDERR_FILENO, ": ", 2);
	}
	if (errno == ERRNO_COMMANDNOTFOUND)
	{
		write(STDERR_FILENO, ERRORMESSAGE_COMMANDNOTFOUND,
			ft_strlen(ERRORMESSAGE_COMMANDNOTFOUND));
	}
	else if (errno == ERRNO_BUILTIN)											//wrong!!!
		write(STDERR_FILENO, ERRORMESSAGE_EXIT, ft_strlen(ERRORMESSAGE_EXIT));
	else if (errno == ERRNO_PATHNOTFOUND)
	{
		write(STDERR_FILENO, ERRORMESSAGE_PATHNOTFOUND,
			ft_strlen(ERRORMESSAGE_PATHNOTFOUND));
	}
	else
		write(STDERR_FILENO, "Unknown error: ", errno);
	write(STDERR_FILENO, "\n", 1);
}

void	write_errormessage(const char *additional_message)
{
	bool	check_already_wrote_errormessage;
	bool	check_custom_error;

	check_already_wrote_errormessage = errno == ERRNO_PARSER;
	if (check_already_wrote_errormessage)
		return ;
	check_custom_error = errno == ERRNO_COMMANDNOTFOUND
		|| errno == ERRNO_BUILTIN;
	if (check_custom_error)
		write_custom_errormessage(additional_message);
	else
		perror(additional_message);
}

//just temporary here
void	ft_free_lst(t_list **lst)
{
	t_list	*ptr;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		ptr = (*lst)->next;
		free(*lst);
		*lst = ptr;
	}
}

void	free_content(void *content)
{
	free(content);
}

void	ft_free_2d_array_nullterminated(void **array)
{
	size_t	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	ft_destroy(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

/*
//Wenn Eingabe nur aus Woertern besteht, das hier als Loop aufrufen, fuer locale Variablen
void	local_variables(char *arguments, t_list **local)
{
	if (ft_strchr(arguments, '='))
	{
		if (!(*local))
			*local = ft_lstnew(arguments);
		else
			ft_lstadd_back(local, ft_lstnew(arguments));
	}
}
*/

// Verhindert jetzt Ausgabe von ^C
// SIGINT -> Ctrl -C
// SIGQUIT -> Ctrl -\ */
void	sig_handler(int signal)
{
	//ft_putnbr_fd(signal, 1);
	if (signal == SIGINT)
	{
		write(1, "\b\b", 2);
		write(1, "  ", 2);
		write(1, "\b\b", 2);
		write(1, "\n", 1);
		write(1, "my_minishell >> ", 17);
	}
	if (signal == SIGQUIT)
	{
		write(1, "\b\b", 2);
		write(1, "  ", 2);
		write(1, "\b\b", 2);
	}
}

//delete, just for debugging
void	print_token(t_token *token)
{
	int	i;

	i = 0;
	while (token[i].type != 8)
	{
		if (token[i].type == 6)
			printf("%i %i %s \n", i, token[i].type, token[i].string_value);
		else
			printf("%i %u \n", i, token[i].type);
		i++;
	}
	printf("%i %u\n", i, token[i].type);
}

//delete, just for debugging
void	print_linkedlist(t_list *linked_list)
{
	printf("--------------------------------------------------\nprint linked_list\n\n");
	while (linked_list != NULL)
	{
		printf("%s\n", (char *) linked_list->content);
		linked_list = linked_list->next;
	}
	printf("--------------------------------------------------\n");
}

/*
//temporary for testing local variables
static	t_list	*init_local_variables()
{
	t_list	*local;
	char	*arguments[4] = {"f=4", "r=3"};
	int		i;

	local = NULL;
	i = 0;
	while (arguments[i] != NULL)
	{
		local_variables(arguments[i], &local);
		i++;
	}
	return (local);
}
*/

/*
what happens if we use cd, does the heredoc file change location?
*/
static	void	delete_heredoc(void)
{
	bool	check_heredoc_file_exists;

	check_heredoc_file_exists = access(FILENAME_HEREDOC, F_OK) == 0;
	if (check_heredoc_file_exists)
		unlink(FILENAME_HEREDOC);
}

//slightly modified version of ft_lstdelnode, since we have to free the content
void	ft_lstdelkey(t_list **lst, void *key, void (*del)(void *))
{
	t_list	*temp;
	t_list	*prev;

	temp = *lst;
	prev = NULL;
	if (temp != NULL && (ft_strncmp(temp->content, key, ft_strlen(temp->content)) == 0))
	{
		*lst = temp->next;
		temp->next = NULL;
		ft_lstclear(&temp, del);
	}
	else
	{
		while (temp != NULL && (ft_strncmp(temp->content, key, ft_strlen(temp->content)) != 0))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return;
		prev->next = temp->next;
		temp->next = NULL;
		ft_lstclear(&temp, del);
	}
}

// better function name?
void	process_input(char *input, t_list **my_envp, t_list **local)
{
	int				number_of_tokens;
	t_token			*token;
	t_command_table	*command_table;

	token = build_tokens(input, &number_of_tokens);
	free(input);
	//when does token building fail??? Only malloc?
	if (token == NULL)
	{
		write_errormessage(NULL);
		return ;					//stay in loop or exit program?
	}
	//delete
	print_token(token);
	command_table = build_command_table(&token, number_of_tokens);
	free_token(token);
	if (command_table != NULL)
		execute(command_table, my_envp, local);
	//delete
	printf("\n-------------------------\nended with %d\n-------------------------\n", errno);
	free_command_table(command_table);
	//delete
	//print_linkedlist(*my_envp);
	//delete
	print_linkedlist(*local);
	delete_heredoc();															//since we are using access errno is changed when here_doc doesn't exists
}

//local variables as parameter?
void	prompt_function(t_list **my_envp, t_list **local)
{
	char	*input;

	write(1, &"my_minishell >> ", 17);
	//TODO add CTRL + D
	input = get_next_line(STDIN_FILENO);
	if (input == NULL)
	{
		write_errormessage(NULL);
		return ;					//stay in loop or exit program?
	}
	process_input(input, my_envp, local);
}

// Das Prompt_Display muesste immer wieder kommen, also irgendwie wieder aufrufen?
// Funktioniert mit while(1), ist halt nur bedingt schoen
int	main(int argc, char *argv[], char *envp[])
{
	t_list	*my_envp;
	t_list	*local;
	int		i;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	//signal(SIGKILL, sig_handler);
	if (*envp == 0x0)
	{
		errno = 1;
		perror("Please specify an envp");
		exit(EXIT_FAILURE);
	}
	my_envp = linked_envp(envp);
	local = NULL;
	while (1)
	{
		prompt_function(&my_envp, &local);
	}
	i = argc;
	i = ft_strlen(argv[0]);
	return (EXIT_SUCCESS);
}
