/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:47:56 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/15 15:17:36 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Handling pipes in case when failed command is connected to pipes
*/
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
		close(*prev_pipe), *prev_pipe = -1;
	if (tmp->op == PIPE)
	{
		close(tmp->pipe_fds[1]);
		*prev_pipe = tmp->pipe_fds[0];
	}
	else
		*prev_pipe = -1;
	close_fds(tmp, 1, 1);
	if (tmp->previous)
	{
		if (tmp->previous->pipe_fds[0] != -1)
			close(tmp->previous->pipe_fds[0]);
		if (tmp->previous->pipe_fds[1] != -1)
			close(tmp->previous->pipe_fds[1]);
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
	close_fds(tmp, 1, 0);
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
