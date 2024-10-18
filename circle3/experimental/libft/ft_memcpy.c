/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:38:05 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 19:12:34 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*addr_dest;
	void	*addr_src;
	size_t	i;

	i = -1;
	if (!dest && !src)
		return (NULL);
	addr_dest = dest;
	addr_src = (void *)src;
	if (addr_src + n >= addr_dest && addr_src + n <= addr_dest + n)
	{
		n--;
		while (n >= 0)
		{
			*(unsigned char *)(dest + n) = *(unsigned char *)(src + n);
			if (n == 0)
				return (dest);
			n--;
		}
	}
	while (++i < n)
		*(unsigned char *)(dest + i) = *(unsigned char *)(src + i);
	return (dest);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	size_t		j;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		j = 0;
		while (j < n)
		{
			if (&(dest[i]) == &(src[j]))
			{
				ft_memmove(dest, src, n);
				return (dest);
			}
			j++;
		}
		i++;
	}
	i = -1;
	while (++i < n)
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
	return (dest);
}
