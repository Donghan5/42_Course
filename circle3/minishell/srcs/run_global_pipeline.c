/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/17 17:00:26 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void setup_redirections(t_glob_pipe *cmd);

void	wait_background_processes(void)
{
	int	pid;

	pid = wait(NULL);
	while (pid > 0)
	{
		pid = wait(NULL);
	}
}

void    run_global_pipeline(t_glob_pipe *cmds_start, t_env *env, int *status)
{
    t_glob_pipe *temp_cmd;
    pid_t       pid;
    int         prev_pipe_read;
    int         is_builtin;
    int         original_stdin;
    int         original_stdout;

    prev_pipe_read = -1;
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    temp_cmd = cmds_start;
    while (temp_cmd)
    {
        if (!temp_cmd->is_exec_ignore && temp_cmd->name)
        {
            is_builtin = builtin_check(temp_cmd);
            if (temp_cmd->operator == PIPE || prev_pipe_read != -1)
            {
                pid = fork();
                if (pid == -1)
                    exit_error("fork");
            }
            else if (is_builtin)
                pid = -1;
			else
				pid = fork();
			if ((pid == -1) && !is_builtin)
				exit_error("fork");
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
                }
                if (temp_cmd->redir_io[0] != STDIN_FILENO)
                    dup2(temp_cmd->redir_io[0], STDIN_FILENO);
                if (temp_cmd->redir_io[1] != STDOUT_FILENO)
                    dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
                
                if (is_builtin)
                {
                    builtin_run(env, temp_cmd, status);
                    exit(*status);
                }
                else
                {
                    search_path_and_run(temp_cmd, env);
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
                    waitpid(pid, status, 0);
                    *status = WEXITSTATUS(*status);
                }
            }
            else
            {
                if (temp_cmd->redir_io[0] != STDIN_FILENO)
                    dup2(temp_cmd->redir_io[0], STDIN_FILENO);
                if (temp_cmd->redir_io[1] != STDOUT_FILENO)
                    dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
                builtin_run(env, temp_cmd, status);
                dup2(original_stdin, STDIN_FILENO);
                dup2(original_stdout, STDOUT_FILENO);
                close_fds(temp_cmd);
            }
            if ((temp_cmd->operator == AND && *status != 0) ||
                (temp_cmd->operator == OR && *status == 0))
                break;
        }
        temp_cmd = temp_cmd->next;
    }
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    wait_background_processes();
}

// void	run_global_pipeline1(t_glob_pipe *cmds_start, t_env *env, int *status)
// {
// 	t_glob_pipe	*temp_cmd;
// 	pid_t		pid;
// 	int			prev_pipe_read;
// 	int			is_builtin;

// 	prev_pipe_read = -1;
// 	temp_cmd = cmds_start;
// 	while (temp_cmd)
// 	{
// 		if (!temp_cmd->is_exec_ignore && temp_cmd->name)
// 		{
// 			is_builtin = builtin_check(temp_cmd);
// 			if (!is_builtin)
// 			{
// 				pid = fork();
// 				if (pid == -1)
// 					exit_error("fork");
// 			}
// 			if (is_builtin || (pid == 0))
// 			{
// 				if (temp_cmd->redir_io[0] != STDIN_FILENO)
// 					dup2(temp_cmd->redir_io[0], STDIN_FILENO);
// 				else if (prev_pipe_read != -1)
// 				{
// 					dup2(prev_pipe_read, STDIN_FILENO);
// 					close(prev_pipe_read);
// 				}
// 				if (temp_cmd->redir_io[1] != STDOUT_FILENO)
// 					dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
// 				else if (temp_cmd->operator == PIPE)
// 				{
// 					close(temp_cmd->pipe_fds[0]);
// 					dup2(temp_cmd->pipe_fds[1], STDOUT_FILENO);
// 				}
// 				close_fds(temp_cmd);
// 				// fprintf(stderr, "%s: pipe_fds %d %d, stdio %d %d, prev pipe %d\n", temp_cmd->name, temp_cmd->pipe_fds[0], temp_cmd->pipe_fds[1], temp_cmd->redir_io[0], temp_cmd->redir_io[1], prev_pipe_read);
// 				if (!builtin_run(env, temp_cmd, status))
// 				{
// 					search_path_and_run(temp_cmd, env);
// 					exit(1);
// 				}
// 				if (!is_builtin)
// 					exit(0);
// 			}
// 			if (!is_builtin)
// 			{
// 				if (prev_pipe_read != -1)
// 					close(prev_pipe_read);
// 				if (temp_cmd->operator == PIPE)
// 				{
// 					close(temp_cmd->pipe_fds[1]);
// 					prev_pipe_read = temp_cmd->pipe_fds[0];
// 				}
// 				else
// 					prev_pipe_read = -1;
// 				close_fds(temp_cmd);
// 				if (temp_cmd->operator != PIPE)
// 				{
// 					waitpid(pid, status, 0);
// 					if ((temp_cmd->operator == AND && WEXITSTATUS(*status) != 0)
// 						|| (temp_cmd->operator == OR
// 							&& WEXITSTATUS(*status) == 0))
// 					{
// 						*status = WEXITSTATUS(*status);
// 						break ;
// 					}
// 					else
// 						*status = WEXITSTATUS(*status);
// 				}
// 			}
// 		}
// 		temp_cmd = temp_cmd->next;
// 	}
// 	wait_background_processes();
// }
