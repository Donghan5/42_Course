/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/12 15:58:48 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// wait the process
void	wait_background_processes(void)
{
	int	pid;

	pid = wait(NULL);
	while (pid > 0)
	{
		pid = wait(NULL);
	}
}

// cycle of the pipeline (when pipe input)
void	pipeline_cycle(t_glob_pipe *t, int *prev, t_env *e)
{
	int		built;
	pid_t	pid;

	while (t)
	{
		process_no_exec_pipe(t, prev);
		if (!t->is_exec_ignore && t->name)
		{
			built = builtin_check(t);
			pid = -1;
			if (t->op == PIPE || *prev != -1 || !built)
				pid = fork();
			if ((t->op == PIPE || *prev != -1 || !built) && (pid == -1))
				return (close_fds(t), perror("fork"));
			if (pid == 0)
				child_process(prev, t, built, e);
			else if (pid > 0)
				parent_process(t, prev, e, pid);
			else
				builtin_no_process(t, e);
			if ((t->op == AND && e->sts != 0) || (t->op == OR && e->sts == 0))
				break ;
		}
		t = t->next;
	}
}

// run pipe-line entire
void	run_global_pipeline(t_glob_pipe **cmds_start, t_env *env)
{
	t_glob_pipe	*temp_cmd;
	int			prev_pipe;

	prev_pipe = -1;
	temp_cmd = *cmds_start;
	pipeline_cycle(temp_cmd, &prev_pipe, env);

	temp_cmd = *cmds_start;
	while (temp_cmd)
	{
		// close(temp_cmd->pipe_fds[0]);
		// close(temp_cmd->pipe_fds[1]);
		// close(temp_cmd->redir_io[0]);
		// close(temp_cmd->redir_io[1]);
		close_fds(temp_cmd);
		// waitpid(temp_cmd->pid, &env->sts, 0);
		// env->sts = WEXITSTATUS(env->sts);
		temp_cmd = temp_cmd->next;
	}
	wait_background_processes();
}
