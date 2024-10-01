/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 23:25:26 by donghank          #+#    #+#             */
/*   Updated: 2024/09/29 15:46:12 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// to check the quote(single and double) is closed
// if close return 1 if not return 0
int	check_unclosed_quote(char *str, char quote)
{
	int	idx;

	idx = 1;
	while (str[idx] && str[idx] != quote)
		idx++;
	if (str[idx] == '\0')
		return (NOT_CLOSED);
	return (CLOSED);
}

// get excutable inside of the double quote
// $ mean --> after, that is the environnement value
int	double_quote_cnt(char *str, int *size, t_env *env)
{
	int	idx;

	idx = 1;
	while (str[idx] && str[idx] != '\"')
	{
		if (str[idx] == '$')
			idx += (env_cnt(&str[idx], size, env) + 1);
		else
		{
			idx++;
			(*size)++;
		}
	}
	return (idx);
}

// to treat single quote cases
// calculate the string inside of the single quote
int	single_quote_cnt(char *str, int *size)
{
	int	idx;

	idx = 1;
	while (str[idx] && str[idx] != '\'')
	{
		idx++;
		(*size)++;
	}
	return (idx);
}
