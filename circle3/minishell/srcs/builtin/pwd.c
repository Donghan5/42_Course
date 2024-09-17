/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:25:41 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 13:54:36 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// built-in function pwd --> print the path
int	pwd_check(t_glob_pipe *cmd, int *status)
{
	int		buf;
	char	*cwd;

	if (!ft_strncmp(cmd->name, "pwd", ft_strlen(cmd->name)))
	{
		cwd = getcwd(NULL, buf);
		if (!cwd)
		{
			*status = 1;
			perror("minishell: pwd");
		}
		else
		{
			*status = 0;
			printf("%s\n", cwd);
		}
		return (1);
	}
	return (0);
}
