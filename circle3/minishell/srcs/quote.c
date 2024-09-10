/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 23:25:26 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 23:25:58 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get excutable inside of the double quote
// $ mean --> after, that is the environnement value
int	double_quote_cnt(char *str, int *size, char **envp)
{
	int	idx;

	idx = 1;
	while (str[idx] && str[idx] == '\"')
	{
		if (str[idx] == '$')
			idx += (env_cnt(&(str[idx]), size, envp) + 1);
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
