/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 14:07:01 by hkalyonc          #+#    #+#             */
/*   Updated: 2022/05/17 15:20:35 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_executor(int error_code, t_command_table *command_table,
	t_list *my_envp)
{
	ft_lstclear(&my_envp, &del_lst_linked_env);
	free_command_table(command_table);
	exit(error_code);
}

void	save_default_fds(t_fd_pair *default_fd)
{
	default_fd->in = dup(STDIN_FILENO);
	default_fd->out = dup(STDOUT_FILENO);
}

//just temporary, currently behaves like infile
int	get_here_doc(char *here_doc_delimiter)
{
	return (open(here_doc_delimiter, O_RDONLY));
}

bool	set_first_command_input(t_fd_pair *command_fd, t_fd_pair default_fd,
	t_command_table *command_table)
{
	if (command_table->infile)
	{
		command_fd->in = open(command_table->infile, O_RDONLY);
		if (command_fd->in == -1)
		{
			write_errormessage(command_table->infile);
			return (false);
		}
	}
	else if (command_table->here_doc)
		command_fd->in = get_here_doc(command_table->here_doc);
	else
		command_fd->in = dup(default_fd.in);
	return (true);
}
void	redirect_command_input(int command_fd_in)
{
	dup2(command_fd_in, STDIN_FILENO);
	close(command_fd_in);
}

void	set_last_command_output(t_fd_pair *command_fd, t_fd_pair default_fd,
	t_command_table *command_table)
{
	if (command_table->outfile)
	{
		command_fd->out = open(command_table->outfile,
			O_CREAT | O_WRONLY | O_TRUNC, MODE_OPENOUTFILE);
	}
	else if (command_table->append)
	{
		command_fd->out = open(command_table->append,
			O_CREAT | O_WRONLY | O_APPEND, MODE_OPENOUTFILE);
	}
	else
	{
		command_fd->out = dup(default_fd.out);
	}
	//command_fd.in doesn't matter anymore
}

void	redirect_command_output(int command_fd_out)
{
	dup2(command_fd_out, STDOUT_FILENO);
	close(command_fd_out);
}

void	redirect_command_fd(int command_id, t_fd_pair *command_fd,
	t_fd_pair default_fd, t_command_table *command_table)
{
	int		pipe_fd[2];
	bool	check_last_command;

	redirect_command_input(command_fd->in);
	check_last_command = command_id == command_table->number - 1;
	if (check_last_command)
		set_last_command_output(command_fd, default_fd, command_table);
	else
	{
		pipe(pipe_fd);
		command_fd->in = pipe_fd[STDIN_FILENO];
		command_fd->out = pipe_fd[STDOUT_FILENO];
	}
	redirect_command_output(command_fd->out);
}

/*
What if we have a program that has the same name as function, e.g. 'cat'.
What has priority the command 'cat' or the executable 'cat' in the current
working directory.
*/
bool	check_got_programpath(char *program_name)
{
	bool	check_is_path;
	bool	check_is_executable;

	check_is_path = ft_strrchr(program_name, '/');
	if (check_is_path)
		return (true);
	check_is_executable = access(program_name, X_OK) == 0;
	return (check_is_executable);
}

int	search_env_path(char *envp[])
{
	bool	check_found_path;
	int		i;

	i = 0;
	while (envp[i])
	{
		check_found_path = ft_strncmp(envp[i], PATH_IDENTIFIER,
			ft_strlen(PATH_IDENTIFIER)) == 0;
		if (check_found_path)
			return (i);
		i++;
	}
	return (PATH_NOTFOUND);
}

char	*subtract_path_identifier(char *path_in_envp)
{
	char	*path_without_identifier;
	size_t	size_path_without_identifier;
	size_t	size_identifier_of_path;

	size_identifier_of_path = ft_strlen(PATH_IDENTIFIER);
	size_path_without_identifier = ft_strlen(path_in_envp)
		- ft_strlen(PATH_IDENTIFIER);
	path_without_identifier = ft_substr(path_in_envp, size_identifier_of_path,
			size_path_without_identifier);
	return (path_without_identifier);
}

//rewrite this with getenv!!!
char	**extract_path_from_envp(char *envp[])
{
	char	*env_path;
	char	**env_path_splitted;
	int		env_path_index;

	env_path_index = search_env_path(envp);
	if (env_path_index == PATH_NOTFOUND)
	{
		//other way to deal with errormessages, can i set a errno???
		perror(ERRORMESSAGE_NOPATH);
		return (NULL);
	}
	env_path = subtract_path_identifier(envp[env_path_index]);
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

char	*create_programpath(char *program_name, char *envp[])
{
	char	*program_path;
	char	**env_path;
	bool	check_could_not_find_binary;

	if (check_got_programpath(program_name))
	{
		program_path = ft_strdup(program_name);
		return (program_path);
	}
	env_path = extract_path_from_envp(envp);
	if (env_path == NULL)
		return (NULL);
	program_path = search_command_binary(program_name, env_path);
	check_could_not_find_binary = program_path == NULL && errno < sys_nerr;
	if (check_could_not_find_binary)
	{
		errno = ERRNO_COMMANDNOTFOUND;
		write_errormessage(program_name);
	}
	ft_free_2d_array_nullterminated((void **) env_path);
	return (program_path);
}

// Jetzt habe ich hier ein Problem, da ich gerne die t_list mit den lokalen Variablen in Zeile 228 uebergeben wuerde
// dann aber zu viele Varaiablen uebergeben werden
int	execute_commands(t_command_table *command_table, t_fd_pair default_fd,
	char *envp[], t_list *my_envp, t_list *local)
{
	t_fd_pair	command_fd;
	int			process_id;
	char		*program_path;
	int			command_id;
	char		**command_arguments;

	if (!set_first_command_input(&command_fd, default_fd, command_table))
		return (LASTPROCESSID_NOCHILDPROCESS);
	command_id = 0;
	while (command_id < command_table->number)
	{
		command_arguments = command_table->simple_command[command_id].arguments;
		redirect_command_fd(command_id, &command_fd, default_fd, command_table);
		if (non_pipe_built_in_check(command_id, command_arguments, my_envp,
				local, command_table) == 1)
			return (LASTPROCESSID_NOCHILDPROCESS); //what happens if builtin fails???
		process_id = fork();
		if (process_id == -1)
		{
			//should the whole program close if not able to fork???
			write_errormessage(NULL);
			exit_executor(errno, command_table, my_envp);
		}
		if (process_id == 0)
		{
			if (built_in_check(command_arguments, my_envp) == 1)
				exit_executor(EXIT_SUCCESS, command_table, my_envp); //what happens if a builtin fails???
			program_path = create_programpath(
				command_arguments[INDEX_COMMANDNAME], envp);
			if (program_path == NULL)
				exit_executor(errno, command_table, my_envp);
			execve(program_path, command_arguments, envp);
			write_errormessage(NULL);
			exit_executor(errno, command_table, my_envp);
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

void	execute(t_command_table *command_table, char *envp[], t_list *lst_env, t_list *local)
{
	t_fd_pair	default_fd;
	int			last_process_id;
	int			status_code;

	save_default_fds(&default_fd);
	last_process_id = execute_commands(command_table, default_fd, envp, lst_env, local);
	restore_default_fds(default_fd);
	if (last_process_id != LASTPROCESSID_NOCHILDPROCESS)
	{
		waitpid(last_process_id, &status_code, 0);		//can this fail?
		errno = WEXITSTATUS(status_code);				//do a check before using WEXITSTATUS?
	}
	//delete
	printf("-------------------------\nended with %d\n-------------------------\n", errno);
}
