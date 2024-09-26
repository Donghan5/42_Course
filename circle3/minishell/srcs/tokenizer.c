/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/26 16:16:38 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_still_an_operator(char *s);

static int	get_strs_amount(char const *s)
{
	int	amount;
	int quoted;
	int	operator_length;

	operator_length = 0;
	amount = 0;
	quoted = 0;
	while (*s)
	{
		if (!quoted)
		{
			while (*s && ft_iswhitespace(*s))
				s++;
			amount++;
		}
		quoted = is_quote(*s);
		if (quoted)
			s++;
		while (quoted && *s && (*s != quoted))
			s++;
		if (quoted && (*s != quoted))
			return (-1);
		else if (quoted)
			s++;
		if (quoted)
		{
			if (!*s || ft_iswhitespace(*s) || is_operator((char *)s))
			{
				quoted = 0;
				continue ;
			}
		}
		while (ft_iswhitespace(*s))
			s++;
		if (!quoted && is_operator((char *)s))
		{
			operator_length = is_operator((char *)s);
			while (*s && operator_length--)
				s++;
			continue ;
		}
		while (*s && !is_quote(*s) && !is_operator((char *)s) && !ft_iswhitespace(*s))
		{
			s++;
			if (!quoted)
				quoted = is_quote(*s);
		}
		while (ft_iswhitespace(*s))
			s++;
	}
	return (amount);
}

static int	length_of_token(char const *s)
{
	int	i;

	i = 0;
	if (is_quote(s[i]))
	{
		i++;
		while (!is_quote(s[i]))
			i++;
	}
	else
	{	
		if (is_operator((char *)s))
			return (is_operator((char *)s));
		while (s[i] && !ft_iswhitespace(s[i]) && !is_operator((char *)s))
			i++;
	}
	return (i);
}

static void	*error_free(char **arr, int amount, char *msg)
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
	smart_print_err(msg);
	return (NULL);
}

static int	is_still_an_operator(char *s)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, s);
	if ((next_interaction >= 1) && (next_interaction <= 7) || (*s == '&'))
		return (1);
	return (0);
}

char	**tokenizer(char const *s)
{
	char	**splitted;
	int		i;
	int		j;
	int		tokens;
	int		quoted;
	int		operator_length;

	if (!s)
		return (NULL);
	quoted = 0;
	i = 0;
	tokens = get_strs_amount(s);
	if (tokens == -1)
		return (error_free(NULL, 0, "quote is not closed\n"));
	// printf("amount of tokens: %d\n", tokens);
	splitted = malloc(sizeof(char *) * (tokens + 1));
	if (!splitted)
		return (NULL);
	while (i < tokens)
	{
		if (!quoted)
		{
			while (*s && ft_iswhitespace(*s))
				s++;
			j = 0;
			splitted[i] = malloc(1000);
			if (!splitted[i])
				return (error_free(splitted, i, "malloc"));
		}
		quoted = is_quote(*s);
		if (quoted)
			splitted[i][j++] = *(s++);
		while (quoted && *s && (*s != quoted))
			splitted[i][j++] = *(s++);
		if (quoted && (*s != quoted))
			return (error_free(splitted, i, "quote is not closed\n"));
		else if (quoted)
			splitted[i][j++] = *(s++);
		if (quoted)
		{
			if (!*s || ft_iswhitespace(*s) || is_operator((char *)s))
			{
				quoted = 0;
				splitted[i++][j] = '\0';
				continue ;
			}
		}
		while (!quoted && ft_iswhitespace(*s))
			s++;
		while (quoted && ft_iswhitespace(*s))
			splitted[i][j++] = *(s++);
		if (!quoted && is_operator((char *)s))
		{
			operator_length = is_operator((char *)s);
			while (*s && operator_length--)
				splitted[i][j++] = *(s++);
			splitted[i++][j] = '\0';
			continue ;
		}
		while (*s && !is_quote(*s) && !is_operator((char *)s) && !ft_iswhitespace(*s))
		{
			splitted[i][j++] = *(s++);
			if (!quoted)
				quoted = is_quote(*s);
		}
		if (!quoted && !is_quote(*s))
			splitted[i++][j] = '\0';
		while (!quoted && ft_iswhitespace(*s))
			s++;
		while (quoted && ft_iswhitespace(*s))
			splitted[i][j++] = *(s++);
	}
	return (splitted[i] = NULL, splitted);
}
