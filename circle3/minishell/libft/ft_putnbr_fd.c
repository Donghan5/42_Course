/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:19:27 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/18 20:40:01 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

static unsigned int	calc_tens(int n)
{
	if (n == 1)
		return (1);
	return (10 * calc_tens(n - 1));
}

static int	calc_length(unsigned int n_u)
{
	int	len;

	len = 0;
	while (n_u)
	{
		n_u /= 10;
		len++;
	}
	return (len);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	n_u;
	int				length;

	n_u = n;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n_u = n * (-1);
	}
	length = calc_length(n_u);
	if (!length)
		ft_putchar_fd('0', fd);
	while (length)
	{
		ft_putchar_fd(n_u / calc_tens(length) + '0', fd);
		n_u %= calc_tens(length);
		length--;
	}
}
