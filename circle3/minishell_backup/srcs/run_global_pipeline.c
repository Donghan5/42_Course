/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 21:50:24 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Closing only real fds
*/
void	smart_close(int fd)
{
	if (fd != -1 && fd > 2)
		close(fd);
}

/*
	Wait for all background processes, no status update
*/
void	wait_background_processes(void)
{
	int	status;

	while (wait(&status) > 0)
		(void)status;
}

/*
	Running whole global pipeline of commands
*/
void	pipeline_cycle(t_glob_pipe *t, int *prev, t_env *e)
{
	int		built;
	pid_t	pid;

	while (t)
	{
		no_execs(t, e, prev);
		if (!t->is_exec_ignore)
		{
			built = builtin_check(t);
			pid = -1;
			if (t->op == PIPE || *prev != -1 || !built)
				pid = fork();
			if ((t->op == PIPE || *prev != -1 || !built) && (pid == -1))
				return (close_fds(t, 1, 0), perror("fork"));
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

/*
	Parent outer shell to run all commands
*/
void	run_global_pipeline(t_glob_pipe **cmds_start, t_env *env)
{
	t_glob_pipe	*temp_cmd;
	int			prev_pipe;

	prev_pipe = -1;
	temp_cmd = *cmds_start;
	pipeline_cycle(temp_cmd, &prev_pipe, env);
	wait_background_processes();
}
