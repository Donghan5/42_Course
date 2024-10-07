/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:47:56 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/30 23:22:03 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_no_exec_pipe(t_glob_pipe *temp_cmd, int *prev_pipe)
{
	if (temp_cmd->is_exec_ignore && (temp_cmd->op == NO_EXEC_PIPE))
	{
		if (*prev_pipe != -1)
			close(*prev_pipe);
		close(temp_cmd->pipe_fds[1]);
		*prev_pipe = temp_cmd->pipe_fds[0];
	}
}

void	builtin_no_process(t_glob_pipe *tmp, int *std_io, t_env *env)
{
	if (tmp->redir_io[0] != STDIN_FILENO)
		dup2(tmp->redir_io[0], STDIN_FILENO);
	if (tmp->redir_io[1] != STDOUT_FILENO)
		dup2(tmp->redir_io[1], STDOUT_FILENO);
	builtin_run(env, tmp);
	dup2(std_io[0], STDIN_FILENO);
	dup2(std_io[1], STDOUT_FILENO);
	close_fds(tmp);
}

void	parent_process(t_glob_pipe *tmp, int *prev_pipe, t_env *env, int pid)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (tmp->op == PIPE)
	{
		close(tmp->pipe_fds[1]);
		*prev_pipe = tmp->pipe_fds[0];
	}
	else
		*prev_pipe = -1;
	close_fds(tmp);
	if (tmp->op != PIPE)
	{
		waitpid(pid, &env->sts, 0);
		env->sts = WEXITSTATUS(env->sts);
	}
}

void	child_process(int *prev_pipe, t_glob_pipe *tmp, int builtin, t_env *env)
{
	if (*prev_pipe != -1)
	{
		dup2(*prev_pipe, STDIN_FILENO);
		close(*prev_pipe);
	}
	if (tmp->op == PIPE)
	{
		close(tmp->pipe_fds[0]);
		dup2(tmp->pipe_fds[1], STDOUT_FILENO);
		close(tmp->pipe_fds[1]);
	}
	if (tmp->redir_io[0] != STDIN_FILENO)
		dup2(tmp->redir_io[0], STDIN_FILENO);
	if (tmp->redir_io[1] != STDOUT_FILENO)
		dup2(tmp->redir_io[1], STDOUT_FILENO);
	close_fds(tmp);
	if (builtin)
	{
		builtin_run(env, tmp);
		handle_errors(&tmp, env->environ, NULL);
		exit(env->sts);
	}
	search_path_and_run(tmp, env);
	env->sts = 1;
	return (free_doub_array(env->environ), exit(1));
}
