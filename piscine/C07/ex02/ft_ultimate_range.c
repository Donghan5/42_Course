/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:56:00 by donghank          #+#    #+#             */
/*   Updated: 2024/03/29 08:06:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	int	*ptr;
	int	i;
	int	size;

	if (min >= max)
	{
		*range = 0;
		return (0);
	}
	size = max - min;
	ptr = (int *)malloc(sizeof(int) * size);
	if (!(ptr))
	{
		*range = 0;
		return (-1);
	}
	*range = ptr;
	i = 0;
	while (i < size)
	{
		ptr[i] = min + i;
		i++;
	}
	return (size);
}
