/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:24:38 by donghank          #+#    #+#             */
/*   Updated: 2024/10/11 22:35:01 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// to get the value for the name
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

// to get the username
char	*get_username(void)
{
	char	*username;

	username = getenv("USER");
	if (!username)
		username = "username";
	return (username);
}

// to put the '~' in front of the cwd
char	*replace_home_tilde(char *cwd)
{
	char	*home;
	char	*tilde;
	int		home_len;

	home = getenv("HOME");
	if (!home)
		return (cwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0)
	{
		tilde = malloc(ft_strlen(cwd) - home_len + 2);
		if (!tilde)
			return (cwd);
		tilde[0] = '~';
		ft_strlcpy(tilde + 1, cwd + home_len, ft_strlen(cwd) - home_len + 1);
		free(cwd);
		return (tilde);
	}
	return (cwd);
}

// to get the hostname and concern . and \n
char	*get_hostname(void)
{
	int		fd;
	char	*hostname;
	char	*current;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		exit_error("open");
	hostname = get_next_line(fd);
	close(fd);
	if (hostname)
	{
		current = hostname;
		while (*current)
		{
			if (*current == '\n' || *current == '.')
			{
				*current = '\0';
				break ;
			}
			current++;
		}
	}
	return (hostname);
}

// to get the prompt interface showing the username, hostname, cwd
char	*get_prompt(void)
{
	char	*prompt;
	char	*temp;
	char	*cwd;
	char	*hostname;
	char	*username;

	cwd = getcwd(NULL, 0);
	username = get_username();
	if (!cwd)
		return (NULL);
	cwd = replace_home_tilde(cwd);
	hostname = get_hostname();
	if (!hostname)
		prompt = triple_strjoin(username, "@", "default_hostname");
	else
	{
		prompt = triple_strjoin(username, "@", hostname);
		free(hostname);
	}
	temp = triple_strjoin(prompt, ":", cwd);
	free(cwd);
	free(prompt);
	prompt = ft_strjoin(temp, "$ ");
	free(temp);
	return (prompt);
}
