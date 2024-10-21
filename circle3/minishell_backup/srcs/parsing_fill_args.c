/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fill_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:28:36 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 21:49:36 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	calc_valid_args(char ***token, int n, int start_index)
{
	int	i;
	int	args_counter;

	i = 0;
	args_counter = 0;
	while (i < n)
	{
		if (is_redirect(token[start_index + i]))
			i++;
		else
			args_counter++;
		i++;
	}
	return (args_counter);
}

/*
	Returns redirects_amount
*/
int	copy_args_to_gpipe(char ***tokens, t_glob_pipe *tmp, int n, int start_index)
{
	int	i_tokens;
	int	i_args;
	int	redirects_amount;

	i_args = 0;
	i_tokens = 0;
	redirects_amount = 0;
	while (i_tokens < n)
	{
		if (is_redirect(tokens[start_index + i_tokens]))
		{
			i_tokens++;
			redirects_amount++;
		}
		else if (tokens[start_index + i_tokens][0][0]
			|| tokens[start_index + i_tokens][1][0])
		{
			tmp->args[i_args++] = ft_strdup(tokens[start_index + i_tokens][0]);
			if (!tmp->args[i_args - 1])
				return (free_doub_array(tmp->args), tmp->args = NULL, -1);
		}
		i_tokens++;
	}
	tmp->args[i_args] = NULL;
	return (redirects_amount);
}

/* returns amount of redirects found */
int	fill_args(char ***tokens, t_glob_pipe **temp_pipe, int n, int start_index)
{
	int	i_tokens;
	int	i_args;
	int	redirects_amount;

	i_tokens = 0;
	redirects_amount = 0;
	i_args = 0;
	(*temp_pipe)->args = malloc (sizeof (char *) \
			* (calc_valid_args(tokens, n, start_index) + 1));
	if (!(*temp_pipe)->args)
		return (0);
	redirects_amount = copy_args_to_gpipe(tokens, *temp_pipe, n, start_index);
	if (redirects_amount == -1)
		return (0);
	(*temp_pipe)->name = (*temp_pipe)->args[0];
	if (redirects_amount && (*temp_pipe)->op == PIPE)
		(*temp_pipe)->op = REDIR_PIPE;
	else if (redirects_amount)
		(*temp_pipe)->op = REDIRECT_EXPECTED;
	return (redirects_amount);
}
