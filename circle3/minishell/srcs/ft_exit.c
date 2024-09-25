/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/25 17:49:56 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	normal_exit_check(t_glob_pipe *cmd, t_env *env)
{
	if (cmd->args[1] && cmd->args[2])
		ft_putstr_fd(EXIT_ERROR, STDOUT);
	else if (cmd->args[1] && str_is_alnum(cmd->args[1]) == 0)
		ft_putstr_fd(EXIT_NUM_ERROR, STDOUT);
	else if (cmd->args[1] && str_is_alnum(cmd->args[1]) == 1)
	{
		env->status = ft_atoi(cmd->args[1]);
		printf("exit\n");
		exit(env->status);
	}
	rl_clear_history();
	free_glob_pipe(&cmd);
	free_doub_array(env->environ);
	printf("exit\n");
	exit(env->status);
}

void	exit_error(char *perror_message)
{
	perror(perror_message);
	rl_clear_history();
	if (errno)
		exit(errno);
	exit(1);
}

int	str_is_alnum(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
