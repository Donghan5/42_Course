/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_whitesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/08/24 23:04:41 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

static int	get_strs_amount(char const *s)
{
	int	amount;

	amount = 0;
	while (*s)
	{
		while (ft_iswhitespace(*s))
			s++;
		if (*s)
		{
			amount++;
			s++;
		}
		while (*s && !ft_iswhitespace(*s))
			s++;
	}
	return (amount);
}

static int	strlen_before_c(char const *s)
{
	int	i;

	i = 0;
	while (s[i] && !ft_iswhitespace(s[i]))
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

char	**ft_whitesplit(char const *s)
{
	char	**splitted;
	int		i;
	int		j;
	int		strs;

	i = 0;
	if (!s)
		return (NULL);
	strs = get_strs_amount(s);
	splitted = malloc(sizeof(char *) * (strs + 1));
	if (!splitted)
		return (NULL);
	while (i < strs)
	{
		j = 0;
		while (ft_iswhitespace(*s))
			s++;
		splitted[i] = malloc(strlen_before_c(s) + 1);
		if (!splitted[i])
			return (error_free(splitted, i), NULL);
		while (*s && !ft_iswhitespace(*s))
			splitted[i][j++] = *(s++);
		splitted[i++][j] = '\0';
	}
	return (splitted[i] = NULL, splitted);
}
