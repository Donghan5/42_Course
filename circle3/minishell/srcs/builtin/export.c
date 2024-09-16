/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/16 12:49:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export function (! if theres not a key, just print)
int	export(t_command *cmd, t_env *env)
{
	if (!cmd->args[1])
		return (print_export(env), SUCCESS);
	if (ft_export(cmd, env) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

// to check and run export func
int	export_check(t_command *cmd, t_env *env)
{
	if (!ft_strncmp(cmd->exec_name, "export", ft_strlen(cmd->exec_name)))
		return (export(cmd, env), RUN);
	return (NOT_RUN);
}
