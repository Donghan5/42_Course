/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:52:46 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/21 16:17:21 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_literal(char	*str)
{
	int	i;

	i = 0;
	if (!str || !*str || !is_quote(*str))
		return (0);
	while (str[i] != '\0')
		i++;
	if ((i > 1) && (is_quote(str[i - 1])))
		return (1);
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

char	***pre_parsing(char *line, t_env *env)
{
	char	***tokens;
	char	**temp_strs;
	int		i;

	temp_strs = tokenizer(line);
	if (!temp_strs || !temp_strs[0])
		return (NULL);
	tokens = malloc(sizeof(char **) * count_tokens(temp_strs));
	if (!tokens)
		return (handle_errors_tokens(temp_strs, NULL, "malloc"), NULL);
	i = 0;
	while (temp_strs[i])
	{
		tokens[i] = malloc(sizeof(char *) * 2);
		if (!tokens[i])
			return (handle_errors_tokens(temp_strs, tokens, "malloc"), NULL);
		tokens[i][0] = expander(temp_strs[i], env);
		if (!tokens[i][0])
			return (handle_errors_tokens(temp_strs, tokens, "malloc"), NULL);
		tokens[i][1] = malloc(1);
		if (!tokens[i][1])
			return (handle_errors_tokens(temp_strs, tokens, "malloc"), NULL);
		*tokens[i][1] = is_literal(temp_strs[i]);
		// printf("tokens[%d][0] = %s; tokens[%d][1] = %d\n", i, tokens[i][0], i, tokens[i][1][0]);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}