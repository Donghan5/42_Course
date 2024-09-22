/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:31:22 by donghank          #+#    #+#             */
/*   Updated: 2024/09/23 00:15:30 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// adding the new name(key) and value
void	add_new_environ(t_env *env, char *name, char *value)
{
	t_name_value	*new_node;

	new_node = malloc(sizeof(t_name_value));
	if (!new_node)
		exit_error(ALLOC_ERROR);
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = env->environ_name_value;
	env->environ_name_value = new_node;
}

void	init_env(t_env *env)
{
	env->environ = NULL;
	env->environ_name_value = NULL;
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
