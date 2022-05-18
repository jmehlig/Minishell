/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:10:44 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/09 13:10:45 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_lennbr(int n)
{
	int	i;

	i = 1;
	if (n >= 0)
	{
		while (n / 10 >= 1)
		{
			n /= 10;
			i++;
		}
	}
	else
	{
		while (n / 10 <= -1)
		{
			n /= 10;
			i++;
		}
	}
	return (i);
}

static char	*ft_positive(int n)
{
	int		i;
	char	*ptr;

	i = ft_lennbr(n);
	ptr = (char *)malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (0);
	ptr[i] = 0;
	while (i > 0)
	{
		ptr[i - 1] = (n % 10) + 48;
		n /= 10;
		i--;
	}
	return (ptr);
}

static char	*ft_negative(int n)
{
	int		i;
	char	*ptr;

	i = ft_lennbr(n);
	ptr = (char *)malloc(sizeof(char) * (i + 2));
	if (!ptr)
		return (0);
	ptr[i + 1] = 0;
	ptr[0] = 45;
	while (i > 0)
	{
		ptr[i] = ((n % 10) * -1) + 48;
		n /= 10;
		i--;
	}
	return (ptr);
}

char	*ft_itoa(int n)
{
	if (n >= 0)
		return (ft_positive(n));
	else
		return (ft_negative(n));
}
