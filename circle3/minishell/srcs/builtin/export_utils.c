/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 23:47:42 by donghank          #+#    #+#             */
/*   Updated: 2024/09/13 00:07:36 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to calculate the size of array
int	env_list_size(t_env_node *env_node)
{
	if (!env_node)
		return (1);
	return (env_list_size(env_node->next) + 1);
}

// convert environ path to array
char	**env_lst_to_array(t_env_node *env_node)
{
	char	**env_arr;
	int		size;
	int		i;

	i = 0;
	size = env_list_size(env_node);
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_arr)
		return (NULL);
	while (env_node)
	{
		env_arr[i] = ft_strdup(env_node->content);
		if (!env_arr[i])
			return (free_doub_array(env_arr), NULL);
		env_node = env_node->next;
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
