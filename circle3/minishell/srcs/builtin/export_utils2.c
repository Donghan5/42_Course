/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 21:23:26 by donghank          #+#    #+#             */
/*   Updated: 2024/09/29 16:08:50 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
