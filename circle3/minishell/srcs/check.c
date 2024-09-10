/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:27:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 17:38:32 by donghank         ###   ########.fr       */
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
		return (1);
	}
	return (0);
}

int	pwd_check(t_command *cmd)
{
	int	buf;

	if (!ft_strncmp(cmd->exec_name, "pwd", ft_strlen(cmd->exec_name)))
	{
		printf("%s\n", getcwd(NULL, buf));
		return (1);
	}
	return (0);
}

int	buildin_check(t_env *env, t_command	*cmd)
{
	normal_exit_check(cmd);
	if (pwd_check(cmd) || cd_check(cmd, env))
		return (1);
	return (0);
}
