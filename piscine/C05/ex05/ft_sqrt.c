/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 09:20:46 by donghank          #+#    #+#             */
/*   Updated: 2024/03/27 09:05:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_sqrt(int nb)
{
	int	sqrt;

	sqrt = 1;
	if (nb > 0)
	{
		while ((sqrt * sqrt) <= nb)
		{
			if ((sqrt * sqrt) == nb)
			{
				return (sqrt);
			}
			if (sqrt > 46360)
			{
				return (0);
			}
			sqrt++;
		}
	}
	return (0);
}
