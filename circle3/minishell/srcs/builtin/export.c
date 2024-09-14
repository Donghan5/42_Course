/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:46:51 by donghank          #+#    #+#             */
/*   Updated: 2024/09/14 14:17:52 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check identify (good characters in the key)
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (ft_putstr_fd(EXPORT_NOT_IDENTIFY, STDOUT), FAIL);
		i++;
	}
	return (SUCCESS);
}

// add the declare -x in front of the paths
int	print_export(t_env *env)
{
	char			**env_arr;
	int				i;
	int				j;

	env_arr = env->environ;
	if (!env_arr)
		return (FAIL);
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		printf("declare -x %s\n", env_arr[i]);
		i++;
	}
	return (SUCCESS);
}

void	create_new_env_var_back(char *tok_str, t_env *env)
{
	t_name_value	*new_var;
	char			*new_var_str;

	new_var_str = ft_strdup(tok_str);
	if (!new_var_str)
		exit_error(ALLOC_ERROR);
	new_var = new_env_node((void *)new_var_str);
	if (!new_var)
		exit_error(ALLOC_ERROR);
	env_add_back(&(env->environ_name_value), new_var);
}

// args --> associate with t_command->exec_name
// in this function to facility, put the parameter to args(double pointer)
// in using term, with t_command->args
// parameters key=value, and index
int	ft_export(t_command *cmd, t_env *env)
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
	value = ft_strdup(&cmd->args[1][i]);
	key_value = triple_strjoin(key, "=", value);
	new_env_node(key_value);
	free(key);
	free(value);
	free(key_value);
	return (SUCCESS);
}

int	export(t_command *cmd, t_env *env)
{
	if (!cmd->args[1])
		return (print_export(env), SUCCESS);
	if (ft_export(cmd, env) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
