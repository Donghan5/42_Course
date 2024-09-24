/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:27:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/24 17:14:43 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_check(t_glob_pipe *cmd)
{
	if ((!ft_strncmp(cmd->name, "exit", 5)) \
		|| (!ft_strncmp(cmd->name, "pwd", 4)) \
		|| (!ft_strncmp(cmd->name, "cd", 3)) \
		|| (!ft_strncmp(cmd->name, "export", 7)) \
		|| (!ft_strncmp(cmd->name, "unset", 6)) \
		|| (!ft_strncmp(cmd->name, "echo", 5)))
		return (RUN);
	return (NOT_RUN);
}

void	builtin_run(t_env *env, t_glob_pipe *cmd)
{
	if (!ft_strncmp(cmd->name, "exit", 5))
		normal_exit_check(cmd, env);
	if (!ft_strncmp(cmd->name, "pwd", 4))
		pwd_check(cmd, env->status);
	if (!ft_strncmp(cmd->name, "cd", 3))
		cd_check(cmd, env);
	if (!ft_strncmp(cmd->name, "export", 7))
		export(cmd, env);
	if (!ft_strncmp(cmd->name, "unset", 6))
		unset_check(cmd, env);
	if (!ft_strncmp(cmd->name, "echo", 5))
		echo_check(cmd, env);
}
