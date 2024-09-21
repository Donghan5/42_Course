/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 23:12:58 by donghank          #+#    #+#             */
/*   Updated: 2024/09/21 17:01:25 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// create_new_env_var --> goal : to create new env_var
void	create_new_env_var(char *tok_str, t_env *env)
{
	t_name_value	*new_var;
	char			*new_var_str;

	new_var_str = ft_strdup(tok_str);
	if (!new_var_str)
		exit_error(ALLOC_ERROR);
	new_var = new_env_node((void *)new_var_str);
	if (!new_var)
		exit_error(ALLOC_ERROR);
	env_add_front(&(env->environ_name_value), new_var);
}

// swap_new_env_var --> goal : to replace env_var
// to using the cur_env_ptr --> to de-allocate current env value
char	*swap_new_env_var(t_env *env, char *var_name, char *new_var)
{
	char			*fin_env;
	t_name_value	*node;
	int				var_name_len;

	var_name_len = ft_strlen(var_name);
	node = env->environ_name_value;
	while (node)
	{
		if (ft_strncmp(node->name, var_name, var_name_len) == 0)
		{
			if (node->value)
			{
				fin_env = triple_strjoin(var_name, "=", new_var);
				if (!fin_env)
					return (printf("MEM ALLOC FAIL\n"), NULL);
				// free(node->value);
				node->value = fin_env;
				printf("fin env is: %s\n", fin_env);
				return (fin_env);
			}
		}
		node = node->next;
	}
	return (NULL);
}

// update_new_env_var --> goal : to apply replaced env_var to current_var
// example var=value - update_new_env_var -> val=new_value
int	update_new_env_var(char *var, char *new_val, t_env *env)
{
	t_name_value	*node;
	t_name_value	*new_node;
	int				var_len;

	var_len = ft_strlen(var);
	node = env->environ_name_value;
	while (node)
	{
		// printf("name: %s, cmp: %d, =:%d \n", node->name, ft_strncmp(node->name, var, var_len) == 0, node->name[var_len] == '=');

		if ((ft_strncmp(node->name, var, var_len) == 0))
		{
			printf("change: %s\n", node->name);
			free(node->value);
			node->value = swap_new_env_var(env, var, new_val);
			printf("va: %s\n", node->value);
			if (!node->value)
				return (NOT_UPDATED);
			return (UPDATED);
		}
		node = node->next;
	}
	return (NOT_UPDATED);
}

// add the environ node at front
void	env_add_front(t_name_value **env_node, t_name_value *new_node)
{
	new_node->next = *env_node;
	*env_node = new_node;
}

// make a new node(data) to environ
t_name_value	*new_env_node(char *content)
{
	t_name_value	*new_node;
	char			*equal_sign;

	new_node = malloc(sizeof(t_name_value));
	if (!new_node)
		return (NULL);
	new_node->value = NULL;
	new_node->next = NULL;
	new_node->name = NULL;
	if (content)
	{
		equal_sign = ft_strchr(content, '=');
		if (equal_sign)
		{
			new_node->name = ft_substr(content, 0, equal_sign - content);
			if (!new_node->name)
				return (free(new_node), NULL);
		}
		new_node->value = ft_strdup(content);
		if (!new_node->value)
			return (free(new_node->name), free(new_node), NULL);
	}
	return (new_node);
}
