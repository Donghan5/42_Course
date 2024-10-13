/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 13:47:24 by donghank          #+#    #+#             */
/*   Updated: 2024/03/17 13:58:48 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c);

void	rush(int x, int y)
{
	int	i;
	int	j;
	
	j = 1;
	while (j <= y)
	{
		i = 1;
		while(i <= x)
		{
			if((j == 1 && i == 1) || (j == y && i == x))
				ft_putchar('/');
			else if((j == 1 && i == x) || (j == y && i == 1))
				ft_putchar("\\");
			else if((j == 1 || j == y) && (i == 1 || i == x))
				ft_putchar("*");
			else
				ft_putchar(' ');
			i++;
		}
		ft_putchar('\n');
		j++;
	}
}
