/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:39:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/29 15:46:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_env *env)
{
	char			**env_arr;
	char			*key;
	char			*value;
	int				i;
	int				j;

	env_arr = env->environ;
	if (!env_arr)
		return ;
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		key = get_key_from_env(env_arr[i]);
		value = getenv_value(key, env_arr);
		printf("%s=%s\n", key, value);
		i++;
	}
}
