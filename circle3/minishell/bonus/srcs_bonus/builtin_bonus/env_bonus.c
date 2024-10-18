/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:39:51 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:59 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

void	ft_env(t_env *env)
{
	char			**env_arr;
	char			*key;
	char			*value;
	int				i;

	env_arr = env->environ;
	if (!env_arr)
		return ;
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		key = get_key_from_env(env_arr[i]);
		value = getenv_value(key, env_arr);
		if (value && value[0] != '\0')
			printf("%s=%s\n", key, value);
		i++;
		free(key);
	}
}

/*
	Print the error message for the unexpected option and arguments
*/
void	check_env(t_env *env, char **cmds)
{
	if (cmds[1] != NULL)
	{
		ft_putendl_fd("minishell: env: don't take option and arguments", 2);
		return ;
	}
	ft_env(env);
}
