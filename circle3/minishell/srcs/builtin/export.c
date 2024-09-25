/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/25 16:43:26 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export function (! if theres not a key, just print)
void	export(t_glob_pipe *cmd, t_env *env)
{
	env->status = 0;
	if (!cmd->args[1])
	{
		env->status = !print_export(env);
		return ;
	}
	if (ft_export(cmd, env) == FAIL)
		env->status = 1;
}
