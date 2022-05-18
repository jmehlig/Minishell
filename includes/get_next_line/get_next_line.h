/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 14:54:50 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/03 17:30:08 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# define MAX_FD 5050

char	*get_next_line(int fd);
// char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin_gnl(char *s1, char *s2);
size_t	ft_strlen_gnl(const char *s);
char	*ft_strdup_gnl(const char *s);
char	*ft_strchr_gnl(const char *s, int c);
void	*get_result(char **readbuffer, char *result);

#endif
