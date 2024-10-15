/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:47:49 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:51:55 by pzinurov         ###   ########.fr       */
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

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new_str;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start || len == 0)
		new_str = malloc(1);
	else if (ft_strlen(s) - start < len)
		new_str = malloc(ft_strlen(s) - start + 1);
	else
		new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	if (ft_strlen(s) > start)
	{
		while (s[start + i] != '\0' && i < len)
		{
			new_str[i] = s[start + i];
			i++;
		}
	}
	new_str[i] = '\0';
	return (new_str);
}
