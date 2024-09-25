/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/25 16:28:28 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_background_processes(void)
{
	int	pid;

	pid = wait(NULL);
	while (pid > 0)
	{
		pid = wait(NULL);
	}
}

int	fd_restore_close(int std_io[2], t_glob_pipe *temp_cmd)
{
	if (temp_cmd)
		close_fds(temp_cmd);
	dup2(std_io[0], STDIN_FILENO);
    dup2(std_io[1], STDOUT_FILENO);
    close(std_io[0]);
    close(std_io[1]);
	return (1);
}

void    run_global_pipeline(t_glob_pipe *cmds_start, t_env *env)
{
    t_glob_pipe *temp_cmd;
    pid_t       pid;
    int         prev_pipe_read;
    int         is_builtin;
	int			std_io[2];

    prev_pipe_read = -1;
    std_io[0] = dup(STDIN_FILENO);
    std_io[1] = dup(STDOUT_FILENO);
    temp_cmd = cmds_start;
    while (temp_cmd)
    {
        if (!temp_cmd->is_exec_ignore && temp_cmd->name)
        {
            is_builtin = builtin_check(temp_cmd);
            if (temp_cmd->operator == PIPE || prev_pipe_read != -1 || !is_builtin)
            {
                pid = fork();
                if (pid == -1)
                    return (fd_restore_close(std_io, temp_cmd), perror("fork"));
            }
            else if (is_builtin)
                pid = -1;
            if (pid == 0)
            {
                if (prev_pipe_read != -1)
                {
                    dup2(prev_pipe_read, STDIN_FILENO);
                    close(prev_pipe_read);
                }
                if (temp_cmd->operator == PIPE)
                {
                    close(temp_cmd->pipe_fds[0]);
                    dup2(temp_cmd->pipe_fds[1], STDOUT_FILENO);
					close(temp_cmd->pipe_fds[1]);
                }
                if (temp_cmd->redir_io[0] != STDIN_FILENO)
                    dup2(temp_cmd->redir_io[0], STDIN_FILENO);
                if (temp_cmd->redir_io[1] != STDOUT_FILENO)
                    dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
                close_fds(temp_cmd);
                if (is_builtin)
                {
                    builtin_run(env, temp_cmd);
                    exit(env->status);
                }
                else
                {
                    search_path_and_run(temp_cmd, env);
					env->status = 1;
					free_doub_array(env->environ);
                    exit(1);
                }
            }
            else if (pid > 0)
            {
                if (prev_pipe_read != -1)
                    close(prev_pipe_read);
                if (temp_cmd->operator == PIPE)
                {
                    close(temp_cmd->pipe_fds[1]);
                    prev_pipe_read = temp_cmd->pipe_fds[0];
                }
                else
                    prev_pipe_read = -1;
                close_fds(temp_cmd);
                if (temp_cmd->operator != PIPE)
                {
                    waitpid(pid, &env->status, 0);
                    env->status = WEXITSTATUS(env->status);
                }
            }
            else
            {
                if (temp_cmd->redir_io[0] != STDIN_FILENO)
                    dup2(temp_cmd->redir_io[0], STDIN_FILENO);
                if (temp_cmd->redir_io[1] != STDOUT_FILENO)
                    dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
                builtin_run(env, temp_cmd);
                dup2(std_io[0], STDIN_FILENO);
                dup2(std_io[1], STDOUT_FILENO);
                close_fds(temp_cmd);
            }
            if ((temp_cmd->operator == AND && env->status != 0) ||
                (temp_cmd->operator == OR && env->status == 0))
                break;
        }
        temp_cmd = temp_cmd->next;
    }
    fd_restore_close(std_io, NULL);
    wait_background_processes();
}
