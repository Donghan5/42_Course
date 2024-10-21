/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:15:16 by donghank          #+#    #+#             */
/*   Updated: 2024/10/19 20:42:02 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static int	env_cnt_heredoc(char *str, int *size, t_env *env)
{
	int		idx;
	char	*status;
	int		len;

	(void)status;
	(void)size;
	if (str[1] == '\0' || str[1] == '\"' || str[1] == '\'')
		return (1);
	idx = size_env_key(str);
	len = size_env_value(str, idx, env->environ);
	return (len);
}

static int	calculate_expanded_len(char *cmd, t_env *env)
{
	int		idx;
	int		size;

	idx = 0;
	size = 0;
	while (cmd[idx])
	{
		if (cmd[idx] == '$')
			size += env_cnt_heredoc(&cmd[idx], &size, env);
		else if (cmd[idx] != '\'' || cmd[idx] != '\"')
			size++;
		idx++;
	}
	return (size);
}

// to copy environ include double quote and dollar sign
static int	env_copy_cnt_hdoc(char *src, char **dest, t_env *env)
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
	if (src[1] == '\0' || ft_iswhitespace((int)src[1]) \
	|| src[1] == '\"' || src[1] == '\'')
		return (**dest = '$', *dest += 1, 0);
	src_idx = getenv_key(src, &key);
	env_val = getenv_value(key, env->environ);
	env_len = ft_strlen(env_val);
	ft_memcpy(*dest, env_val, env_len);
	*dest += env_len;
	free(key);
	return (src_idx);
}

static void	copy_strings_hdoc(char *input, char *dest, t_env *env)
{
	int		idx;
	char	*dest_end;

	idx = -1;
	dest_end = dest;
	while (input[++idx])
	{
		if (input[idx] == '$')
			idx += env_copy_cnt_hdoc(&input[idx], &dest_end, env);
		else
			*dest_end++ = input[idx];
	}
	*dest_end = '\0';
}

/*
	expander in heredoc mode
	@param cmd, env
*/
char	*expander_heredoc(char *cmd, t_env *env)
{
	int		cmd_len;
	char	*expanded;
	int		idx;
	int		size;
	char	*env_val;

	idx = -1;
	size = 0;
	(void)env_val;
	cmd_len = calculate_expanded_len(cmd, env);
	expanded = ft_calloc((cmd_len + 1), sizeof(char));
	if (!expanded)
		return (NULL);
	copy_strings_hdoc(cmd, expanded, env);
	return (expanded);
}
