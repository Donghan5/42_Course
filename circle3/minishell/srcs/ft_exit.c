/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:16:59 by donghank          #+#    #+#             */
/*   Updated: 2024/09/29 15:38:04 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// converting the type of long int
static long int	ft_atoll(char *str, int *err)
{
	int							sign;
	int							i;
	int							j;
	unsigned long long int		res;

	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	j = i;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + (str[i++] - '0');
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] || i - j > 20 || ((sign == -1 && (res - 1) > LONG_MAX) || \
	(sign == 1 && (res > LONG_MAX))))
		*err = 1;
	return ((int)(sign * res) % 256);
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
	int	exit_code;
	int	err;

	err = 0;
	exit_code = ft_atoll(cmd->args[1], &err);
	if (err == 1)
	{
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
