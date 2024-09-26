/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:47:06 by donghank          #+#    #+#             */
/*   Updated: 2024/09/26 12:23:18 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// to check identify is correct
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (printf(UNSET_NOT_IDENTIFY), FAIL);
		i++;
	}
	return (SUCCESS);
}

// to delete key in the environ value
// please sure of the double free
static void	remove_env_var(t_glob_pipe *cmd, t_env *env)
{
	int		key_size;
	char	*key;
	int		i;
	int		j;

	key = cmd->args[1];
	key_size = ft_strlen(key);
	i = 0;
	while (env->environ[i])
	{
		if (ft_strncmp(env->environ[i], key, key_size) == 0 && \
		(env->environ[i][key_size] == '=' || env->environ[i][key_size] == '\0'))
		{
			free(env->environ[i]);
			env->environ[i] = NULL;
			j = 1;
			while (env->environ[j])
			{
				env->environ[j] = env->environ[j + 1];
				j++;
			}
			env->environ[j] = NULL;
			break ;
		}
		i++;
	}
}

// goal --> delete value
int	unset(t_glob_pipe *cmd, t_env *env)
{
	char	*key;
	int		i;

	i = 1;
	while (cmd->args[i])
	{
		key = get_key_from_env(cmd->args[i]);
		if (check_identify_key(key) == FAIL)
		{
			free(key);
			env->status = 1;
			i++;
			return (FAIL);
		}
		remove_env_var(cmd, env);
		free(key);
		i++;
	}
	return (SUCCESS);
}

// to check run cond and run func
void	unset_check(t_glob_pipe *cmd, t_env *env)
{
	env->status = 0;
	if (unset(cmd, env) == FAIL)
		env->status = 1;
}
