/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 21:23:26 by donghank          #+#    #+#             */
/*   Updated: 2024/09/26 21:00:22 by donghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// check identify (good characters in the key)
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha((int)key[i]) && key[i] != '_')
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (FAIL);
	}
	i++;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(key, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

// to find key(specify) index
int	find_key_index(char **env_arr, char *key)
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
int	size_environ(t_env	*env)
{
	int	i;

	i = 0;
	while (env->environ[i])
		i++;
	return (i);
}

// to add and update new declaration
int	update_env_array(t_env *env, char *key_value)
{
	char	**new_env;
	int		i;
	int		size_env;
	int		var_index;

	var_index = find_key_index(env->environ, key_value);
	if (var_index != -1)
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
	int				j;
	char			*key;
	char			*value;
	char			*key_value;

	if (!cmd->args[1])
		return (FAIL);
	j = 1;
	while (cmd->args[j])
	{
		i = 0;
		while (cmd->args[j][i] && cmd->args[j][i] != '=')
			i++;
		key = ft_substr(cmd->args[j], 0, i);
		if (check_identify_key(key) == FAIL)
			return (free(key), FAIL);
		if (cmd->args[j][i] == '=')
			i++;
		value = ft_strdup(&cmd->args[j][i]);
		key_value = triple_strjoin(key, "=", value);
		update_env_array(env, key_value);
		free(key);
		free(value);
		free(key_value);
		j++;
	}
	return (SUCCESS);
}
