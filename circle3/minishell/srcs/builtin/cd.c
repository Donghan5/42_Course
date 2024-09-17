/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:23:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 13:54:14 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// built-in cd function --> change directory
int	cd_check(t_glob_pipe *cmd, t_env *env, int *status)
{
	char	*path;

	if (!ft_strncmp(cmd->name, "cd", ft_strlen(cmd->name)))
	{
		*status = 0;
		if (cmd->args[1] == NULL)
		{
			if(chdir(getenv("HOME")) != 0)
			{
				*status = 1;
				perror("minishell: cd");
			}
		}
		else if (cmd->args[1][0] == '~')
		{
			path = ft_strjoin(getenv("HOME"), (cmd->args[1] + 1));
			if (chdir(path) != 0)
			{
				*status = 1;
				perror("minishell: cd");
			}
			free(path);
		}
		else
		{
			if (chdir(cmd->args[1]) != 0)
			{
				*status = 1;
				perror("minishell: cd");
			}
		}
		return (1);
	}
	return (0);
}
