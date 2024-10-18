/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:38:13 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/21 20:07:21 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	i = -1;
	if (!dest || !src)
		return (NULL);
	if (n == 0)
		return (dest);
	d = dest;
	s = (unsigned char *)src;
	if (s < d)
	{
		n--;
		while (n >= 0)
		{
			*(d + n) = *(s + n);
			if (n == 0)
				return (dest);
			n--;
		}
	}
	while (++i < n)
		*(d + i) = *(s + i);
	return (dest);
}
