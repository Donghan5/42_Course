/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:30:11 by donghank          #+#    #+#             */
/*   Updated: 2024/09/30 21:18:38 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// print elememts all cmds
static void	print_cmds(t_glob_pipe *cmd, int i)
{
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
}

// Doing role of the echo
// treat multiple newline options
int	echo(t_glob_pipe *cmd)
{
	int		i;
	int		j;
	int		newline;

	i = 1;
	newline = 1;
	while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
	{
		j = 2;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
		{
			newline = 0;
			i++;
		}
		else
			break ;
	}
	print_cmds(cmd, i);
	if (newline)
		printf("\n");
	return (SUCCESS);
}

void	echo_check(t_glob_pipe *cmd, t_env *env)
{
	env->sts = 0;
	if (echo(cmd) == FAIL)
		env->sts = 1;
}
