/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/28 16:36:34 by donghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// to convert to long long type
static long long int	ft_atoll(char *str)
{
	int					sign;
	long long int		res;
	long long int		prev_res;

	sign = 1;
	res = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		prev_res = res;
		res = res * 10 + (*str++ - '0');
		if ((sign == 1 && res < prev_res) || (sign == -1 && res > prev_res))
			return (0);
	}
	return (sign * res);
}

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
	long long int	exit_code;

	exit_code = ft_atoll(cmd->args[1]);
	if (exit_code == 0 && exit_compare_tool(cmd->args[1], "0") != 0)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		env->status = 2;
		rl_clear_history();
		free_glob_pipe(&cmd);
		free_doub_array(env->environ);
		exit(env->status);
	}
	else
	{
		env->status = exit_code % 256;
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
	ft_putendl_fd("minishell: exit: too many arguments", 2);
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
