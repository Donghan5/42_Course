/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 21:50:04 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	fill_token_to_gpipe(t_glob_pipe **tmp, char ***tokens, int tokens_n, int *i)
{
	int	redirs;

	if (tokens[*i] && ((is_operator_token(tokens[*i])
				&& !is_redirect(tokens[*i])) || !tokens[*i + 1]))
	{
		fill_operator_token(*tmp, tokens[*i]);
		if (!tokens[*i + 1])
			redirs = fill_args(tokens, tmp, tokens_n, *i + 1 - tokens_n);
		else
			redirs = fill_args(tokens, tmp, tokens_n, *i - tokens_n);
		if (!(*tmp)->args)
			return (0);
		if (redirs
			&& !create_redirs(tokens, tokens_n,
				*i + !tokens[*i + 1] - tokens_n, tmp))
			return (0);
		if (tokens[*i + 1])
		{
			*tmp = new_glob_pipe(*tmp);
			if (!(*tmp))
				return (0);
		}
		(*i) = *i + 1;
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
	while (tokens[i])
	{
		tokens_n = 0;
		while (tokens[i]
			&& !(is_operator_token(tokens[i]) && !is_redirect(tokens[i])))
		{
			tokens_n++;
			if (!tokens[i + 1])
				break ;
			i++;
		}
		if (!fill_token_to_gpipe(&temp_glob, tokens, tokens_n, &i))
			return (handle_errors(glob_pipe, NULL, "malloc"));
	}
	return (1);
}
