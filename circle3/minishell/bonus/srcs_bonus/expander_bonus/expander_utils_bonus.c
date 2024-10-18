/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:20:36 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:13 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// to copy environ include double quote and dollar sign
int	env_copy_cnt(char *src, char **dest, t_env *env)
{
	int		src_idx;
	char	*key;
	char	*env_val;
	int		env_len;
	char	*status;

	src_idx = 0;
	if (src[1] == '?')
	{
		status = ft_itoa(env->sts);
		ft_memcpy(*dest, status, ft_strlen(status));
		*dest += ft_strlen(status);
		free(status);
		return (1);
	}
	if (src[1] == '\0' || src[1] == '\"' || ft_iswhitespace((int)src[1]))
		return (**dest = '$', *dest += 1, 0);
	src_idx = getenv_key(src, &key);
	env_val = getenv_value(key, env->environ);
	env_len = ft_strlen(env_val);
	ft_memcpy(*dest, env_val, env_len);
	*dest += env_len;
	free(key);
	return (src_idx);
}

// to copy inside of the double quote
int	double_quote_copy_cnt(char *src, char **dest, t_env *env)
{
	int	idx;

	idx = 1;
	while (src[idx] && src[idx] != '\"')
	{
		if (src[idx] == '$')
			idx += (env_copy_cnt(&(src[idx]), dest, env) + 1);
		else
		{
			**dest = src[idx];
			(*dest)++;
			idx++;
		}
	}
	return (idx);
}

// to copy inside of the single quote
int	single_quote_copy_cnt(char *src, char **dest)
{
	int	src_idx;
	int	dest_len;

	src_idx = 1;
	dest_len = 0;
	while (src[src_idx] && src[src_idx] != '\'')
	{
		src_idx++;
		dest_len++;
	}
	ft_memcpy(*dest, src + 1, dest_len);
	*dest += dest_len;
	return (src_idx);
}

// to do role of the expanded_str in double quote
// action like echo "Hello $USER" -> "Hello donghank(username)"
// have to think where we put this things
// in using input must be line(prompt), envp must be env.environ
void	copy_strings(char *input, char *dest, t_env *env)
{
	int		idx;
	char	*dest_end;

	idx = -1;
	dest_end = dest;
	while (input[++idx])
	{
		if (input[idx] == '\'' && check_unclosed_quote(&input[idx], '\''))
			idx += single_quote_copy_cnt(&input[idx], &dest_end);
		else if (input[idx] == '\"' && check_unclosed_quote(&input[idx], '\"'))
			idx += double_quote_copy_cnt(&input[idx], &dest_end, env);
		else if (input[idx] == '$')
			idx += env_copy_cnt(&input[idx], &dest_end, env);
		else
			*dest_end++ = input[idx];
	}
	*dest_end = '\0';
}
