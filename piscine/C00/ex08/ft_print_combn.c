/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_combn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 08:49:00 by donghank          #+#    #+#             */
/*   Updated: 2024/03/17 16:47:19 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char s)
{
	write(1, &s, 1);
}

void	print_combination(int n, int start, int index, int *copy)
{
	int	i;

	if (index == n)
	{
		i = 0;
		while (i < n)
		{
			ft_putchar(copy[i] + '0');
			i++;
		}
		if (copy[0] != 9 - n + 1)
		{
			write(1, ", ", 2);
		}
		return ;
	}
	i = start;
	while (i <= 9)
	{
		copy[index] = i;
		print_combination(n, i + 1, index + 1, copy);
		i++;
	}
}

void	ft_print_combn(int n)
{
	int	copy[10];

	print_combination(n, 0, 0, copy);
}
