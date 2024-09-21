/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 21:23:26 by donghank          #+#    #+#             */
/*   Updated: 2024/09/21 16:09:19 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check identify (good characters in the key)
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (ft_putstr_fd(EXPORT_NOT_IDENTIFY, STDOUT), FAIL);
		i++;
	}
	return (SUCCESS);
}

static int	find_key_index(char **env_arr, char *key)
{
	int	i;

	i = 0;
	while (env_arr[i])
	{
		if (!(ft_strncmp(env_arr[i], key, ft_strlen(key))) && \
		env_arr[i][ft_strlen(key)] == '=')
			return (i);
		i++;
	}
	return (NOT_FOUND);
}

// to calcuate element of the double pointer array
static int	size_environ(t_env	*env)
{
	int	i;

	i = 0;
	while (env->environ[i])
		i++;
	return (i);
}

// to add and update new declaration
static int	update_env_array(t_env *env, char *key_value)
{
	char	**new_env;
	int		i;
	int		size_env;
	int		var_index;

	var_index = find_key_index(env->environ, key_value);
	if (var_index != NOT_FOUND)
		return (free(env->environ[var_index]), \
		env->environ[var_index] = ft_strdup(key_value), SUCCESS);
	size_env = size_environ(env);
	new_env = (char **)malloc(sizeof(char *) * (size_env + 2));
	if (!new_env)
		return (FAIL);
	i = 0;
	while (env->environ[i])
	{
		new_env[i] = env->environ[i];
		i++;
	}
	new_env[i] = ft_strdup(key_value);
	new_env[i + 1] = NULL;
	env->environ = new_env;
	return (SUCCESS);
}

// args --> associate with t_command->name
// in this function to facility, put the parameter to args(double pointer)
// in using term, with t_command->args
// parameters key=value, and index
int	ft_export(t_glob_pipe *cmd, t_env *env)
{
	int				i;
	char			*key;
	char			*value;
	char			*key_value;

	i = 0;
	if (!cmd->args[1] || !(ft_strrchr(cmd->args[1], (int) '=')))
		return (1);
	while (cmd->args[1][i] && cmd->args[1][i] != '=')
		i++;
	key = ft_substr(cmd->args[1], 0, i);
	if (check_identify_key(key))
		return (free(key), FAIL);
	i++;
	value = ft_strdup(&cmd->args[1][i]);
	key_value = triple_strjoin(key, "=", value);
	if (ft_strncmp(key, "SHLVL", 5) == 0)
		update_shlvl(key, value, env);
	else
		update_env_array(env, key_value);
	free(key);
	free(value);
	free(key_value);
	return (SUCCESS);
}
