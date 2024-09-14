/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:47:06 by donghank          #+#    #+#             */
/*   Updated: 2024/09/13 14:31:41 by donghank         ###   ########.fr       */
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

// to check identify is correct
static int	check_identify_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (ft_putstr_fd(UNSET_NOT_IDENTIFY, STDOUT), FAIL);
		i++;
	}
	return (SUCCESS);
}

// to delete key in the envp
static int	remove_env_var(t_command *cmd, char **envp)
{
	int		key_size;
	char	*key;
	int		i;

	key_size = size_env_key(cmd->args[1]);
	key = key_duplicate(cmd);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_size) == 0 && \
		(envp[i][key_size] == '=' || envp[i][key_size] == '\0'))
		{
			free(envp[i]);
			while (envp[i])
			{
				envp[i] = envp[i + 1];
				i++;
			}
			return (SUCCESS);
		}
		i++;
	}
	return (FAIL);
}

// goal --> delete value
int	unset(t_command *cmd, char **envp)
{
	char	*key;
	int		i;

	key = key_duplicate(cmd);
	if (check_identify_key(key) == FAIL)
		return (free(key), FAIL);
	if (remove_env_var(cmd, envp) == FAIL)
		return (free(key), FAIL);
	free(key);
	return (SUCCESS);
}