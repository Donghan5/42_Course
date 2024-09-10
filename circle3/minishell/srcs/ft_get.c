/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:24:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 13:30:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value_for_name(t_name_value	*arr, char *name)
{
	int	i;

	i = 0;
	while (arr[i].name)
	{
		printf("%s~%s\n", arr[i].name, arr[i].value);
		if (!ft_strncmp(arr[i].name, name, ft_strlen(arr[i].name)))
		{
			printf("FOUND\n");
			return (arr[i].value);
		}
		i++;
	}
	return (NULL);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*temp;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	prompt = triple_strjoin(getenv("USER"), "@", getenv("HOSTNAME"));
	temp = triple_strjoin(prompt, ":", cwd);
	free(cwd);
	free(prompt);
	prompt = ft_strjoin(temp, "$ ");
	free(temp);
	return (prompt);
}
