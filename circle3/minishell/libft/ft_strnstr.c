/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:39:05 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:50:09 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (*little == '\0')
		return ((char *)big);
	if (len == 0)
		return (NULL);
	i = 0;
	while (big[i] != '\0' && len)
	{
		j = 0;
		while (big[i + j] == little[j] && j < len)
		{
			j++;
			if (!little[j])
				return ((char *)big + i);
		}
		i++;
		len--;
	}
	return (NULL);
}
