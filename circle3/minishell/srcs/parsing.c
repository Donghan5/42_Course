/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 14:00:06 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* returns the length of operator */
int	is_operator(char *token)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, token);
	if (next_interaction == AND || next_interaction == APPEND_OUT || next_interaction == HERE_DOC
		|| next_interaction == OR)
		return (2);
	if ((next_interaction >= 1) && (next_interaction <= 7))
		return (1);
	return (0);
}

int	is_redirect(char *token)
{
	int	next_interaction;

	next_interaction = fill_operator(NULL, token);
	if ((next_interaction >= 4) && (next_interaction <= 7))
		return (1);
	return (0);
}

t_glob_pipe	*new_glob_pipe(t_glob_pipe	*prev);

int	add_redir_as_glob_pipes(char **tokens, int n, int start_index, t_glob_pipe *temp_pipe)
{
	int	i;
	
	i = start_index;
	while (i < start_index + n)
	{
		if (is_redirect(tokens[i]) && tokens[i + 1])
		{
			temp_pipe = new_glob_pipe(temp_pipe);
			if (!temp_pipe)
				return (0);
			fill_operator(temp_pipe, tokens[i]);
			temp_pipe->args = malloc(sizeof (char *) * 2);
			temp_pipe->args[0] = ft_strdup(tokens[i + 1]);
			temp_pipe->args[1] = NULL;
			temp_pipe->name = temp_pipe->args[0];
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
	// memset(new_elem, 0, sizeof (t_glob_pipe));
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

int	parse(t_glob_pipe **glob_pipe, char *line)
{
	int			i;
	int			token_counter;
	int			files_counter;
	int			redirects_found;
	char		**tokens;
	t_glob_pipe	*temp_glob;

	tokens = ft_split_tokens(line);
	if (!tokens)
		return (0);
	i = 0;
	*glob_pipe = new_glob_pipe(NULL);
	if (!(*glob_pipe))
		return (free_doub_array((void **) tokens), 0);
	temp_glob = *glob_pipe;
	while (tokens[i])
	{
		token_counter = 0;
		while (tokens[i] && !(is_operator(tokens[i]) && !is_redirect(tokens[i])))
		{
			token_counter++;
			if (!tokens[i + 1])
				break;
			i++;
		}
		if (tokens[i] && is_operator(tokens[i]) && !tokens[i + 1])
		{
			ft_putstr_fd("minishell: parse error near `newline\'\n", 2);
			return (free_doub_array((void **) tokens), free_glob_pipe(glob_pipe), 0);
		}
		if (tokens[i] && (is_operator(tokens[i]) && !is_redirect(tokens[i]) || !tokens[i + 1]))
		{
			fill_operator(temp_glob, tokens[i]);
			if (!tokens[i + 1])
				redirects_found = fill_args(tokens, temp_glob, token_counter, i + 1 - token_counter);
			else
				redirects_found = fill_args(tokens, temp_glob, token_counter, i - token_counter);
			if (!temp_glob->args)
			{
				perror("malloc");
				return (free_doub_array((void **) tokens), free_glob_pipe(glob_pipe), 0);
			}
			if (tokens[i + 1] && is_operator(tokens[i + 1]))
			{
				ft_putstr_fd("minishell: wrong arguments\n", 2);
				return (free_doub_array((void **) tokens), free_glob_pipe(glob_pipe), 0);
			}
			if (redirects_found && !add_redir_as_glob_pipes(tokens, token_counter, i + !tokens[i + 1] - token_counter, temp_glob))
				return (free_doub_array((void **) tokens), free_glob_pipe(glob_pipe), 0);
			if (tokens[i + 1])
			{
				temp_glob = new_glob_pipe(temp_glob);
				if (!temp_glob)
					return (free_doub_array((void **) tokens), free_glob_pipe(glob_pipe), 0);
			}
			i++;
		}
	}
	free_doub_array((void **) tokens);
	return (1);
}

void	parse_env(t_env *env)
{
	int		i;
	char	**temp;

	i = 0;
	while (env->environ[i])
		i++;
	// env->environ_name_value = malloc(sizeof(t_env *) * (i + 1));
	// if (!env->environ_name_value)
	// 	exit_error("malloc");
	// i = 0;
	// while (env->environ[i])
	// {
	// 	temp = ft_split(env->environ[i], '=');
	// 	env->environ_name_value[i].name = temp[0];
	// 	env->environ_name_value[i].value = temp[1];
	// 	i++;
	// }
	// env->environ_name_value[i].name = NULL;
	// env->environ_name_value[i].value = NULL;
}
