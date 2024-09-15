/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/15 21:30:00 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export(t_command *cmd, t_env *env)
{
	if (!cmd->args[1])
		return (print_export(env), SUCCESS);
	if (ft_export(cmd, env) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
