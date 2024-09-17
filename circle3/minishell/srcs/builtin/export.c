/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 14:45:40 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export function (! if theres not a key, just print)
void	export(t_glob_pipe *cmd, t_env *env, int *status)
{
	*status = 0;
	if (!cmd->args[1])
	{
		*status = !print_export(env);
		return ;
	}
	if (ft_export(cmd, env) == FAIL)
		*status = 1;
}

// to check and run export func
int	export_check(t_glob_pipe *cmd, t_env *env, int *status)
{
	if (!ft_strncmp(cmd->name, "export", ft_strlen(cmd->name)))
		return (export(cmd, env, status), RUN);
	return (NOT_RUN);
}
