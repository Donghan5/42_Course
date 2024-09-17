/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:33:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 14:06:13 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to dup the key
char	*key_duplicate(t_glob_pipe *cmd)
{
	char	*key_dup;
	int		key_size;
	int		i;

	key_size = size_env_key(cmd->args[1]);
	key_dup = (char *)malloc(sizeof(char) * (key_size + 1));
	if (!key_dup)
		return (NULL);
	i = 0;
	while (i < key_size)
	{
		key_dup[i] = cmd->args[1][i];
		i++;
	}
	key_dup[i] = '\0';
	return (key_dup);
}

// to get the value
char	*getenv_value(t_glob_pipe *cmd, char **envp)
{
	int		key_size;
	char	*key;
	int		i;

	key_size = size_env_key(cmd->args[1]);
	key = key_duplicate(cmd);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_size) && envp[i][key_size] == '=')
			return (envp[i] + key_size + 1);
		i++;
	}
	return (NULL);
}
