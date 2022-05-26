/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 14:07:01 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/05/25 16:47:40 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_executor(int error_code, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	ft_lstclear(my_envp, free_content);
	ft_lstclear(local, free_content);
	free_command_table(command_table);
	exit(error_code);
}

t_fd_utils	init_fd_utils(void)
{
	t_fd_utils	fd_utils;
	int			pipe_fd[2];

	fd_utils.default_fd.in = dup(STDIN_FILENO);
	fd_utils.default_fd.out = dup(STDOUT_FILENO);
	//can fail
	pipe(pipe_fd);
	fd_utils.empty_pipe_fd.in = pipe_fd[STDIN_FILENO];
	fd_utils.empty_pipe_fd.out = pipe_fd[STDOUT_FILENO];
	close(fd_utils.empty_pipe_fd.out);
	return (fd_utils);
}

void	set_first_command_input(t_fd_utils *fd_utils)
{
	fd_utils->command_fd.in = dup(fd_utils->default_fd.in);
}

void	redirect_command_input(t_simple_command simple_command,
	t_fd_utils *fd_utils)
{
	bool	check_command_input_redirection;

	check_command_input_redirection = simple_command.fd_in != -1;
	if (simple_command.fd_failed == true)
		dup2(fd_utils->empty_pipe_fd.in, STDIN_FILENO);
	else if (check_command_input_redirection)
	{
		dup2(simple_command.fd_in, STDIN_FILENO);
		close(simple_command.fd_in);
	}
	else
		dup2(fd_utils->command_fd.in, STDIN_FILENO);
	close(fd_utils->command_fd.in);
}

void	set_last_command_output(t_simple_command simple_command,
	t_fd_utils *fd_utils)
{
	bool	check_command_output_redirection;

	check_command_output_redirection = simple_command.fd_out != -1;
	if (check_command_output_redirection)
		fd_utils->command_fd.out = simple_command.fd_out;
	else
		fd_utils->command_fd.out = dup(fd_utils->default_fd.out);
}

void	set_command_output(t_simple_command simple_command,
	t_fd_utils *fd_utils, int pipe_fd_out)
{
	bool	check_command_output_redirection;

	check_command_output_redirection = simple_command.fd_out != -1;
	if (check_command_output_redirection)
	{
		close(pipe_fd_out);
		fd_utils->command_fd.out = simple_command.fd_out;
	}
	else
		fd_utils->command_fd.out = pipe_fd_out;
}

void	redirect_command_output(t_fd_utils *fd_utils)
{
	dup2(fd_utils->command_fd.out, STDOUT_FILENO);
	close(fd_utils->command_fd.out);
}

void	redirect_command_fd(int command_id, t_simple_command simple_command,
	t_fd_utils *fd_utils, int number_of_commands)
{
	bool	check_last_command;
	int		pipe_fd[2];

	redirect_command_input(simple_command, fd_utils);
	check_last_command = command_id == number_of_commands - 1;
	if (check_last_command)
		set_last_command_output(simple_command, fd_utils);
	else
	{
		//can fail
		pipe(pipe_fd);
		fd_utils->command_fd.in = pipe_fd[STDIN_FILENO];
		set_command_output(simple_command, fd_utils, pipe_fd[STDOUT_FILENO]);
	}
	redirect_command_output(fd_utils);
}

//change handle_empty_command
bool	handle_empty_command(int command_id, t_command_table *command_table,
	t_fd_pair default_fd)
{
	bool	check_command_not_empty;
	bool	check_end_of_pipe;

	check_command_not_empty = command_table->simple_command[
			command_id].arguments[INDEX_COMMANDNAME] != NULL;
	if (check_command_not_empty)
		return (false);
	if (command_table->simple_command[command_id].fd_failed == true)
		errno = EXIT_FAILURE;
	else
	{
		check_end_of_pipe = command_id != 0 &&
			command_id == command_table->number - 1 
			&& command_table->simple_command[command_id].fd_in == -1
			&& command_table->simple_command[command_id].fd_out == -1;
		if (check_end_of_pipe)
		{
			write(default_fd.out, "lazy fuck\n", 10);
			return (true);
		}
		else
			errno = EXIT_SUCCESS;
	}
	return (true);
}

char	**duplicate_arguments(t_simple_command simple_command)
{
	char	**dup_arguments;
	int		i;

	dup_arguments = (char **) malloc((simple_command.number + 1) * sizeof(char *));
	if (dup_arguments == NULL)
		return (NULL);
	i = 0;
	while (i < simple_command.number)
	{
		dup_arguments[i] = ft_strdup(simple_command.arguments[i]);
		if (dup_arguments[i] == NULL)
		{
			ft_free_2d_array_nullterminated((void **) dup_arguments);
			return (NULL);
		}
		i++;
	}
	dup_arguments[i] = NULL;
	return (dup_arguments);
}

bool	check_got_programpath(char *program_name)
{
	bool	check_is_path;
	bool	check_is_executable;

	//is this enough to check if it is a path???
	check_is_path = ft_strrchr(program_name, '/');
	if (check_is_path)
	{
		check_is_executable = access(program_name, X_OK) == 0;
		if (check_is_executable == false)
		{
			write_errormessage(program_name);
			errno = ERRNO_COMMANDNOTFOUND;
		}
		return (check_is_executable);
	}
	check_is_executable = ft_strncmp(program_name, "make", 5) == 0;
	if (check_is_executable == false)
	{
		errno = ERRNO_COMMANDNOTFOUND;
		write_errormessage(program_name);
	}
	return (check_is_executable);
}

char	*search_env_path(t_list *my_envp)
{
	char	*env_path_with_identifier;
	bool	check_found_path;

	while (my_envp)
	{
		check_found_path = ft_strncmp((char *) my_envp->content,
			PATH_IDENTIFIER, ft_strlen(PATH_IDENTIFIER)) == 0;
		if (check_found_path)
		{
			env_path_with_identifier = ft_strdup((char *) my_envp->content);
			return (env_path_with_identifier);
		}
		my_envp = my_envp->next;
	}
	errno = ERRNO_PATHNOTFOUND;
	return (NULL);
}

char	*subtract_path_identifier(char *env_path_with_identifier)
{
	char	*path_without_identifier;
	size_t	size_path_without_identifier;
	size_t	size_identifier_of_path;

	size_identifier_of_path = ft_strlen(PATH_IDENTIFIER);
	size_path_without_identifier = ft_strlen(env_path_with_identifier)
		- ft_strlen(PATH_IDENTIFIER);
	path_without_identifier = ft_substr(env_path_with_identifier,
		size_identifier_of_path, size_path_without_identifier);
	return (path_without_identifier);
}

char	**extract_path_from_envp(t_list *my_envp)
{
	char	*env_path_with_identifier;
	char	*env_path;
	char	**env_path_splitted;

	env_path_with_identifier = search_env_path(my_envp);
	if (env_path_with_identifier == NULL)
		return (NULL);
	env_path = subtract_path_identifier(env_path_with_identifier);
	if (!env_path)
		return (NULL);
	env_path_splitted = ft_split(env_path, PATH_DELIMITER);
	free(env_path);
	return (env_path_splitted);
}

char	*put_together_binary(char *env_path_instance, char *command_name)
{
	char	*directory;
	char	*binary_path;

	directory = ft_strjoin(env_path_instance, "/");
	if (!directory)
		return (NULL);
	binary_path = ft_strjoin(directory, command_name);
	free(directory);
	return (binary_path);
}

char	*search_command_binary(char *command_name, char **env_path)
{
	char	*binary_path;
	bool	check_binary_exists;
	int		i;

	i = 0;
	while (env_path[i])
	{
		binary_path = put_together_binary(env_path[i], command_name);
		if (binary_path == NULL)
			return (NULL);
		check_binary_exists = access(binary_path, X_OK) == 0;
		if (check_binary_exists)
			return (binary_path);
		free(binary_path);
		i++;
	}
	return (NULL);
}

char	*create_programpath(char *program_name, t_list *my_envp)
{
	char	*program_path;
	char	**env_path;
	bool	check_failed_because_malloc_error;

	env_path = extract_path_from_envp(my_envp);
	if (env_path == NULL)
		return (NULL);
	program_path = search_command_binary(program_name, env_path);
	ft_free_2d_array_nullterminated((void **) env_path);
	check_failed_because_malloc_error = errno == ENOMEM;
	if (check_failed_because_malloc_error)
	{
		write_errormessage(NULL);
		return (NULL);
	}
	if (program_path != NULL)
		return (program_path);
	if (check_got_programpath(program_name))
	{
		program_path = ft_strdup(program_name);
		if (program_path == NULL)
			write_errormessage(NULL);
		return (program_path);
	}
	return (NULL);
}

char	**create_env_array(t_list *my_envp)
{
	char	**env;
	int		size_envp;
	int		i;

	size_envp = ft_lstsize(my_envp);
	env = (char **) malloc((size_envp + 1) * sizeof(char *));
	if (env == NULL)
		return (NULL);
	i = 0;
	while (i < size_envp)
	{
		env[i] = ft_strdup((char *) my_envp->content);
		if (env[i] == NULL)
		{
			ft_free_2d_array_nullterminated((void **) env);
			return (NULL);
		}
		my_envp = my_envp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

void	execute_simple_command(int command_id, t_command_table *command_table,
	t_list **my_envp, t_list **local)
{
	char				**arguments;
	char				*program_path;
	char				**env;

	arguments = duplicate_arguments(command_table->simple_command[command_id]);
	if (arguments == NULL)
		exit_executor(errno, command_table, my_envp, local);
	if (built_in_check(arguments, *my_envp) == 1)								//what happens if fails???
	{
		ft_free_2d_array_nullterminated((void **) arguments);
		exit_executor(errno, command_table, my_envp, local);
	}
	program_path = create_programpath(arguments[INDEX_COMMANDNAME], *my_envp);
	if (program_path == NULL)
	{
		ft_free_2d_array_nullterminated((void **) arguments);
		exit_executor(errno, command_table, my_envp, local);
	}
	env = create_env_array(*my_envp);
	if (env == NULL)
	{
		ft_free_2d_array_nullterminated((void **) arguments);
		free(program_path);
		exit_executor(errno, command_table, my_envp, local);
	}
	ft_lstclear(my_envp, free_content);
	ft_lstclear(local, free_content);
	free_command_table(command_table);
	execve(program_path, arguments, env);
	free(program_path);
	ft_free_2d_array_nullterminated((void **) env);
	ft_free_2d_array_nullterminated((void **) arguments);
	write_errormessage(NULL);
	exit(errno);
}

int	execute_commands(t_command_table *command_table, t_fd_utils *fd_utils,
	t_list **my_envp, t_list **local)
{
	int					process_id;
	int					command_id;
	t_simple_command	simple_command;

	command_id = 0;
	while (command_id < command_table->number)
	{
		simple_command = command_table->simple_command[command_id];
		redirect_command_fd(command_id, simple_command, fd_utils,
			command_table->number);
		//does these have an output???, if not then first check for built ins and then redirection the file descriptors
		if (non_pipe_built_in_check(command_id, command_table, my_envp,
				local) == 1)
			return (LASTPROCESSID_NOCHILDPROCESS); //what happens if builtin fails???
		process_id = fork();
		if (process_id == -1)
		{
			write_errormessage(NULL);								//should the whole program close if not able to fork???
			exit_executor(errno, command_table, my_envp, local);	//we are in the main process, since fork failed!!!
		}
		if (process_id == 0)
		{
			errno = 0;															//child process sets errno to 0!
			//change this!!!
			//change handle_empty_command
			if (handle_empty_command(command_id, command_table, fd_utils->default_fd) == false)
			{
				execute_simple_command(command_id, command_table, my_envp,
					local);
			}
			//only handle_empty_command need this!
			exit_executor(errno, command_table, my_envp, local);
		}
		command_id++;
	}
	return (process_id);
}

void	restore_default_fds(t_fd_pair default_fd)
{
	dup2(default_fd.in, STDIN_FILENO);
	dup2(default_fd.out, STDOUT_FILENO);
	close(default_fd.in);
	close(default_fd.out);
}

void	execute(t_command_table *command_table, t_list **my_envp, t_list **local)
{
	t_fd_utils	fd_utils;
	int			last_process_id;
	int			status_code;
	bool		check_only_one_command_with_failed_fd;

	check_only_one_command_with_failed_fd = command_table->number == 1
			&& command_table->simple_command[0].fd_failed;
	if (check_only_one_command_with_failed_fd)
	{
		errno = 1;
		return ;
	}
	fd_utils = init_fd_utils();
	set_first_command_input(&fd_utils);
	last_process_id = execute_commands(command_table, &fd_utils, my_envp, local);
	restore_default_fds(fd_utils.default_fd);
	if (last_process_id != LASTPROCESSID_NOCHILDPROCESS)
	{
		waitpid(last_process_id, &status_code, 0);		//can this fail?
		errno = WEXITSTATUS(status_code);				//do a check before using WEXITSTATUS?
	}
}
