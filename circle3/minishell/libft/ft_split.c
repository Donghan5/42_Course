/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 16:52:53 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	get_strs_amount(char const *s, char c)
{
	int	amount;

	amount = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			amount++;
			s++;
		}
		while (*s && *s != c)
			s++;
	}
	return (amount);
}

static int	strlen_before_c(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static void	error_free(char **arr, int amount)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (i < amount)
		{
			if (arr[i])
				free (arr[i]);
			i++;
		}
		free (arr);
	}
}

char	**ft_split(char const *s, char c)
{
	char	**splitted;
	int		i;
	int		j;
	int		strs;

	i = 0;
	if (!s)
		return (NULL);
	strs = get_strs_amount(s, c);
	splitted = malloc(sizeof(char *) * (strs + 1));
	if (!splitted)
		return (NULL);
	while (i < strs)
	{
		j = 0;
		while (*s == c)
			s++;
		splitted[i] = malloc(strlen_before_c(s, c) + 1);
		if (!splitted[i])
			return (error_free(splitted, i), NULL);
		while (*s && *s != c)
			splitted[i][j++] = *(s++);
		splitted[i++][j] = '\0';
	}
	return (splitted[i] = NULL, splitted);
}
