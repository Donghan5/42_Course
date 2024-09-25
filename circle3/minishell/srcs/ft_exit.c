/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/25 23:09:36 by donghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// implement strcmp to compare digit
static int	exit_compare_tool(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

// exit action with exit_code num
// if theres not number in exit_code --> exit with stderr
// show the exit_code input(this is alphabetic)
static void	exit_with_num(t_glob_pipe *cmd, t_env *env)
{
	int	exit_code;

	exit_code = ft_atoi(cmd->args[1]);
	if (exit_code == 0 && exit_compare_tool(cmd->args[1], "0") != 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		env->status = 255;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		exit(env->status);
	}
	else
	{
		env->status = exit_code;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		printf("exit\n");
		exit(env->status);
	}
}

// avoid to exit error form
static void	exit_many_args(t_glob_pipe *cmd, t_env *env)
{
	ft_putendl_fd(EXIT_ERROR, STDOUT);
	env->status = 1;
	rl_clear_history();
	free_glob_pipe(&cmd);
	free_doub_array(env->environ);
	exit(env->status);
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
		printf("exit\n");
		exit(env->status);
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
