/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:31:22 by donghank          #+#    #+#             */
/*   Updated: 2024/09/22 15:30:37 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// syncronize the environ value (apply the updated)
void	sync_env(t_env *env)
{
	char			**new_environ;
	char			**old_environ;
	extern char		**environ;
	t_name_value	*node;
	char			*key_value;
	int				i;

	i = 0;
	node = env->environ_name_value;
	while (node)
	{
		i++;
		node = node->next;
	}
	new_environ = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_environ)
		exit_error(ALLOC_ERROR);
	node = env->environ_name_value;
	i = 0;
	while (node)
	{
		key_value = malloc(ft_strlen(node->name) + ft_strlen(node->value) + 2);
		if (!key_value)
			exit_error(ALLOC_ERROR);
		key_value = ft_strdup(node->name);
		printf("middle key_value: %s\n", key_value);
		key_value = triple_strjoin(key_value, "=", node->value);
		printf("final key_value: %s\n", key_value);
		printf("new_environ status: %s\n", new_environ[i]);
		new_environ[i] = key_value;
		node = node->next;
		i++;
	}
	new_environ[i] = NULL;
	old_environ = env->environ;
	env->environ = new_environ;
	environ = new_environ;
	if (old_environ)
		free_doub_array((void **)old_environ);
}

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
