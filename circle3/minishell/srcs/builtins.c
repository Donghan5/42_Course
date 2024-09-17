/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:27:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 15:54:21 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_check(t_glob_pipe *cmd)
{
	if ((!ft_strncmp(cmd->name, "pwd", ft_strlen(cmd->name))) \
		|| (!ft_strncmp(cmd->name, "exit", ft_strlen(cmd->name))) \
		|| (!ft_strncmp(cmd->name, "cd", ft_strlen(cmd->name))) \
		|| (!ft_strncmp(cmd->name, "export", ft_strlen(cmd->name))) \
		|| (!ft_strncmp(cmd->name, "unset", ft_strlen(cmd->name))) \
		|| (!ft_strncmp(cmd->name, "echo", ft_strlen(cmd->name))))
		return (RUN);
	return (NOT_RUN);
}

int	builtin_run(t_env *env, t_glob_pipe *cmd, int *status)
{
	normal_exit_check(cmd, status);
	if (pwd_check(cmd, status) || cd_check(cmd, env, status) || \
	export_check(cmd, env, status) || unset_check(cmd, env, status) || \
	echo_check(cmd, status))
		return (RUN);
	return (NOT_RUN);
}

