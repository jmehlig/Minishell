/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:08:53 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:08:55 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	x;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		x = (unsigned int)(n * -1);
	}
	else
		x = (unsigned int)n;
	if (x >= 10)
		ft_putnbr_fd(x / 10, fd);
	ft_putchar_fd((x % 10) + 48, fd);
}
