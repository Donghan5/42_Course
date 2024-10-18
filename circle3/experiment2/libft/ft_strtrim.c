/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:36:26 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:52:22 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static int	length_bef_trim(char const *s1_middle, char const *set)
{
	int	i;
	int	j;

	i = 0;
	while (s1_middle[i] != '\0')
	{
		j = 0;
		while (is_in_set(*(s1_middle + i + j), set))
		{
			j++;
			if (!*(s1_middle + i + j))
				return (i);
		}
		i++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	int		i;
	int		middle_length;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (is_in_set(*s1, set))
		s1++;
	middle_length = length_bef_trim(s1, set);
	new_str = malloc(middle_length + 1);
	if (!new_str)
		return (NULL);
	while (i < middle_length)
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
