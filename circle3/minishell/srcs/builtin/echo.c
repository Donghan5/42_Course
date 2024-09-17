/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:30:11 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 15:09:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echocmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

int	echo(t_glob_pipe *cmd)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	if (cmd->args[1] && echocmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}

int	echo_check(t_glob_pipe *cmd, int *status)
{
	if (!ft_strncmp(cmd->name, "echo", ft_strlen(cmd->name)))
	{
		if (echo(cmd) == FAIL)
			*status = 1;
		return (RUN);
	}
	return (NOT_RUN);
}
