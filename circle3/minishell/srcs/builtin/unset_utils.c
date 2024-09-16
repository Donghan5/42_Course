/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:33:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/16 11:34:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to dup the key
char	*key_duplicate(t_command *cmd)
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
char	*getenv_value(t_command *cmd, char **envp)
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
