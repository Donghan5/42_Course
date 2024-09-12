/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/13 00:11:52 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check identify
int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (ft_putstr_fd(NOT_IDENTIFY, STDOUT), FAIL);
		i++;
	}
	return (SUCCESS);
}

// add the declare -x in front of the paths
void	print_export(t_env_node *env_node)
{
	t_env_node	*tmp;
	char		**env_arr;
	int			i;
	int			j;

	env_arr = env_lst_to_array(env_node);
	if (!env_arr)
		return ;
	sort_env_arr(env_arr);
	i = 0;
	while (env_arr[i])
	{
		printf("declare -x %s\n", env_arr[i]);
		free(env_arr[i]);
	}
	free(env_arr);
}

// args --> associate with t_command->exec_name
// in this function to facility, put the parameter to args(double pointer)
// in using term, with t_command->args
// parameters key=value, and index
int	ft_export(t_command *cmd, t_env_node *env_node)
{
	int		i;
	char	*key;
	char	*value;
	char	*key_value;

	i = 0;
	if (!cmd->args[1] || !(ft_strrchr(cmd->args[1], (int) '=')))
		return (1);
	while (cmd->args[1][i] && cmd->args[1][i] != '=')
		i++;
	key = ft_substr(cmd->args[1], 0, i);
	if (check_identify_key(key))
		return (free(key), FAIL);
	i++;
	value = ft_strdup(&cmd->args[1][i + 1]);
	key_value = triple_strjoin(key, "=", value);
	update_new_env_var(key, value, env_node);
	new_env_node(key_value);
	free(key);
	free(value);
	free(key_value);
	return (SUCCESS);
}

int	export(t_command *cmd, t_env_node *env_node)
{
	if (!cmd->args[1])
		return (print_export(env_node), SUCCESS);
	if (ft_export(cmd, env_node) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
