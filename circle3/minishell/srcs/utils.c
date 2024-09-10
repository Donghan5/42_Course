/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:20:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 17:40:49 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}

void	free_doub_array(void **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free (arr[i]);
		i++;
	}
	if (arr)
		free (arr);
}

char	*triple_strjoin(char *s1, char *s2, char *s3)
{
	char	*new_str;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	new_str = ft_strjoin(tmp, s3);
	free (tmp);
	if (!new_str)
		return (NULL);
	return (new_str);
}
