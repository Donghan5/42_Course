/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 13:32:44 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	search_path_and_run(t_command *cmd, t_env *env, int fds[2])
{
	char	*path;
	char	**splitted;
	char	*full_path;
	int		i;
	int		executed;

	i = 0;
	executed = 0;
	path = getenv("PATH");
	if (access(cmd->exec_name, X_OK) == 0)
		execute_path(cmd->exec_name, cmd->args, env->environ, fds);
	else
	{
		splitted = ft_split(path, ':');
		while (splitted && splitted[i])
		{
			full_path = triple_strjoin(splitted[i], "/", cmd->exec_name);
			if (!full_path)
			{
				free_doub_array((void **) splitted);
				exit_error("malloc");
			}
			if (access(full_path, X_OK) == 0)
			{
				executed = 1;
				execute_path(full_path, cmd->args, env->environ, fds);
				free(full_path);
				break ;
			}
			i++;
			free(full_path);
		}
	}
	if (!executed)
	{
		write(2, cmd->exec_name, ft_strlen(cmd->exec_name));
		write(2, ": command not found\n", 20);
	}
	free_doub_array((void **) splitted);
}

int	execute_path(char *path, char **args, char **environ, int fds[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		free(path);
		free_doub_array((void **) args);
		exit_error("fork");
	}
	if (pid == 0)
	{
		setup_pipes(fds[0], fds[1]);
		if (execve(path, args, environ) == -1)
		{
			free(path);
			free_doub_array((void **) args);
			exit_error("execve");
		}
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
	}
	return (1);
}
