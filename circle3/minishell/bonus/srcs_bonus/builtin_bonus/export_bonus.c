/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:55 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// check identify (good characters in the key)
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha((int)key[i]) && key[i] != '_')
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (FAIL);
	}
	i++;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(key, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

// helper function process of the export
static void	process_of_export(char *arg, t_env *env, int *exit_status)
{
	int		i;
	char	*key;
	char	*key_value;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = ft_substr(arg, 0, i);
	if (check_identify_key(key) == FAIL)
	{
		*exit_status = FAIL;
		free(key);
		return ;
	}
	if (arg[i] == '=')
		i++;
	key_value = triple_strjoin(key, "=", arg + i);
	update_env_array(env, key_value);
	free(key);
	free(key_value);
}

int	ft_export(t_glob_pipe *cmd, t_env *env)
{
	int	j;
	int	exit_status;

	j = 1;
	exit_status = SUCCESS;
	while (cmd->args[j])
	{
		process_of_export(cmd->args[j], env, &exit_status);
		j++;
	}
	return (exit_status);
}

// export function (! if theres not a key, just print)
void	export(t_glob_pipe *cmd, t_env *env)
{
	env->sts = 0;
	if (!cmd->args[1])
	{
		env->sts = !print_export(env);
		return ;
	}
	if (ft_export(cmd, env) == FAIL)
		env->sts = 1;
}
