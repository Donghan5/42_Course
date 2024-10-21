/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 23:47:42 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 22:12:47 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

// add the declare -x in front of the paths
int	print_export(t_env *env)
{
	char			**env_arr;
	char			*key;
	char			*value;
	int				i;

	env_arr = env->environ;
	if (!env_arr)
		return (0);
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		key = get_key_from_env(env_arr[i]);
		value = getenv_value(key, env_arr);
		if (value && value[0] != '\0')
			printf("declare -x %s=\"%s\"\n", key, value);
		else
			printf("declare -x %s\n", key);
		i++;
		free(key);
	}
	return (1);
}

// to calculate the size of array
int	env_list_size(t_name_value *env_node)
{
	t_name_value	*begin_node;
	int				count;

	count = 0;
	begin_node = env_node;
	while (begin_node)
	{
		count++;
		begin_node = begin_node->next;
	}
	return (count);
}

// convert environ path to array
char	**env_lst_to_array(t_name_value *env)
{
	char	**env_arr;
	int		size;
	int		i;

	i = 0;
	size = env_list_size(env);
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_arr)
		return (NULL);
	while (env)
	{
		env_arr[i] = ft_strdup(env->value);
		if (!env_arr[i])
			return (free_doub_array(env_arr), NULL);
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

// to calculate which is bigger (base on ascii)
static int	array_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

// sorting the array by ascending order (by using array_cmp)
void	sort_env_array(char **env_arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env_arr[i])
	{
		j = i + 1;
		while (env_arr[j])
		{
			if (array_cmp(env_arr[i], env_arr[j]) > 0)
			{
				tmp = env_arr[i];
				env_arr[i] = env_arr[j];
				env_arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
