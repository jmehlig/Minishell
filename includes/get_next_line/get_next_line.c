/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:45:11 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 14:54:09 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_new_line(char *readbuffer)
{
	char	*new_line;
	int		i;

	i = 0;
	while (readbuffer[i] != '\n' && readbuffer[i] != '\0')
		i++;
	new_line = malloc(i + 1);
	if (!new_line)
		return (NULL);
	i = 0;
	while (readbuffer[i] != '\n' && readbuffer[i] != '\0')
	{
		new_line[i] = readbuffer[i];
		i++;
	}
	new_line[i] = '\0';
	return (new_line);
}

static char	*rearrenge_buffer(char *readbuffer, int len)
{
	char	*new_readbuffer;
	char	*temp;

	new_readbuffer = malloc(ft_strlen_gnl(&readbuffer[len + 1]) + 1);
	temp = new_readbuffer;
	if (!new_readbuffer)
		return (NULL);
	while (readbuffer[len + 1] != '\0')
	{
		*temp = readbuffer[len + 1];
		temp++;
		len++;
	}
	*temp = '\0';
	free(readbuffer);
	return (new_readbuffer);
}

static char	*print_line(char *line)
{
	static char	*readbuffer;
	char		*result;

	if (!readbuffer)
	{
		readbuffer = malloc(1);
		*readbuffer = '\0';
	}
	if (!readbuffer)
		return (NULL);
	if (ft_strlen_gnl(line) > 0)
		readbuffer = ft_strjoin_gnl(readbuffer, line);
	free(line);
	if (ft_strlen_gnl(readbuffer) == 0)
		return (get_result(&readbuffer, (char *) NULL));
	result = ft_new_line(readbuffer);
	if (readbuffer[ft_strlen_gnl(result)] == '\n')
	{
		readbuffer = rearrenge_buffer(readbuffer, ft_strlen_gnl(result));
		return (result);
	}
	else
		return (get_result(&readbuffer, result));
}

char	*read_next_line(int fd, char *line, char *buffer, int bytesread)
{
	while (bytesread > 0)
	{
		buffer[bytesread] = '\0';
		if (!ft_strchr_gnl(buffer, '\n'))
		{
			line = ft_strjoin_gnl(line, buffer);
			bytesread = read(fd, buffer, BUFFER_SIZE);
		}
		else
		{
			line = ft_strjoin_gnl(line, buffer);
			free(buffer);
			return (print_line(line));
		}
	}
	free(buffer);
	return (print_line(line));
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	int			bytesread;

	buffer = malloc(BUFFER_SIZE + 1);
	line = malloc(1);
	*line = '\0';
	bytesread = read(fd, buffer, BUFFER_SIZE);
	return (read_next_line(fd, line, buffer, bytesread));
}
