/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_checks_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:29:47 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 22:11:10 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

/* returns the length of operator */
int	is_operator(char *token)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, token);
	if (next_interaction == PAREN_OPEN || next_interaction == PAREN_CLOSE)
		return (1);
	if (next_interaction == AND || next_interaction == APPEND_OUT
		|| next_interaction == HERE_DOC
		|| next_interaction == OR)
		return (2);
	if ((next_interaction >= 1) && (next_interaction <= 7))
		return (1);
	return (0);
}

int	is_operator_token(char **token)
{
	int	next_interaction;

	if (token[1][0])
		return (0);
	next_interaction = fill_operator_token(NULL, token);
	if (next_interaction == PAREN_OPEN || next_interaction == PAREN_CLOSE)
		return (1);
	if (next_interaction == AND || next_interaction == APPEND_OUT
		|| next_interaction == HERE_DOC
		|| next_interaction == OR)
		return (2);
	if ((next_interaction >= 1) && (next_interaction <= 7))
		return (1);
	return (0);
}

int	is_paren(char **token)
{
	int	next_interaction;

	if (token[1][0])
		return (0);
	next_interaction = fill_operator_token(NULL, token);
	if (next_interaction == PAREN_CLOSE)
		return (PAREN_CLOSE);
	if (next_interaction == PAREN_OPEN)
		return (PAREN_OPEN);
	return (0);
}

int	is_redirect(char **token)
{
	int	next_interaction;

	if (token[1][0])
		return (0);
	next_interaction = fill_operator_token(NULL, token);
	if ((next_interaction >= 4) && (next_interaction <= 7))
		return (1);
	return (0);
}
