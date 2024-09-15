/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/15 15:16:30 by donghank         ###   ########.fr       */
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
// static int

// to add and update new declaration
static int	update_env_array(t_env *env, char *key_value)
{
	char	**new_env;
	int		i;
	int		var_index;

	var_index = find_key_index(env->environ, key_value);
	i = 0;
	if (var_index != NOT_FOUND)
	{
		free(env->environ[var_index]);
		env->environ[var_index] = ft_strdup(key_value);
		return (SUCCESS);
	}
	while (env->environ[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
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

// args --> associate with t_command->exec_name
// in this function to facility, put the parameter to args(double pointer)
// in using term, with t_command->args
// parameters key=value, and index
int	ft_export(t_command *cmd, t_env *env)
{
	int				i;
	char			*key;
	char			*value;
	char			*key_value;
	t_name_value	*new_var;

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
	// new_var = new_env_node(key_value);
	update_env_array(env, key_value);
	free(key);
	free(value);
	free(key_value);
	return (SUCCESS);
}

int	export(t_command *cmd, t_env *env)
{
	if (!cmd->args[1])
		return (print_export(env), SUCCESS);
	if (ft_export(cmd, env) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
