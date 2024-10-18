/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:40:05 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/30 20:58:18 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	skip_the_rest(char const **s, int *quoted)
{
	int	operator_length;

	operator_length = 0;
	while (ft_iswhitespace(**s))
		(*s)++;
	if (!*quoted && is_operator((char *)*s))
	{
		operator_length = is_operator((char *)*s);
		while (**s && operator_length--)
			(*s)++;
		return (2);
	}
	while (**s && !is_quote(**s)
		&& !is_operator((char *)*s) && !ft_iswhitespace(**s))
	{
		(*s)++;
		*quoted = is_quote(**s);
	}
	while (ft_iswhitespace(**s))
		(*s)++;
	return (1);
}

static int	skip_quoted(char const **s, int *quoted)
{
	if (!*quoted)
		return (1);
	(*s)++;
	while (**s && (**s != *quoted))
		(*s)++;
	if (**s != *quoted)
		return (0);
	else
		(*s)++;
	if (!**s || ft_iswhitespace(**s) || is_operator((char *)*s))
	{
		*quoted = 0;
		return (2);
	}
	return (1);
}

int	calc_tokens(char const *s)
{
	int	amount;
	int	quoted;
	int	status;

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
		status = skip_quoted(&s, &quoted);
		if (!status)
			return (-1);
		else if (status == 2)
			continue ;
		status = skip_the_rest(&s, &quoted);
		if (status == 2)
			continue ;
	}
	return (amount);
}
