/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:26:46 by donghank          #+#    #+#             */
/*   Updated: 2024/09/25 16:28:19 by pzinurov         ###   ########.fr       */
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

/* returns amount of redirects found */
int	fill_args(char ***tokens, t_glob_pipe *temp_pipe, int n, int start_index)
{
	int	i_tokens;
	int	i_args;
	int	redirects_amount;

	i_tokens = 0;
	redirects_amount = 0;
	i_args = 0;
	temp_pipe->args = malloc (sizeof (char *) * (calc_valid_args(tokens, n, start_index) + 1));
	if (!temp_pipe->args)
		return (0);
	while (i_tokens < n)
	{
		if (is_redirect(tokens[start_index + i_tokens]))
		{
			i_tokens++;
			redirects_amount++;
		}
		else
		{
			temp_pipe->args[i_args++] = ft_strdup(tokens[start_index + i_tokens][0]);
			if (!temp_pipe->args[i_args - 1])
				return (free_doub_array(temp_pipe->args), temp_pipe->args = NULL, 0);
		}
		i_tokens++;
	}
	temp_pipe->args[i_args] = NULL;
	temp_pipe->name = temp_pipe->args[0];
	if (redirects_amount && temp_pipe->operator == PIPE)
		temp_pipe->operator = REDIR_PIPE;
	else if (redirects_amount)
		temp_pipe->operator = REDIRECT_EXPECTED;
	return (redirects_amount);
}

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
	else if (word[0] == '|')
		operator = PIPE;
	else if (word[0] == '>')
		operator = REDIRECT_OUT;
	else if (word[0] == '<')
		operator = REDIRECT_IN;
	if (operator && glob_pipe)
		glob_pipe->operator = operator;
	return (operator);
}

int	fill_operator_token(t_glob_pipe *glob_pipe, char **token)
{
	int		operator;
	char	*word;

	operator = 0;
	word = token[0];
	if (token[1][0])
		return (0);
	if (!ft_strncmp(word, "||", 2))
		operator = OR;
	else if (!ft_strncmp(word, "&&", 2))
		operator = AND;
	else if (!ft_strncmp(word, ">>", 2))
		operator = APPEND_OUT;
	else if (!ft_strncmp(word, "<<", 2))
		operator = HERE_DOC;
	else if (word[0] == '|')
		operator = PIPE;
	else if (word[0] == '>')
		operator = REDIRECT_OUT;
	else if (word[0] == '<')
		operator = REDIRECT_IN;
	if (operator && glob_pipe)
		glob_pipe->operator = operator;
	return (operator);
}
