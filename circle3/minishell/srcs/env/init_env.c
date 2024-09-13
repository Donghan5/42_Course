/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:13:01 by donghank          #+#    #+#             */
/*   Updated: 2024/09/13 17:41:54 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	init_env(t_env *env, t_command *cmd)
// {
// 	char	**environ;
// 	char	*key;
// 	char	*value;
// 	t_env_node	*head;
// 	t_env_node	*curr;
// 	t_env_node	*new;
// 	int			i;

// 	environ = env->environ;
// 	head = NULL;
// 	curr = NULL;
// 	i = 0;
// 	while (environ[i])
// 	{
// 		key = key_duplicate(cmd);
// 		value = getenv_value(cmd, env->environ);
// 		create_new_env_var(value, &new);
// 		if (!new)
// 			return ;
// 		if (!head)
// 			head = new;
// 		else
// 			curr->next = new;
// 		curr = new;
// 		free(key);
// 		free(value);
// 		i++;
// 	}
// 	env->env_node = head;
// }
