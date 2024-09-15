/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 23:47:42 by donghank          #+#    #+#             */
/*   Updated: 2024/09/15 14:14:39 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
			return (free_doub_array((void **)env_arr), NULL);
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

static int	array_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

// sorting the array
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
