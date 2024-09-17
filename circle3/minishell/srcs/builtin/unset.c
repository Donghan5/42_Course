/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:47:06 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 14:06:31 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to check identify is correct
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (ft_putstr_fd(UNSET_NOT_IDENTIFY, STDOUT), FAIL);
		i++;
	}
	return (SUCCESS);
}

// to delete key in the envp
static int	remove_env_var(t_glob_pipe *cmd, t_env *env)
{
	int		key_size;
	char	*key;
	int		i;

	key_size = size_env_key(cmd->args[1]);
	key = key_duplicate(cmd);
	key = cmd->args[1];
	i = 0;
	while (env->environ[i])
	{
		if (ft_strncmp(env->environ[i], key, key_size) == 0 && \
		(env->environ[i][key_size] == '=' || env->environ[i][key_size] == '\0'))
		{
			free(env->environ[i]);
			while (env->environ[i])
			{
				env->environ[i] = env->environ[i + 1];
				i++;
			}
			return (SUCCESS);
		}
		i++;
	}
	return (FAIL);
}

// goal --> delete value
int	unset(t_glob_pipe *cmd, t_env *env)
{
	char	*key;
	int		i;

	key = key_duplicate(cmd);
	if (check_identify_key(key) == FAIL)
		return (free(key), FAIL);
	if (remove_env_var(cmd, env) == FAIL)
		return (free(key), FAIL);
	free(key);
	return (SUCCESS);
}

// to check run cond and run func
int	unset_check(t_glob_pipe *cmd, t_env *env)
{
	if (!ft_strncmp(cmd->name, "unset", ft_strlen(cmd->name)))
		return (unset(cmd, env), RUN);
	return (NOT_RUN);
}
