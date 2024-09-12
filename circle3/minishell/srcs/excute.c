/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 14:57:30 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_g_pipe *g)
{
	int	i;

	i = 0;
	while (i < g->close_count)
	{
		close(g->files_to_close[i]);
		i++;
	}
}

void search_path_and_run(t_g_pipe *g, t_env *env)
{
    char *path;
    char **splitted;
    char *full_path;
    int i;

    path = getenv("PATH");
    if (access(g->cmd->exec_name, X_OK) == 0)
        execve(g->cmd->exec_name, g->cmd->args, env->environ);
    else
    {
        splitted = ft_split(path, ':');
        i = 0;
        while (splitted && splitted[i])
        {
            full_path = triple_strjoin(splitted[i], "/", g->cmd->exec_name);
            if (!full_path)
            {
                free_doub_array((void **)splitted);
                exit_error("malloc");
            }
            if (access(full_path, X_OK) == 0)
            {
                execve(full_path, g->cmd->args, env->environ);
                free(full_path);
                break;
            }
            free(full_path);
            i++;
        }
        free_doub_array((void **)splitted);
    }
    
    write(2, g->cmd->exec_name, ft_strlen(g->cmd->exec_name));
    write(2, ": command not found\n", 20);
    exit(127);
}

void	setup_operators_child(t_g_pipe *g)
{
	if (g->standard_io[0] != STDIN_FILENO)
	{
		dup2(g->standard_io[0], STDIN_FILENO);
		close(g->standard_io[0]);
	}
	if (g->standard_io[1] != STDOUT_FILENO)
	{
		dup2(g->standard_io[1], STDOUT_FILENO);
		close(g->standard_io[1]);
	}
	close_fds(g);
}

int	execute_path(char *path, t_g_pipe *g, char **environ)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		if (path)
			free(path);
		free_doub_array((void **) g->cmd->args);
		exit_error("fork");
	}
	if (pid == 0)
	{
		setup_operators_child(g);
		if (path && (execve(path, g->cmd->args, environ) == -1))
		{
			free(path);
			free_doub_array((void **) g->cmd->args);
			exit_error("execve");
		}
		else if (execve(g->cmd->exec_name, g->cmd->args, environ) == -1)
		{
			free_doub_array((void **) g->cmd->args);
			exit_error("execve");
		}
	}
	else
	{
		close_fds(g);
		if (g->cmd->next_interaction != PIPE)
		{
			int status;
			waitpid(pid, &status, 0);
			status = WEXITSTATUS(status);

			if ((g->cmd->next_interaction == AND && status != 0) ||
				(g->cmd->next_interaction == OR && status == 0))
			{
				return (0);
			}
		}
	}
	return (1);
}
