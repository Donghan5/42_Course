/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/10/04 10:57:05 by donghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// exit action with exit_code num
// if theres not number in exit_code --> exit with stderr
// show the exit_code input(this is alphabetic)
static void	exit_with_num(t_glob_pipe *cmd, t_env *env)
{
	int	exit_code;
	int	err;

	err = 0;
	exit_code = ft_atoll(cmd->args[1], &err);
	if (err == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		env->sts = 2;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		exit(env->sts);
	}
	else
	{
		env->sts = exit_code;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		printf("exit\n");
		exit(env->sts);
	}
}

// avoid to exit error form
static void	exit_many_args(t_glob_pipe *cmd, t_env *env)
{
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	env->sts = 1;
	rl_clear_history();
	free_glob_pipe(&cmd);
	free_doub_array(env->environ);
	exit(env->sts);
}

// general exit cases
void	normal_exit_check(t_glob_pipe *cmd, t_env *env)
{
	if (cmd->args[1] && cmd->args[2])
		exit_many_args(cmd, env);
	else if (cmd->args[1])
		exit_with_num(cmd, env);
	else
	{
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		printf("exit\n");
		exit(env->sts);
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
