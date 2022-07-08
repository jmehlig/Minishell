/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:52:13 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/23 09:29:56 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_list	*linked_envp(char *envp[])
{
	t_list	*lst;
	char	*str_content;
	t_list	*node;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		str_content = ft_strdup(envp[i]);
		if (str_content == NULL)
		{
			ft_lstclear(&lst, free_content);
			return (NULL);
		}
		node = ft_lstnew((void *) str_content);
		if (node == NULL)
		{
			free(str_content);
			ft_lstclear(&lst, free_content);
			return (NULL);
		}
		ft_lstadd_back(&lst, node);
		i++;
	}
	return (lst);
}

static	void	delete_heredoc(void)
{
	bool	check_heredoc_file_exists;
	int		errno_buffer;

	errno_buffer = errno;
	check_heredoc_file_exists = access(FILENAME_HEREDOC, F_OK) == 0;
	if (check_heredoc_file_exists)
		unlink(FILENAME_HEREDOC);
	errno = errno_buffer;
}

static	void	process_input(char *input, t_list **my_envp, t_list **local)
{
	int				number_of_tokens;
	t_token			*token;
	t_command_table	*command_table;

	token = build_tokens(input, &number_of_tokens);
	free(input);
	if (token == NULL)
	{
		write_errormessage(NULL);
		return ;
	}
	command_table = build_command_table(&token, number_of_tokens, my_envp,
			local);
	free_token(token);
	if (command_table == NULL)
		return ;
	expander(command_table, *my_envp, *local);
	execute(command_table, my_envp, local);
	free_command_table(&command_table);
	delete_heredoc();
}

static	void	prompt_function(t_list **my_envp, t_list **local)
{
	char	*input;
	int		errno_buffer;

	signal_handling(BEGIN);
	errno_buffer = errno;
	input = readline("my_minishell >> ");
	errno = errno_buffer;
	if (input == NULL)
	{
		rl_replace_line("my_minishell >> exit", 0);
		rl_redisplay();
		write(1, "exit", 4);
		ft_lstclear(my_envp, free_content);
		ft_lstclear(local, free_content);
		rl_clear_history();
		exit(0);
	}
	if (input != NULL && input[0] != '\0')
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		add_history(input);
		process_input(input, my_envp, local);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_list	*my_envp;
	t_list	*local;
	int		ignore;

	if (argc != 1 || *envp == 0x0)
	{
		ft_putendl_fd("Too many parameters or couldn't find environment", 2);
		exit(EXIT_FAILURE);
	}
	my_envp = linked_envp(envp);
	if (my_envp == NULL)
	{
		write_errormessage(NULL);
		exit(EXIT_FAILURE);
	}
	local = NULL;
	while (1)
		prompt_function(&my_envp, &local);
	ignore = ft_strlen(argv[0]);
	return (EXIT_SUCCESS);
}
