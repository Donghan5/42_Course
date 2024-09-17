/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/17 14:01:59 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_strs_amount(char const *s)
{
	int	amount;

	amount = 0;
	while (*s)
	{
		while (ft_iswhitespace(*s))
			s++;
		if (is_operator((char *)s))
		{
			amount++;
			s += is_operator((char *)s);
		}
		if (*s)
		{
			amount++;
			s++;
		}
		while (*s && !ft_iswhitespace(*s) && !is_operator((char *)s))
			s++;
	}
	return (amount);
}

static int	length_of_token(char const *s)
{
	int	i;

	i = 0;
	if (is_operator((char *)s))
		return (is_operator((char *)s));
	while (s[i] && !ft_iswhitespace(s[i]) && !is_operator((char *)s))
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

int	is_still_an_operator(char *s)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, s);
	if ((next_interaction >= 1) && (next_interaction <= 7) || (*s == '&'))
		return (1);
	return (0);
}

char	**ft_split_tokens(char const *s)
{
	char	**splitted;
	int		i;
	int		j;
	int		tokens;

	i = 0;
	if (!s)
		return (NULL);
	tokens = get_strs_amount(s);
	splitted = malloc(sizeof(char *) * (tokens + 1));
	if (!splitted)
		return (NULL);
	while (i < tokens)
	{
		j = 0;
		while (ft_iswhitespace(*s))
			s++;
		splitted[i] = malloc(length_of_token(s) + 1);
		if (!splitted[i])
			return (error_free(splitted, i), NULL);
		if (is_operator((char *)s))
		{
			while (*s && !ft_iswhitespace(*s) && is_still_an_operator((char *)s))
				splitted[i][j++] = *(s++);
		}
		else if (*s)
		{
			while (*s && !ft_iswhitespace(*s) && !is_operator((char *)s))
				splitted[i][j++] = *(s++);
		}
		splitted[i++][j] = '\0';
	}
	return (splitted[i] = NULL, splitted);
}
