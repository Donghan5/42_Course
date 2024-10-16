/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/10/13 22:19:00 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Exit with numeric arguments.
	If argument is not numeric it prints error and exits anyway.
*/
static void	exit_with_num(t_glob_pipe *cmd, t_env *env)
{
	int	exit_code;
	int	err;

	err = 0;
	exit_code = ft_atol(cmd->args[1], &err);
	if (err == 1)
	{
		if (env->is_interactive)
			printf("exit\n");
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		env->sts = 2;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		exit(env->sts);
	}
	env->sts = exit_code;
	rl_clear_history();
	free_glob_pipe(&cmd);
	free_doub_array(env->environ);
	if (env->is_interactive)
		printf("exit\n");
	exit(env->sts);
}

static void	exit_many_args(t_glob_pipe *cmd, t_env *env)
{
	if (env->is_interactive)
		printf("exit\n");
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	env->sts = 1;
	rl_clear_history();
	free_glob_pipe(&cmd);
	free_doub_array(env->environ);
	exit(env->sts);
}

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
		if (env->is_interactive)
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
