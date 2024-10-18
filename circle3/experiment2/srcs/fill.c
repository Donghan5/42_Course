/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:26:46 by donghank          #+#    #+#             */
/*   Updated: 2024/10/17 23:05:40 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fill_operator(t_glob_pipe *glob_pipe, char *word)
{
	int		operator;

	operator = 0;
	if (!ft_strncmp(word, "||", 2))
		operator = OR;
	else if (!ft_strncmp(word, "&&", 2))
		operator = AND;
	else if (!ft_strncmp(word, ">>", 2))
		operator = APPEND_OUT;
	else if (!ft_strncmp(word, "<<", 2))
		operator = HERE_DOC;
	else if (word[0] == '(')
		operator = PAREN_OPEN;
	else if (word[0] == ')')
		operator = PAREN_CLOSE;
	else if (word[0] == '|')
		operator = PIPE;
	else if (word[0] == '>')
		operator = REDIRECT_OUT;
	else if (word[0] == '<')
		operator = REDIRECT_IN;
	if (operator && glob_pipe)
		glob_pipe->op = operator;
	return (operator);
}

int	fill_operator_token(t_glob_pipe *glob_pipe, char **token)
{
	int		operator;

	operator = 0;
	if (token[1][0])
		return (0);
	operator = fill_operator(glob_pipe, token[0]);
	if (operator && glob_pipe)
		glob_pipe->op = operator;
	if (glob_pipe && (operator == PAREN_CLOSE || operator == PAREN_OPEN))
		glob_pipe->op = 0;
	return (operator);
}
