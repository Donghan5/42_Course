/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:27:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/13 16:10:20 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_check(t_command	*cmd, t_env *env)
{
	char	*path;

	if (!ft_strncmp(cmd->exec_name, "cd", ft_strlen(cmd->exec_name)))
	{
		if (cmd->args[1] == NULL)
		{
			if (chdir(getenv("HOME")) != 0)
				perror("cd");
		}
		else if (cmd->args[1][0] == '~')
		{
			path = ft_strjoin(getenv("HOME"), (cmd->args[1] + 1));
			if (chdir(path) != 0)
				perror("cd");
			free(path);
		}
		else
		{
			if (chdir(cmd->args[1]) != 0)
				perror("cd");
		}
		return (RUN);
	}
	return (NOT_RUN);
}

int	pwd_check(t_command *cmd)
{
	int	buf;

	if (!ft_strncmp(cmd->exec_name, "pwd", ft_strlen(cmd->exec_name)))
	{
		printf("%s\n", getcwd(NULL, buf));
		return (RUN);
	}
	return (NOT_RUN);
}

int	builtin_check(t_command	*cmd)
{
	if ((!ft_strncmp(cmd->exec_name, "pwd", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "exit", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "cd", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "export", ft_strlen(cmd->exec_name))) \
		|| (!ft_strncmp(cmd->exec_name, "unset", ft_strlen(cmd->exec_name))))
		return (RUN);
	return (NOT_RUN);
}

int	export_check(t_command *cmd, t_env *env)
{
	if (!ft_strncmp(cmd->exec_name, "export", ft_strlen(cmd->exec_name)))
	{
		return (export(cmd, env), RUN);
	}
	return (NOT_RUN);
}

int	unset_check(t_command *cmd, char **envp)
{
	if (!ft_strncmp(cmd->exec_name, "unset", ft_strlen(cmd->exec_name)))
	{
		return (unset(cmd, envp), RUN);
	}
	return (NOT_RUN);
}


int	builtin_run(t_env *env, t_command *cmd)
{
	// init_env(env, cmd);
	normal_exit_check(cmd);
	if (pwd_check(cmd) || cd_check(cmd, env) || export_check(cmd, env) || unset_check(cmd, env->environ))
		return (RUN);
	return (NOT_RUN);
}

