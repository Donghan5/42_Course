/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 19:35:51 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 17:00:00 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	calc_length(int n)
{
	int				length;
	unsigned int	n_u;

	length = 0;
	n_u = n;
	if (n < 0)
	{
		n_u = n * (-1);
		length++;
	}
	if (!n_u)
		return (1);
	while (n_u)
	{
		n_u /= 10;
		length++;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	char			*num;
	int				length;
	unsigned int	n_u;

	length = calc_length(n);
	num = malloc(length + 1);
	if (!num)
		return (NULL);
	n_u = n;
	if (n < 0)
	{
		num[0] = '-';
		n_u = n * (-1);
	}
	num[length] = '\0';
	if (!n_u)
		num[0] = '0';
	while (n_u)
	{
		num[length - 1] = n_u % 10 + '0';
		length--;
		n_u /= 10;
	}
	return (num);
}
