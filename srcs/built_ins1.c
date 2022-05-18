/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 16:06:15 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/17 14:54:56 by hkalyonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// unset should be fine
// look up special cases export

// Bescheuert aber wahr: env t=12, also env plus gueltige Variable, lasse env anders drucken, aber veraendere Wert nicht, also kein export...
// Ok hab's, mit linked list kopieren, waere unnoetig gewesen, vielleicht loeschen, aber was soll's, ausser es leaked

//Generell erfolgt Zugriff auf Variablen ueber $(NAME) ueber die selber erstellte envp t_list
//Kann eine 'Variable' noch mehr?

//Eigene env erstellen, mit linked list, die kann dann veraendert werden
//....export test -> export -> env
//Eventuell diese Umgebung als globale Variable? Da Zugriff von Namen hierauf erfolgen muss...

// local variables bearbeitet

//cd leaked immer noch....
// cd .??
// cd muss noch anders werden??

//unset — unset values and attributes of variables and functions
//Each variable or function specified by name shall be unset.
// Was genau heisst loeschen? free scheint nicht auszureichen, und momentan ist es nur free, und entfernen aus der Liste
// Kann man mehr loeschen als das, was mit env angezeigt wird? Wo wird danach gesucht?
// reicht das fuer Variablen?
// [unset] [NAME]
int	my_unset(char **arguments, t_list *my_envp)
{
	t_list	*temp;
	char	*join;

	temp = my_envp;
	while (temp->next)
	{
		join = ft_strjoin(arguments[1], "=");
		if (ft_strncmp(temp->content, join, ft_strlen(arguments[1]) + 1) == 0)
		{
			ft_lstdelnode(&my_envp, temp->content);
		}
		free(join);
		temp = temp->next;
	}
	ft_lstfree(&my_envp);
	return (0);
}

t_list	*check_already_there(char **arguments, t_list *my_envp, int i, int j)
{
	t_list	*temp;
	char	*var_name;

	temp = my_envp;
	var_name = ft_substr(arguments[j], 0, i);
	while (temp->next)
	{
		if (ft_strncmp(temp->content, var_name, ft_strlen(var_name)) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

char	**check_local(char **arguments, t_list *local)
{
	int 	i;

	while (local != 0 && local->next)
	{
		i = 1;
		while (arguments[i] != NULL)
		{
			if (ft_strncmp(arguments[i], local->content, ft_strlen(arguments[i])) == 0)
			{
				arguments[i] = ft_strdup(local->content);
				ft_lstdelnode(&local, local->content);
			}
			i++;
		}
	}
	return (arguments);
}

// OK, wo auch immer ich hier die Varaible speichere, da muss ich sie mit unset loeschen
//Einfuehren von Umgebungsvariablen
//Fehlermeldung fuer kein = enthalten? -> bash tut das nicht
//Mehrere gleichzeitig geht, ausserdem bei keinem Zusatz env
int	my_export(char **arguments, t_list **my_envp, t_list *local)
{
	t_list	*new;
	int		i;
	t_list	*there;
	int		j;

	i = 0;
	j = 1;
	arguments = check_local(arguments, local);
	while (arguments[j] != NULL)
	{
		while (arguments[j][i] != '\0')
		{
			if (arguments[j][i] == '=')
				break;
			i++;
			if (arguments[j][i] == '\0')
				return (0);
		}
		there = check_already_there(arguments, *my_envp, i, j);
		if (there != NULL)
			there->content = ft_strjoin(ft_substr(there->content, 0, i), ft_substr(arguments[j], i, ft_strlen(arguments[j])));
		else
		{
			new = ft_lstnew(arguments[j]);
			ft_lstadd_back(my_envp, new);
		}
		j++;
	}
	if (arguments[1] == NULL)
		my_env(*my_envp, arguments);
	return (0);
}

char	*get_pwd_path(char *arguments, char *pwd)
{
	char	*temp;
	char	*temp2;
	char	*ptr;
	int		i;

	if (arguments[0] != '.')
		return(arguments);
	else
	{
		if (arguments[1] == '/' && ft_strchr(&arguments[2], '/') != NULL)
		{
			i = ft_strchr(&arguments[2], '/') -  &arguments[2];
			temp = ft_substr(&arguments[2], 0, i);
			//printf("%s %i\n", &arguments[2], i);
			//printf("%s\n", temp);
			//temp2 = ft_strnstr(pwd, temp, ft_strlen(pwd));
			i = ft_strnstr(pwd, temp, ft_strlen(pwd)) - pwd;
			temp2 = ft_substr(pwd, 4, i - 4);
			//printf("%s\n", temp2);
			free(temp);
			ptr = ft_strjoin(temp2, &arguments[2]);
			free (temp2);
			return (ptr);
		}
		if (ft_strncmp(arguments, "..", 3) == 0)
		{
			i = ft_strrchr(pwd, '/') - pwd;
			return(ft_substr(pwd, 4, i - 4));
		}
	}
	errno = ENOENT;
	perror("cd to path failed");
	exit(1);
}

//Sollte pwd benutzen?
// Muss hier noch mehr geaendert werden?
//chdir - macht was genau? Wie einsehbar?
// path kann arguments[1] sein, wenn absolute path
int	my_cd(char **arguments, t_list **my_envp)
{
	char	*path;
	char	*oldpath;
	t_list	*temp;

	temp = *my_envp;
	if (arguments[2] != NULL)
	{
		perror("cd");
		exit(1);
	}
	while(temp->next)
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
		{
			path = get_pwd_path(arguments[1], temp->content);
			if (chdir(path) != 0)
			{
				perror("chdir() to path failed");
				free(path);
				exit(1);
			}
			oldpath = malloc(ft_strlen(temp->content));
			oldpath = ft_substr(temp->content, 4, ft_strlen(temp->content));
			temp->content = ft_strjoin("PWD=", path);
			temp->next->content = ft_strjoin("OLDPWD=", oldpath);
			free(path);
			free(oldpath);
			return (0);
		}
		temp = temp->next;
	}
	return (0);
}

// char	*ft_pwd(char *envp[])
// {
// 	char	*path;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while(envp[i] != NULL)
// 	{
// 		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
// 		{
// 			path = malloc(ft_strlen(envp[i]) + 1);
// 			if (!path)
// 				return (NULL);
// 			while(envp[i][j + 4] != '\0')
// 			{
// 				path[j] = envp[i][j + 4];
// 				j++;
// 			}
// 			path[j] = '\0';
// 			return (path);
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }

//Diese Variablen sind alle schon als Umgebungsvariablen angelegt
// Sollte ich allerdings darin etwas veraendern, wie lege ich die Variable auch als Variable an?
t_list	*linked_envp(char *envp[])
{
	t_list	*lst;
	int		i;

	i = 1;
	lst = ft_lstnew(envp[0]);
	while (envp[i] != NULL)
	{
		ft_lstadd_back(&lst, ft_lstnew(envp[i]));
		i++;
	}
	return (lst);
}

t_list	*copy_list(t_list *my_envp)
{
	t_list *copy;

	copy = ft_lstnew(my_envp->content);
	while(my_envp->next)
	{
		ft_lstadd_back(&copy, ft_lstnew(my_envp->content));
		my_envp = my_envp->next;
	}
	return (copy);
}

// void	print_not_export(t_list *my_envp, char **arguments)
// {
// 	t_list	*copy;
// 	int		i;
// 	char	*temp_arg[2];

// 	copy = my_envp;
// 	i = 1;
// 	while (arguments[i] != NULL)
// 	{
// 		if (ft_strchr(arguments[i], '=') == NULL)
// 		{
// 			errno = ENOENT;
// 			perror("env:");
// 			exit(1);
// 		}
// 		i++;
// 	}
// 	copy = copy_list(my_envp);
// 	my_export(arguments, copy);
// 	temp_arg[0] = "export";
// 	my_env(copy, temp_arg);
// 	//ft_lstfree(copy);
// }

int	my_env(t_list *my_envp, char **arguments)
{
	if (!my_envp)
		return (0);
	if (arguments[1] != NULL)
	{
		errno = 7;
		perror("pwd:");
		exit(0);
	}
	// if (arguments[1] != NULL)
	// 	print_not_export(my_envp, arguments);
	while (my_envp->next)
	{
		ft_putstr_fd(my_envp->content, 1);
		ft_putchar_fd('\n', 1);
		my_envp = my_envp->next;
	}
	ft_putstr_fd(my_envp->content, 1);
	ft_putchar_fd('\n', 1);
	return (0);
}

void	my_pwd(t_list *my_envp, char **arguments)
{
	char	*path;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (arguments[1] != NULL)
	{
		errno = 7;
		perror("pwd");
		exit(0);
	}
	while(my_envp->next)
	{
		if (ft_strncmp(my_envp->content, "PWD=", 4) == 0)
		{
			path = malloc(ft_strlen(my_envp->content) + 1);
			if (!path)
				return ;
			while(((char *)(my_envp->content))[j + 4] != '\0')
			{
				path[j] = ((char *)(my_envp->content))[j + 4];
				j++;
			}
			path[j] = '\0';
			ft_putstr_fd(path, 1);
			ft_putchar_fd('\n', 1);
			free (path);
			return ;
		}
		my_envp = my_envp->next;
	}
}

// exit schliesst die shell und gibt Rueckmeldung ueber succesful oder unsuccsesful
// noch nicht ganz richtig, erst mal Idee probieren
void	my_exit(t_list *my_envp, char **arguments,
	t_command_table *command_table)
{
	bool	check_no_arguments;
	bool	check_one_argument;
	uint8_t	exit_code;

	check_no_arguments = arguments[1] == NULL;
	check_one_argument = check_no_arguments == false && arguments[2] == NULL;
	if (check_no_arguments)
		exit_code = 0;
	if (check_one_argument)
		exit_code = (uint8_t) ft_atoi(arguments[1]);
	if (check_no_arguments == true || check_one_argument == true)
	{
		free_command_table(command_table);
		ft_lstclear(&my_envp, &del_lst_linked_env);
		exit(exit_code);
	}
	else
	{
		errno = ERRNO_BUILTIN;
		write_errormessage(NULL);
		return ;
	}
}
