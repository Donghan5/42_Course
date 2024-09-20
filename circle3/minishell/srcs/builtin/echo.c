/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:30:11 by donghank          #+#    #+#             */
/*   Updated: 2024/09/20 16:30:14 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// have to reduce the line following by the norm rule
int	echo(t_glob_pipe *cmd, t_env *env)
{
	int		i;
	int		newline;
	int		j;
	char	*expanded_arg;

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
	while (cmd->args[i])
	{
		expanded_arg = expander(cmd->args[i], env);
		if (expanded_arg)
		{
			printf("%s", expanded_arg);
			free(expanded_arg);
		}
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}

int	echo_check(t_glob_pipe *cmd, t_env *env, int *status)
{
	if (!ft_strncmp(cmd->name, "echo", ft_strlen(cmd->name)))
	{
		*status = 0;
		if (echo(cmd, env) == FAIL)
			*status = 1;
		return (RUN);
	}
	return (NOT_RUN);
}
