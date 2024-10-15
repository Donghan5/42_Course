/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:26:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:44:17 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
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
	if (!new_str)
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
