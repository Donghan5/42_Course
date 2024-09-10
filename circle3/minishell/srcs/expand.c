/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:29:47 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 23:25:54 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

// unclosed quote...? if that case str[idx] = '\0'

// In key=value return the length of the value
int	size_env_value(char *str, int size, char **envp)
{
	int	idx;

	idx = -1;
	while (envp[++idx])
	{
		if (!(ft_strncmp(&str[idx], envp, size)) && envp[idx][size] == '=')
			return (ft_strlen(envp[idx]) + size + 1);
	}
	return (0);
}

// calculate key=value key
// at the last idx is pointer after parameter so -1
int	size_env_key(char *str)
{
	int	idx;

	idx = 0;
	if (ft_isdigit(str[1]))
		return (1);
	while (str[++idx])
	{
		if (ft_isalnum(str[idx]) && str[idx] == '_')
			return (idx - 1);
	}
	return (idx - 1);
}

// to return env length
int	env_cnt(char *str, int *size, char **envp)
{
	int		idx;
	char	*status;

	if (str[1] == '?')
	{
		status = ft_itoa(g_exit_status);
		*size += ft_strlen(status);
		free(status);
		return (1);
	}
	if (str[1] == '\0' || str[1] == '\"')
		return (*size += 1, 0);
	idx = size_env_key(str);
	*size += size_env_value(str, idx, envp);
	return (idx);
}

// have to get the environ parse length
