/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:23:59 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:13:05 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// when user want to direct to home
static void	go_to_home(t_env *env)
{
	if (chdir(getenv("HOME")) != 0)
	{
		env->sts = 1;
		ft_putstr_fd("minishell: cd: HOME is not set\n", 2);
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
		ft_putstr_fd("minishell: cd: ", 2);
		perror(cmd->args[1]);
	}
	free(path);
}

/*
	Update oldpwd env variable
*/
static void	update_oldpwd(char *oldpwd, t_env *env)
{
	char	*full_oldpwd;

	full_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	if (!full_oldpwd)
		return ;
	update_environ(env, full_oldpwd);
	free(full_oldpwd);
}

/*
	Update pwd env variable
*/
static void	update_pwd(t_env *env)
{
	char	*pwd;
	char	*full_pwd;

	pwd = getcwd(NULL, 0);
	full_pwd = ft_strjoin("PWD=", pwd);
	if (!full_pwd)
		return (free(pwd));
	update_environ(env, full_pwd);
	free(pwd);
	free(full_pwd);
}

void	cd_check(t_glob_pipe *cmd, t_env *env)
{
	char	*oldpwd;

	env->sts = 0;
	oldpwd = getcwd(NULL, 0);
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
			ft_putstr_fd("minishell: cd: ", 2);
			perror(cmd->args[1]);
		}
	}
	if (!env->sts)
		return (update_oldpwd(oldpwd, env), update_pwd(env), free(oldpwd));
	free(oldpwd);
}
