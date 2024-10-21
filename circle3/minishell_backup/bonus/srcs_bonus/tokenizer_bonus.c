/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/21 19:26:10 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static int	copy_quoted(char **splitted, char const **s, int *quoted, int *i_j)
{
	if (!*quoted)
		return (1);
	splitted[i_j[0]][i_j[1]++] = *(*s);
	(*s)++;
	while (**s && (**s != *quoted))
	{
		splitted[i_j[0]][i_j[1]++] = *(*s);
		(*s)++;
	}
	if (**s != *quoted)
		return (handle_errors_tokens(splitted, \
			NULL, "minishell: unexpected EOF while \
looking for matching `'' or `\"'\n"));
	else
	{
		splitted[i_j[0]][i_j[1]++] = *(*s);
		(*s)++;
	}
	if (!**s || ft_iswhitespace(**s) || is_operator((char *)*s))
	{
		*quoted = 0;
		splitted[i_j[0]++][i_j[1]] = '\0';
		return (2);
	}
	return (1);
}

static int	copy_the_rest(char **splttd, char const **s, int *quoted, int *i_j)
{
	int	operator_length;

	while (!*quoted && ft_iswhitespace(**s))
		(*s)++;
	if (!*quoted && is_operator((char *)*s))
	{
		operator_length = is_operator((char *)*s);
		while (**s && operator_length--)
			splttd[i_j[0]][i_j[1]++] = *(*s)++;
		splttd[i_j[0]++][i_j[1]] = '\0';
		return (2);
	}
	while (**s && !is_quote(**s)
		&& !is_operator((char *)*s) && !ft_iswhitespace(**s))
	{
		splttd[i_j[0]][i_j[1]++] = *(*s)++;
		*quoted = is_quote(**s);
	}
	if (!*quoted && !is_quote(**s))
		splttd[i_j[0]++][i_j[1]] = '\0';
	while (!*quoted && ft_iswhitespace(**s))
		(*s)++;
	return (1);
}

static int	tokenizing(char **splitted, int tokens, char const *s)
{
	int		i_j[2];
	int		quoted;
	int		copy_status;

	quoted = 0;
	i_j[0] = 0;
	while (i_j[0] < tokens)
	{
		if (!quoted && !create_new_token(splitted, &s, i_j))
			return (0);
		quoted = is_quote(*s);
		copy_status = copy_quoted(splitted, &s, &quoted, i_j);
		if (!copy_status)
			return (0);
		if (copy_status == 2)
			continue ;
		copy_status = copy_the_rest(splitted, &s, &quoted, i_j);
	}
	return (splitted[i_j[0]] = NULL, 1);
}

char	**tokenizer(char const *s)
{
	char	**splitted;
	int		tokens;

	if (!s)
		return (NULL);
	tokens = calc_tokens(s);
	if (tokens == -1)
		return (smart_print_err("minishell: unexpected EOF while \
looking for matching `'' or `\"'\n"), NULL);
	splitted = malloc(sizeof(char *) * (tokens + 1));
	if (!splitted)
		return (NULL);
	if (tokenizing(splitted, tokens, s))
		return (splitted);
	return (NULL);
}
