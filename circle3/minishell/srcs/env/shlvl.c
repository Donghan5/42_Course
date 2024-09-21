/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:55:15 by donghank          #+#    #+#             */
/*   Updated: 2024/09/21 16:51:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to check the shlvl is adapatble cases
static int	is_valid_lvl(const char *str)
{
	int	idx;

	idx = 0;
	if (str[idx] == '-')
		idx++;
	while (str[idx])
	{
		if (!(str[idx] >= '0' && str[idx] <= '9'))
			return (NON_VALID);
		idx++;
	}
	return (VALID);
}

// to get the current shell level
static int	get_level(const char *str)
{
	int	num;
	int	sign;
	int	idx;

	idx = 0;
	num = 0;
	sign = 1;
	if (str[idx] == ' ' || (str[idx] >= 9 && str[idx] <= 13))
		idx++;
	if (is_valid_lvl(str) == NON_VALID)
		return (0);
	if (str[idx] == '+' || str[idx] == '-')
	{
		if (str[idx] == '-')
			sign *= -1;
		idx++;
	}
	while (str[idx] >= '0' && str[idx] <= '9')
	{
		num = num * 10 + (str[idx] - '0');
		idx++;
	}
	return (sign * num);
}

// goal : to increase the level of the shell
// note : using struct of the libft
void	increment_shell_level(t_env *env)
{
	char	*shlvl_str;
	int		shell_lvl;
	char	*new_shlvl_str;

	shlvl_str = getenv("SHLVL");
	printf("shlvl_str: %s\n", shlvl_str);
	if (!shlvl_str)
		shell_lvl = 1;
	else
		shell_lvl = get_level(shlvl_str);
	printf("current shell lvl: %d\n", shell_lvl);
	new_shlvl_str = ft_itoa(shell_lvl + 1);
	printf("new shlvl: %s\n", new_shlvl_str);
	if (!new_shlvl_str)
		exit_error(ALLOC_ERROR);
	if (update_new_env_var("SHLVL", new_shlvl_str, env) == UPDATED)
	{
		free(new_shlvl_str);
		printf("Successfully update: %s\n", getenv("SHLVL"));
	}
	else
	{
		free(new_shlvl_str);
		printf("upadate fail\n");
	}
}

void	update_shlvl(char *key, char *value, t_env *env)
{
	char	*new_value;
	int		lvl;

	lvl = ft_atoi(value) + 1;
	new_value = ft_itoa(lvl);
	update_new_env_var(key, new_value, env);
	free(new_value);
}
