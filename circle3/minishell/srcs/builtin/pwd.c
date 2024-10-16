/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:25:41 by donghank          #+#    #+#             */
/*   Updated: 2024/10/16 20:37:30 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// built-in function pwd --> print the path
void	pwd_check(t_glob_pipe *cmd, int *status)
{
	int		buf;
	char	*cwd;

	(void)cmd;
	buf = 0;
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
	free(cwd);
}
