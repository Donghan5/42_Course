/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:11:06 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

t_glob_pipe	*new_glob_pipe(t_glob_pipe	*prev)
{
	t_glob_pipe	*new_elem;

	new_elem = malloc(sizeof (t_glob_pipe));
	if (!new_elem)
		return (NULL);
	new_elem->args = NULL;
	new_elem->name = NULL;
	new_elem->previous = NULL;
	new_elem->next = NULL;
	new_elem->close_count = 0;
	new_elem->is_exec_ignore = 0;
	new_elem->op = 0;
	new_elem->pipe_fds[0] = -1;
	new_elem->pipe_fds[1] = -1;
	new_elem->redir_io[0] = -1;
	new_elem->redir_io[1] = -1;
	new_elem->priority = 0;
	if (prev)
	{
		prev->next = new_elem;
		new_elem->previous = prev;
	}
	return (new_elem);
}

int	create_redirs(char ***tokens, int n, int index, t_glob_pipe **temp)
{
	int	i;

	i = index;
	while (i < index + n)
	{
		if (is_redirect(tokens[i]) && tokens[i + 1])
		{
			*temp = new_glob_pipe(*temp);
			if (!*temp)
				return (0);
			fill_operator_token(*temp, tokens[i]);
			(*temp)->args = malloc(sizeof (char *) * 2);
			if (!(*temp)->args)
				return (0);
			(*temp)->args[0] = ft_strdup(tokens[i + 1][0]);
			(*temp)->args[1] = NULL;
			(*temp)->name = (*temp)->args[0];
		}
		i++;
	}
	return (1);
}

int	fill_token_to_gpipe(t_glob_pipe **tmp, char ***toks, int toks_n, int *i)
{
	int	redirs;

	if (toks[*i] && ((is_operator_token(toks[*i]) && !is_redirect(toks[*i])
				&& !is_paren(toks[*i])) || !toks[*i + 1]))
	{
		fill_operator_token(*tmp, toks[*i]);
		if (!toks[*i + 1])
			redirs = fill_args(toks, tmp, toks_n, *i + 1 - toks_n);
		else
			redirs = fill_args(toks, tmp, toks_n, *i - toks_n);
		if (!(*tmp)->args)
			return (0);
		if (redirs
			&& !create_redirs(toks, toks_n,
				*i + !toks[*i + 1] - toks_n, tmp))
			return (0);
		if (toks[*i + 1])
		{
			*tmp = new_glob_pipe(*tmp);
			if (!(*tmp))
				return (0);
		}
		(*i) = *i + 1;
	}
	return (1);
}

int	priority_manager(char ***tokens, int *i, t_glob_pipe *t, int reset)
{
	static int	priority;

	if (reset)
		priority = 0;
	if (!reset && t)
	{
		if (is_paren(tokens[*i]) == PAREN_OPEN)
			priority++;
		if (!t->priority)
			t->priority = priority;
		if (is_paren(tokens[*i]) == PAREN_CLOSE)
			priority--;
		if (!tokens[*i + 1])
			return (0);
		(*i)++;
	}
	while (!t && !reset && is_paren(tokens[*i]))
	{
		if (is_paren(tokens[*i]) == PAREN_OPEN)
			priority++;
		if (is_paren(tokens[*i]) == PAREN_CLOSE)
			priority--;
		(*i)++;
	}
	return (1);
}

int	parse(t_glob_pipe **glob_pipe, char ***tokens)
{
	int			i;
	int			tokens_n;
	t_glob_pipe	*temp_glob;

	i = 0;
	*glob_pipe = new_glob_pipe(NULL);
	if (!(*glob_pipe))
		return (handle_errors(NULL, NULL, "malloc"));
	temp_glob = *glob_pipe;
	priority_manager(NULL, NULL, NULL, 1);
	while (tokens[i])
	{
		tokens_n = 0;
		priority_manager(tokens, &i, NULL, 0);
		while (tokens[i] && !(is_operator_token(tokens[i])
				&& !is_redirect(tokens[i]) && !is_paren(tokens[i])))
		{
			tokens_n++;
			if (!priority_manager(tokens, &i, temp_glob, 0))
				break ;
		}
		if (!fill_token_to_gpipe(&temp_glob, tokens, tokens_n, &i))
			return (handle_errors(glob_pipe, NULL, "malloc"));
	}
	return (1);
}
