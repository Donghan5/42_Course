/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:24:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/21 14:04:05 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value_for_name(t_name_value *arr, char *name)
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

// to put the '~' in front of the cwd
char	*replace_home_tilde(char *cwd)
{
	char	*home;
	char	*tilde;
	int		home_len;

	home = getenv("HOME");
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0)
	{
		tilde = malloc(ft_strlen(cwd) - home_len + 2);
		if (!tilde)
			return (NULL);
		tilde[0] = '~';
		ft_strlcpy(tilde + 1, cwd + home_len, ft_strlen(cwd) - home_len + 1);
		free(cwd);
		return (tilde);
	}
	return (cwd);
}

// to get hostname in the /usr/hostname
char	*get_hostname(void)
{
	int		fd;
	char	*hostname;
	char	*newline_pos;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		exit_error("open");
	hostname = get_next_line(fd);
	close(fd);
	if (hostname)
	{
		newline_pos = ft_strchr(hostname, '.');
		if (newline_pos)
			*newline_pos = '\0';
	}
	return (hostname);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*temp;
	char	*cwd;
	char	*hostname;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	cwd = replace_home_tilde(cwd);
	hostname = get_hostname();
	if (!hostname)
		prompt = triple_strjoin(getenv("USER"), "@", "hostname");
	else
	{
		prompt = triple_strjoin(getenv("USER"), "@", hostname);
		free(hostname);
	}
	temp = triple_strjoin(prompt, ":", cwd);
	free(cwd);
	free(prompt);
	prompt = ft_strjoin(temp, "$ ");
	free(temp);
	return (prompt);
}
