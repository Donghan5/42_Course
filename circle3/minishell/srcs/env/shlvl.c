/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:55:15 by donghank          #+#    #+#             */
/*   Updated: 2024/09/23 00:17:41 by donghank         ###   ########.fr       */
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
	char	*key_value;

	shlvl_str = ft_getenv("SHLVL", env);
	if (!shlvl_str)
		shell_lvl = 1;
	else
		shell_lvl = get_level(shlvl_str);
	new_shlvl_str = ft_itoa(shell_lvl + 1);
	if (!new_shlvl_str)
		exit_error(ALLOC_ERROR);
	key_value = triple_strjoin("SHLVL", "=", new_shlvl_str);
	if (update_env_array(env, key_value) == FAIL)
	{
		free(key_value);
		free(new_shlvl_str);
		printf("update fail");
	}
	printf("Updated SHLVL to: %s\n", new_shlvl_str);
	printf("Current SHLVL: %s\n", ft_getenv("SHLVL", env));
	free(key_value);
	free(new_shlvl_str);
}
