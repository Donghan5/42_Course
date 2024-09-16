/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:23:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/16 11:27:14 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// built-in cd function --> change directory
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
