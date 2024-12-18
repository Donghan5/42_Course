/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:30:49 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/23 20:43:02 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	i;
	size_t	length_s1;
	size_t	length_s2;

	i = 0;
	length_s1 = ft_strlen(s1);
	length_s2 = ft_strlen(s2);
	new_str = malloc(length_s1 + length_s2 + 1);
	if (new_str == NULL)
		return (NULL);
	while (i < length_s1)
	{
		new_str[i] = s1[i];
		i++;
	}
	while (i - length_s1 < length_s2)
	{
		new_str[i] = s2[i - length_s1];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

void	ft_bzero(void *s, size_t n)
{
	while (n)
	{
		*(unsigned char *)s = '\0';
		s++;
		n--;
	}
}

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
		while (1)
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
