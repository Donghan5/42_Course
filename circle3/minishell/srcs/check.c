/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:27:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 13:11:51 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_check(t_command	*cmd)
{
	if ((!ft_strncmp(cmd->exec_name, "pwd", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "exit", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "cd", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "export", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "unset", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "echo", ft_strlen(cmd->exec_name))))
		return (RUN);
	return (NOT_RUN);
}




int	builtin_run(t_env *env, t_command *cmd)
{
	normal_exit_check(cmd);
	if (pwd_check(cmd) || cd_check(cmd, env) || \
	export_check(cmd, env) || unset_check(cmd, env) || \
	echo_check(cmd))
		return (RUN);
	return (NOT_RUN);
}

