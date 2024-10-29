/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tool_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:31:22 by donghank          #+#    #+#             */
/*   Updated: 2024/10/25 18:57:38 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// adding the new name(key) and value
void	add_new_environ(t_env *env, char *name, char *value)
{
	t_name_value	*new_node;

	new_node = malloc(sizeof(t_name_value));
	if (!new_node)
		return ;
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = env->environ_name_value;
	env->environ_name_value = new_node;
}

// initialize all element in t_name_value
t_name_value	*new_node_value(void)
{
	t_name_value	*new_node;

	new_node = (t_name_value *)malloc(sizeof(t_name_value));
	if (!new_node)
		return (NULL);
	new_node->name = NULL;
	new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

// to get environ semi-automatic
char	*ft_getenv(const char *name, t_env *env)
{
	int	i;
	int	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (env->environ[i])
	{
		if (ft_strncmp(env->environ[i], name, name_len) == 0 && \
		env->environ[i][name_len] == '=')
			return (env->environ[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

char	**copy_envs(t_env *env, char *key_value)
{
	char	**new_env;
	int		size_env;
	int		i;

	size_env = size_environ(env);
	new_env = (char **)malloc(sizeof(char *) * (size_env + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env->environ[i])
	{
		new_env[i] = ft_strdup(env->environ[i]);
		if (!new_env[i])
			return (free_doub_array(new_env), NULL);
		i++;
	}
	new_env[size_env] = ft_strdup(key_value);
	if (!new_env[size_env])
		return (free_doub_array(new_env), NULL);
	new_env[size_env + 1] = NULL;
	return (new_env);
}

/*
	update the shlvl value, replace old one to new one
	so environ allocate in stack mem area
*/
int	update_environ(t_env *env, char *key_value)
{
	char	**new_env;
	int		var_i;
	char	*key;

	key = ft_substr(key_value, 0, ft_strchr(key_value, '=') - key_value);
	if (!key)
		return (FAIL);
	var_i = find_key_index(env->environ, key);
	free(key);
	if (var_i != NOT_FOUND)
		return (free(env->environ[var_i]), \
		env->environ[var_i] = ft_strdup(key_value), SUCCESS);
	new_env = copy_envs(env, key_value);
	if (!new_env)
		return (FAIL);
	if (env->environ)
		free_doub_array(env->environ);
	return (env->environ = new_env, SUCCESS);
}
