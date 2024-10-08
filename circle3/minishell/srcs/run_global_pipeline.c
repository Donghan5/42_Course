/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/08 23:42:14 by donghan          ###   ########.fr       */
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

// to restore std fds by using dup2
int	fd_restore_close(int std_io[2], t_glob_pipe *temp_cmd)
{
	if (temp_cmd)
		close_fds(temp_cmd);
	if (std_io)
	{
		dup2(std_io[0], STDIN_FILENO);
		close(std_io[0]);
		dup2(std_io[1], STDOUT_FILENO);
		close(std_io[1]);
	}
	return (1);
}

// cycle of the pipeline (when pipe input)
void	pipeline_cycle(t_glob_pipe *t, int *std_io, int *prev, t_env *e)
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
				return (fd_restore_close(std_io, t), perror("fork"));
			if (pid == 0)
				child_process(prev, t, built, e);
			else if (pid > 0)
				parent_process(t, prev, e, pid);
			else
				builtin_no_process(t, std_io, e);
			if ((t->op == AND && e->sts != 0) || (t->op == OR && e->sts == 0))
				break ;
		}
		t = t->next;
	}
}

// run pipe-line entire
void	run_global_pipeline(t_glob_pipe *cmds_start, t_env *env)
{
	t_glob_pipe	*temp_cmd;
	int			prev_pipe;
	int			std_io[2];

	prev_pipe = -1;
	std_io[0] = dup(STDIN_FILENO);
	std_io[1] = dup(STDOUT_FILENO);
	// std_io[0] = 100;
	// std_io[1] = 101;
	temp_cmd = cmds_start;
	pipeline_cycle(temp_cmd, std_io, &prev_pipe, env);
	fd_restore_close(std_io, NULL);
	wait_background_processes();
}
