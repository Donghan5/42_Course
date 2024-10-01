/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:23:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/30 21:17:48 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// when user want to direct to home
static void	go_to_home(t_env *env)
{
	if (chdir(getenv("HOME")) != 0)
	{
		env->sts = 1;
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
}

// when user input the cd ~
static void	go_to_tilde(t_glob_pipe *cmd, t_env *env)
{
	char	*path;

	path = ft_strjoin(getenv("HOME"), (cmd->args[1] + 1));
	if (chdir(path) != 0)
	{
		env->sts = 1;
		perror("minishell: cd");
	}
	free(path);
}

// built-in cd function --> change directory
void	cd_check(t_glob_pipe *cmd, t_env *env)
{
	env->sts = 0;
	if (cmd->args[1] == NULL)
		go_to_home(env);
	else if (cmd->args[1][0] == '~' && getenv("HOME"))
		go_to_tilde(cmd, env);
	else if (cmd->args[1] && cmd->args[2])
	{
		env->sts = 1;
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	}
	else
	{
		if (chdir(cmd->args[1]) != 0)
		{
			env->sts = 1;
			perror("minishell: cd");
		}
	}
}
