/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:33:38 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:34 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

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
