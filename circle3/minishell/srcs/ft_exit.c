/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/24 17:15:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	normal_exit_check(t_glob_pipe *cmd, t_env *env)
{
	if (!ft_strncmp(cmd->name, "exit", ft_strlen(cmd->name)))
	{
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array((void **)env->environ);
		printf("exit\n");
		exit(*env->status);
	}
}

void	exit_error(char *perror_message)
{
	perror(perror_message);
	rl_clear_history();
	if (errno)
		exit(errno);
	exit(1);
}
