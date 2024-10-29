/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:47:56 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/25 18:44:41 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Handling pipes in case when failed command is connected to pipes
*/
void	no_execs(t_glob_pipe *temp_cmd, t_env *env, int *prev_pipe)
{
	if (!temp_cmd->name)
		env->sts = 0;
	if (temp_cmd->is_exec_ignore && (temp_cmd->op == NO_EXEC_PIPE))
	{
		if (*prev_pipe != -1)
			smart_close(*prev_pipe);
		smart_close(temp_cmd->pipe_fds[1]);
		*prev_pipe = temp_cmd->pipe_fds[0];
	}
	if (temp_cmd->is_exec_ignore)
		close_fds(temp_cmd, 0, 0);
}

/*
	Running builtin without subshell and handling redirections via std backups
*/
void	builtin_no_process(t_glob_pipe *tmp, t_env *env)
{
	int	std_out;
	int	std_in;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (tmp->redir_io[0] != STDIN_FILENO)
	{
		dup2(tmp->redir_io[0], STDIN_FILENO);
		close (tmp->redir_io[0]);
	}
	if (tmp->redir_io[1] != STDOUT_FILENO)
	{
		dup2(tmp->redir_io[1], STDOUT_FILENO);
		close (tmp->redir_io[1]);
	}
	if (ft_strncmp("exit", tmp->name, 5))
		builtin_run(env, tmp);
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close (std_in);
	close (std_out);
	close_fds(tmp, 0, 0);
	if (!ft_strncmp("exit", tmp->name, 5))
		builtin_run(env, tmp);
}

void	parent_process(t_glob_pipe *tmp, int *prev_pipe, t_env *env, int pid)
{
	if (*prev_pipe != -1)
		smart_close(*prev_pipe);
	if (tmp->op == PIPE)
	{
		smart_close(tmp->pipe_fds[1]);
		*prev_pipe = tmp->pipe_fds[0];
	}
	else
		*prev_pipe = -1;
	close_fds(tmp, 1, 1);
	if (tmp->previous)
	{
		smart_close(tmp->previous->pipe_fds[0]);
		smart_close(tmp->previous->pipe_fds[1]);
	}
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
		smart_close(*prev_pipe);
	}
	if (tmp->op == PIPE)
	{
		smart_close(tmp->pipe_fds[0]);
		dup2(tmp->pipe_fds[1], STDOUT_FILENO);
		smart_close(tmp->pipe_fds[1]);
	}
	if (tmp->redir_io[0] != STDIN_FILENO)
		dup2(tmp->redir_io[0], STDIN_FILENO);
	if (tmp->redir_io[1] != STDOUT_FILENO)
		dup2(tmp->redir_io[1], STDOUT_FILENO);
	close_fds(tmp, 1, 0);
	if (!tmp->name)
		return (env->sts = 0, free_doub_array(env->environ), exit(0));
	if (builtin)
	{
		builtin_run(env, tmp);
		return (handle_errors(&tmp, env->environ, NULL), exit(env->sts));
	}
	search_path_and_run(tmp, env);
	return (env->sts = 1, free_doub_array(env->environ), exit(1));
}
