/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:33:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/20 16:40:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to dup the key
char	*get_key(t_glob_pipe *cmd)
{
	char	*key;
	int		key_size;
	int		i;

	if (cmd->args[1] == NULL)
		return (NULL);
	key_size = size_env_key(cmd->args[1]);
	key = ft_substr(cmd->args[1], 0, key_size);
	if (!key)
		return (NULL);
	return (key);
}

// get key and return the length of the key
int	getenv_key(char *src, char **key)
{
	int	key_len;

	key_len = size_env_key(src);
	*key = ft_substr(src, 1, key_len);
	return (key_len);
}

// to get the value
char	*getenv_value(char *key, char **envp)
{
	int		key_size;
	int		i;

	i = 0;
	key_size = ft_strlen(key);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_size) && envp[i][key_size] == '=')
			return (envp[i] + key_size + 1);
		i++;
	}
	return (NULL);
}
