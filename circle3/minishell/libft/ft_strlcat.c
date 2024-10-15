/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:38:34 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:49:05 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	src_len;
	size_t	dst_len;

	i = 0;
	if (siz == 0 && !src)
		return (0);
	src_len = ft_strlen(src);
	if (siz == 0)
		return (src_len);
	dst_len = ft_strlen(dst);
	while (i < src_len && (dst_len + i) < (siz - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (siz > dst_len)
		dst[dst_len + i] = '\0';
	if (siz < dst_len)
		return (src_len + siz);
	return (src_len + dst_len);
}
