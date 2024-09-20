/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:47:06 by donghank          #+#    #+#             */
/*   Updated: 2024/09/20 16:49:02 by donghank         ###   ########.fr       */
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

	key = cmd->args[1];
	key_size = ft_strlen(cmd->args[1]);
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

	key = cmd->args[1];
	if (check_identify_key(key) == FAIL)
		return (FAIL);
	if (remove_env_var(cmd, env) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

// to check run cond and run func
int	unset_check(t_glob_pipe *cmd, t_env *env, int *status)
{
	if (!ft_strncmp(cmd->name, "unset", ft_strlen(cmd->name)))
	{
		*status = 0;
		if (unset(cmd, env) == FAIL)
			*status = 1;
	}
	return (NOT_RUN);
}
