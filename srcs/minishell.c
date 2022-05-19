/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:52:13 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/17 20:37:58 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Behandeln der Quotes mit ft_strtrim, waehrend des Einlesens der Variables
// Was am einfachsten ist, wenn wir schon durch den Parser durch sind
// Achtung! Qutoes und Dollarzeichen muessen auch in der Mitte des Inputs funktionieren, nicht nur am Anfang!!!
// Aber innerhalb DQ ist SQ egal!!!!
// Deswegen geht durchsuchen leider nicht
// Aber dann funktioniert trim nicht......
// Ich koennte &arguments[i][j] trimmen, jedes Mal.......

//was will ich denn zurueckgeben? Eigentlich muss ich j uebergeben?
//mehrere $ hitereinander werden beachtet - selbst wenn dq nicht an ist, kann ich nicht bis ans Ende gehen...
char	*get_variable_value(char *argument, t_list my_envp, bool *sq, bool *dq, int j)
{
	char	*arg_var;
	char	*var;
	int		i;

	i = 0;
	if (*sq == true)
		return (argument);
	//Wie groß kann arg_var sein? Variable kann länger sein als Varname
	// Mit strjoin arbeiten? 
	arg_var = malloc(ft_strlen(arg_var));
	if (!arg_var)
		return (NULL);
	while (i < j)
	{
		arg_var[i] = argument[i];
		i++;
	}
	var = malloc(ft_strlen(&argument[i]) + 1);
	if (!var)
		return (NULL);
	if (*dq == true)
	{

		while (argument[i] != "\"")
		{
			var[i - j] = argument[i];
			i++;
		}
	}
	else
	{

	}
	var = ;//Hierein die Varaible kopieren (ft_strdup?), wie ist das mit ""?
	//Durchsuche my_envp fuer die Variable
	//Fuege Variablenwert in arg_var ein
	free(var);
	// falls noch mehr in argument steht, hier in arg_var kopieren
	arg_var[i] = '\0';
	free(argument);
	return (arg_var);
}

char	*delete_quote(char *argument, int j)
{
	int		i;
	char	*copy;

	i = 0;
	copy = malloc(ft_strlen(argument));
	if (!copy)
		return (NULL);
	while (argument[i] != '\0')
	{
		if (i != j)
			copy[i] = argument[i];
		i++;
	}
	copy[i] = '\0';
	free(argument);
	return (copy);
}

char	**get_quotes_changed(char **arguments, t_list my_envp)
{
	int		i;
	int		j;
	bool	*dq;
	bool	*sq;

	i = 0;
	j = 0;
	*dq = false;
	*sq = false;
	while (arguments[i] != NULL)
	{
		while (arguments[i][j] != '\0')
		{
			if (arguments[i][j] == '\'' && *dq == false)
			{
				quote_checker(arguments[i][j], dq, sq);
				arguments[i] = delete_quote(arguments[i], j);
			}
			else if (arguments[i][j] == '\"' && *sq == false)
			{
				arguments[i] = delete_quote(arguments[i], j);
				quote_checker(arguments[i][j], dq, sq);
			}
			else if (arguments[i][j] == "$" && *sq == false)
				arguments[i] = get_variable_value(&(arguments[i][j]), my_envp, sq, dq, j);
			j++;
		}
		i++;
	}
}

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
	else if (errno == ERRNO_BUILTIN)
	{
		write(STDERR_FILENO, "exit\n", 5);
		write(STDERR_FILENO, ERRORMESSAGE_EXIT, ft_strlen(ERRORMESSAGE_EXIT));
	}
	else
		write(STDERR_FILENO, "Unknown error: ", errno);
	write(STDERR_FILENO, "\n", 1);
}

void	write_errormessage(const char *additional_message)
{
	bool	check_custom_error;

	check_custom_error = errno == ERRNO_COMMANDNOTFOUND
		|| errno == ERRNO_BUILTIN;
	if (check_custom_error)
		write_custom_errormessage(additional_message);
	else
		perror(additional_message);
}

//just temporary here
//or rebuild ft_lstclear so that it doesn't uses ft_delone
void	del_lst_linked_env(void *content)
{
	char	*env_variable;

	//better way to doing noting???
	env_variable = (char *) content;
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

// exit sollte auf Dauer my_exit aufrufen
// Verhindert jetzt Ausgabe von ^C
void	sig_handler(int signal)
{
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
		exit(1);
	}
}

int	prompt_function(int argc, char *argv[], char *envp[])
{
	char *input;
	t_token *token;
	t_list	*local;
	//char			len;
	t_command_table	*command_table;
	//just temporary
	int				number_of_tokens;

	while (1)
	{
		write(1, "my_minishell >> ", 17);
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		input = get_next_line(0);
		//exit(0);
		if (*envp == 0x0)
		{
			errno = 1;
			perror("Please specify an envp");
			exit(EXIT_FAILURE);
		}
		//len = count_spaces(input);
		// printf("%s %d\n", input, len);
		token = build_tokens(input, &number_of_tokens);
		free(input);
		int i = 0;
		while (token[i].type != 8)
		{
			if (token[i].type == 6)
				printf("%i %i %s \n", i, token[i].type, token[i].string_value);
			else
				printf("%i %u \n", i, token[i].type);
			i++;
		}
		printf("%i %u\n", i, token[i].type);
		// commands = parser(token);
		// i = 0;
		// int j = 0;
		// while (i < commands.number)
		// {
		// 	while (j < commands.simple_command[i].number)
		// 	{
		// 		printf("%i %i %s\n", i, j, commands.simple_command[i].arguments[j]);
		// 		j++;
		// 	}
		// 	j = 0;
		// 	i++;
		// }
		char *arguments[4] = {"f=4", "r=3"};
		i = 0;
		while (arguments[i] != NULL)
		{
			local_variables(arguments[i], &local);
			i++;
		}
		command_table = build_command_table(token, number_of_tokens);
		free_token(token);
		t_list *my_envp = linked_envp(envp);
		execute(command_table, envp, my_envp, local);
		ft_lstclear(&my_envp, &del_lst_linked_env);
		free_command_table(command_table);
		// free(my_envp);
		// printf("%p", arguments);
		// system("leaks minishell");
		//printf("%s", local->content);
		//printf("%s", local->next->content);
		//built_in_check(arguments, my_envp, local);
		//ft_putstr_fd("\n\n\n\n\n\n\n", 1);
		//char *arguments2[4] = {"export", "f"};
		//char *arguments3[4] = {"env"};
		//built_in_check(arguments2, my_envp, local);
		//my_env(my_envp, arguments3);
		i = argc;
		i = ft_strlen(argv[0]);
		//my_exit(&token->string_value);
	}
	return (0);
}

// Das Prompt_Display muesste immer wieder kommen, also irgendwie wieder aufrufen?
// Funktioniert mit while(1), ist halt nur bedingt schoen
int	main(int argc, char *argv[], char *envp[])
{
	int	i;

	prompt_function(argc, argv, envp);
	i = argc;
	i = ft_strlen(argv[0]);
	return (0);
}
