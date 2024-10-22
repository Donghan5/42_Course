/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/21 21:21:00 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_literal(char	*str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	if (!str || !*str)
		return (0);
	while (str[i] != '\0')
	{
		if (!quote && is_quote(str[i]))
			quote = str[i];
		else if (quote && is_quote(str[i]) == quote)
			return (1);
		i++;
	}
	return (0);
}

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

/*
	Returns char ***tokens, which is structured as:

	tokens[i]	 - double array of two strings;
	tokens[i][0] - content of a token (string);
	tokens[i][1] - string with one character, indicating complete quoting,
		tokens[i][1][0]	== 1	if tokens[i][0] was quoted before expansion,
		tokens[i][1][0] == 0	otherwise.
*/
char	***pre_parsing(char *line, t_env *env)
{
	char	***tokens;
	char	**temp_strs;
	int		i;

	temp_strs = tokenizer(line);
	if (!temp_strs || !temp_strs[0])
		return (NULL);
	tokens = malloc(sizeof(char **) * (count_tokens(temp_strs) + 1));
	if (!tokens)
		return (handle_errors_tokens(temp_strs, NULL, "malloc"), NULL);
	i = 0;
	while (temp_strs[i])
	{
		tokens[i] = malloc(sizeof(char *) * 2);
		if (!tokens[i])
			return (handle_errors_tokens(temp_strs, tokens, "malloc"), NULL);
		tokens[i][0] = expander(temp_strs[i], env);
		tokens[i][1] = malloc(1);
		if (!tokens[i][0] || !tokens[i][1])
			return (handle_errors_tokens(temp_strs, tokens, "malloc"), NULL);
		*tokens[i][1] = is_literal(temp_strs[i]);
		i++;
	}
	tokens[i] = NULL;
	return (free_doub_array(temp_strs), tokens);
}
