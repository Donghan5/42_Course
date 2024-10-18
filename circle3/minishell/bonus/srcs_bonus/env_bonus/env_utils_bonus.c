/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:29:47 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:28 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// In key=value return the length of the value
// size is the size of the key(str)
int	size_env_value(char *str, int size, char **envp)
{
	int	idx;

	idx = -1;
	while (envp[++idx])
	{
		if (!ft_strncmp(&str[1], envp[idx], size) && envp[idx][size] == '=')
			return (ft_strlen(envp[idx] + size + 1));
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
		if (!(ft_isalnum(str[idx]) || str[idx] == '_'))
			return (idx - 1);
	}
	return (idx - 1);
}

// to return env length
// when meet ? --> exit(quit...?) the program
int	env_cnt(char *str, int *size, t_env *env)
{
	int		idx;
	char	*status;

	idx = 0;
	if (str[1] == '?')
	{
		status = ft_itoa(env->sts);
		*size += ft_strlen(status);
		free(status);
		return (1);
	}
	if (str[1] == '\0' || str[1] == '\"')
	{
		*size += 1;
		return (0);
	}
	idx = size_env_key(str);
	*size += size_env_value(str, idx, env->environ);
	return (idx);
}

// to get parsed len of the env
int	get_env_parse_len(char *str, t_env *env)
{
	int	idx;
	int	size;

	idx = -1;
	size = 0;
	while (str[++idx])
	{
		if (str[idx] == '\'' && check_unclosed_quote(str, '\''))
			size += single_quote_cnt(&str[idx], &size);
		else if (str[idx] == '\"' && check_unclosed_quote(str, '\"'))
			size += double_quote_cnt(&str[idx], &size, env);
		else if (str[idx] == '$')
			size += env_cnt(&str[idx], &size, env);
		else if (str[idx])
			size++;
	}
	return (size);
}

// to search the key(name)
char	*get_key_from_env(char *env_str)
{
	int		key_size;
	char	*key;

	key_size = 0;
	while (env_str[key_size] && env_str[key_size] != '=')
		key_size++;
	key = ft_substr(env_str, 0, key_size);
	return (key);
}
