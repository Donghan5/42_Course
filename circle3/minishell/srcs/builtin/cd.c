/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:23:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/24 16:59:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// built-in cd function --> change directory
void	cd_check(t_glob_pipe *cmd, t_env *env)
{
	char	*path;

	*env->status = 0;
	if (cmd->args[1] == NULL)
	{
		if (chdir(getenv("HOME")) != 0)
		{
			*env->status = 1;
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			// perror("minishell: cd");
		}
	}
	else if (cmd->args[1][0] == '~' && getenv("HOME"))
	{
		path = ft_strjoin(getenv("HOME"), (cmd->args[1] + 1));
		if (chdir(path) != 0)
		{
			*env->status = 1;
			perror("minishell: cd");
		}
		free(path);
	}
	else
	{
		if (chdir(cmd->args[1]) != 0)
		{
			*env->status = 1;
			perror("minishell: cd");
		}
	}
}
