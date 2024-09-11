/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:36:45 by donghank          #+#    #+#             */
/*   Updated: 2024/09/11 16:10:45 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to do role of the expanded_str in double quote
// action like echo "Hello $USER" -> "Hello donghank(username)"
// have to think where we put this things
char	*expander(char *input_str, char **envp)
{
	int		idx;
	int		size;
	char	*expanded_str;

	size = get_env_parse_len(input_str, envp);
	idx = 0;
	while (input_str[idx])
	{
		if (input_str[idx] == '\'' && \
		check_unclosed_quote(&input_str[idx], '\''))
			idx += single_quote_cnt(&input_str[idx], &size);
		else if (input_str[idx] == '"' && \
		check_unclosed_quote(&input_str[idx], '"'))
			idx += double_quote_cnt(&input_str[idx], &size, envp);
		else if (input_str[idx] == '$')
			idx += env_cnt(&input_str[idx], &size, envp);
		else
			expanded_str[size++] = input_str[idx++];
	}
	expanded_str[size] = '\0';
	return (expanded_str);
}
