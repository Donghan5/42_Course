/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_lex_error_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:33:39 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/21 21:18:22 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

int	compound_error(char **current, char **next, t_glob_pipe **glob_pipe)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (is_redirect(current) && next)
		ft_putstr_fd(next[0], 2);
	else if (is_redirect(current))
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(current[0], 2);
	ft_putstr_fd("\'\n", 2);
	if (glob_pipe)
		free_glob_pipe(glob_pipe);
	return (0);
}

int	is_balance(char **current, char **next_token, int reset)
{
	static int	balance;

	if (reset)
	{
		balance = 0;
		return (1);
	}
	if (is_paren(current) == PAREN_OPEN)
		balance++;
	if (is_paren(current) == PAREN_CLOSE)
		balance--;
	if (!next_token && balance != 0)
	{
		balance = 0;
		return (0);
	}
	return (1);
}

int	is_duo_error(char **current, char **next)
{
	int	paren;
	int	paren_n;

	paren = is_paren(current);
	paren_n = is_paren(next);
	if (fill_operator_token(NULL, current) == PIPE && is_redirect(next))
		return (0);
	if (((is_operator_token(current) && !paren)
			&& (is_operator_token(next) && !paren_n))
		|| (paren == PAREN_OPEN && paren_n == PAREN_CLOSE))
		return (1);
	if (is_redirect(current) && paren_n)
		return (1);
	if (!is_operator_token(current) && paren_n == PAREN_OPEN)
		return (1);
	if ((paren == PAREN_OPEN && (is_operator_token(next)
				&& !is_redirect(next) && !paren_n))
		|| (paren_n == PAREN_CLOSE && (is_operator_token(current)
				&& !is_redirect(current)) && !paren))
		return (1);
	return (0);
}

int	is_tokens_error(char **current, char **next, int i)
{
	int	paren;

	if (!is_balance(current, next, 0))
		return (1);
	paren = is_paren(current);
	if ((i == 0) && is_operator_token(current)
		&& !(is_redirect(current) || (paren == PAREN_OPEN)))
		return (1);
	if ((is_operator_token(current) && !(paren == PAREN_CLOSE))
		&& (!next || (!*next[0] && !next[1][0])))
		return (1);
	if (!next)
		return (0);
	return (is_duo_error(current, next));
}

int	is_lex_error(char ***tokens)
{
	int	i;

	i = 0;
	is_balance(NULL, NULL, 1);
	if (!tokens)
		return (1);
	while (tokens[i])
	{
		if (is_tokens_error(tokens[i], tokens[i + 1], i))
			return (compound_error(tokens[i], tokens[i + 1], NULL), 1);
		i++;
	}
	return (0);
}
