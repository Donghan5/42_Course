/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_new_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:42:26 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/30 20:47:51 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	length_of_token(char const *s)
{
	int	i;
	int	quote;

	i = 0;
	if (is_operator((char *)s))
		return (is_operator((char *)s) + 1);
	while (s[i] && !ft_iswhitespace(s[i]) && !is_operator((char *)(s + i)))
	{
		if (is_quote(s[i]))
		{
			quote = is_quote(s[i]);
			i++;
			while (is_quote(s[i]) != quote)
				i++;
		}
		i++;
	}
	i++;
	return (i);
}

int	create_new_token(char **splitted, char const **s, int *i_j)
{
	while (**s && ft_iswhitespace(**s))
		(*s)++;
	i_j[1] = 0;
	splitted[i_j[0]] = malloc(length_of_token(*s));
	if (!splitted[i_j[0]])
		return (handle_errors_tokens(splitted, NULL, "malloc"));
	return (1);
}
