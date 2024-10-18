/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:31:22 by donghank          #+#    #+#             */
/*   Updated: 2024/09/30 21:03:12 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// update the shlvl value, replace old one to new one
// so environ allocate in stack mem area
int	update_environ(t_env *env, char *key_value)
{
	char	**new_env;
	int		size_env;
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
	size_env = size_environ(env);
	new_env = (char **)malloc(sizeof(char *) * (size_env + 2));
	if (!new_env)
		return (FAIL);
	ft_memcpy(new_env, env->environ, sizeof(char *) * size_env);
	new_env[size_env] = ft_strdup(key_value);
	if (!new_env[size_env])
		return (free_doub_array(new_env), FAIL);
	new_env[size_env + 1] = NULL;
	if (env->environ)
		free_doub_array(env->environ);
	return (env->environ = new_env, SUCCESS);
}
