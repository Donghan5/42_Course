/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:39:16 by donghank          #+#    #+#             */
/*   Updated: 2024/03/27 09:35:56 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int	i;
	int	*result;

	i = 0;
	if (min >= max)
		return (0);
	result = (int *)malloc(sizeof(int) * (max - min));
	if (!(result))
		return (0);
	while (min < max)
	{
		result[i] = min;
		i++;
		min++;
	}
	return (result);
}
