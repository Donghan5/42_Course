/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 13:02:24 by donghank          #+#    #+#             */
/*   Updated: 2024/09/22 01:54:15 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// initialize all element in struct env
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

char	*get_key_from_env(char *env_str)
{
	int		key_size;
	char	*key;

	key_size = 0;
	while (env_str[key_size] && env_str[key_size] != '=')
		key_size++;
	key = (char *)malloc(sizeof(char) * (key_size + 1));
	if (!key)
		return (NULL);
	key = ft_substr(env_str, 0, key_size);
	return (key);
}

// why there is seg fault?
int	sync_env_with_sys(t_env *env)
{
	t_name_value	*node;
	extern char		**environ;
	int				i;
	int				size;

	size = 0;
	while (node)
	{
		size++;
		node = node->next;
	}
	// if (environ)
	// 	free_doub_array((void **)environ);
	// environ = malloc(sizeof(char *) * (size + 1));
	// if (!environ)
	// 	return (FAIL);
	node = env->environ_name_value;
	i = 0;
	while (node)
	{
		environ[i] = triple_strjoin(node->name, "=", node->value);
		if (!environ[i])
		{
			while (--i > 0)
				free(environ[i]);
			return (free(environ), environ = NULL, FAIL);
		}
		i++;
		node = node->next;
	}
	return (environ[i] = NULL, SUCCESS);
}
