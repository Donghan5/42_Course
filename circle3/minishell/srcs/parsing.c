/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/09/24 16:00:53 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* returns the length of operator */
int	is_operator(char *token)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, token);
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
	if (next_interaction == AND || next_interaction == APPEND_OUT
		|| next_interaction == HERE_DOC
		|| next_interaction == OR)
		return (2);
	if ((next_interaction >= 1) && (next_interaction <= 7))
		return (1);
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

t_glob_pipe	*new_glob_pipe(t_glob_pipe	*prev);

int	add_redir_as_glob_pipes(char ***tokens, int n, int index, t_glob_pipe **temp)
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
	new_elem->operator = 0;
	if (prev)
	{
		prev->next = new_elem;
		new_elem->previous = prev;
	}
	return (new_elem);
}

int	parse(t_glob_pipe **glob_pipe, char ***tokens)
{
	int			i;
	int			token_counter;
	int			files_counter;
	int			redirects_found;
	t_glob_pipe	*temp_glob;

	i = 0;
	*glob_pipe = new_glob_pipe(NULL);
	if (!(*glob_pipe))
		return (handle_errors(NULL, NULL, "malloc"));
	temp_glob = *glob_pipe;
	while (tokens[i])
	{
		token_counter = 0;
		while (tokens[i] && !(is_operator_token(tokens[i]) && !is_redirect(tokens[i])))
		{
			token_counter++;
			if (!tokens[i + 1])
				break ;
			i++;
		}
		if (tokens[i] && is_operator_token(tokens[i]) && !tokens[i + 1])
			return (handle_errors(glob_pipe, NULL, "minishell: parse error near `newline\'\n"));
		if (tokens[i] && (is_operator_token(tokens[i]) && !is_redirect(tokens[i]) || !tokens[i + 1]))
		{
			fill_operator_token(temp_glob, tokens[i]);
			if (!tokens[i + 1])
				redirects_found = fill_args(tokens, temp_glob, token_counter, i + 1 - token_counter);
			else
				redirects_found = fill_args(tokens, temp_glob, token_counter, i - token_counter);
			if (!temp_glob->args)
				return (handle_errors(glob_pipe, NULL, "malloc"));
			if (tokens[i + 1] && is_operator_token(tokens[i + 1]))
				return (handle_errors(glob_pipe, NULL, "minishell: wrong arguments\n"));
			if (redirects_found && !add_redir_as_glob_pipes(tokens, token_counter, i + !tokens[i + 1] - token_counter, &temp_glob))
				return (handle_errors(glob_pipe, NULL, "malloc"));
			if (tokens[i + 1])
			{
				temp_glob = new_glob_pipe(temp_glob);
				if (!temp_glob)
					return (handle_errors(glob_pipe, NULL, "malloc"));
			}
			i++;
		}
	}
	return (1);
}

void	parse_env(t_env *env, char **envs)
{
	int		i;
	char	**temp;

	i = 0;
	env->environ = NULL;
	while (envs && envs[i])
		i++;
	if (!i)
	{
		env->environ = malloc(sizeof(char *));
		if (!env->environ)
			exit_error("malloc");
		env->environ[0] = NULL;
		return ;
	}
	env->environ = malloc((i + 1) * sizeof(char *));
	if (!env->environ)
			exit_error("malloc");
	i = 0;
	while (envs && envs[i])
	{
		env->environ[i] = ft_strdup(envs[i]);
		if (!env->environ)
			return (free_doub_array((void **) env->environ), exit_error("malloc"));
		i++;
	}
	env->environ[i] = NULL;
}
