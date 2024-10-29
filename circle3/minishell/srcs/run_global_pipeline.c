/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/29 12:48:55 by pzinurov         ###   ########.fr       */
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

int	process_picker(t_glob_pipe *t, t_env *e, int *prev, int non_int_fd)
{
	int	pid;
	int	built;

	built = builtin_check(t);
	pid = -1;
	if (t->op == PIPE || *prev != -1 || !built)
	{
		smart_close(non_int_fd);
		pid = fork();
	}
	if ((t->op == PIPE || *prev != -1 || !built) && (pid == -1))
		return (close_fds(t, 1, 0), perror("fork"), 0);
	if (pid == 0)
		child_process(prev, t, built, e);
	else if (pid > 0)
		parent_process(t, prev, e, pid);
	else
		builtin_no_process(t, e);
	return (1);
}

/*
	Running whole global pipeline of commands
*/
void	pipeline_cycle(t_glob_pipe *t, int *prev, t_env *e, int non_int_fd)
{
	while (t)
	{
		no_execs(t, e, prev);
		if (!t->is_exec_ignore)
		{
			if (!process_picker(t, e, prev, non_int_fd))
				return ;
			if ((t->op == AND && e->sts != 0) || (t->op == OR && e->sts == 0))
				break ;
		}
		t = t->next;
	}
}

/*
	Parent outer shell to run all commands
*/
void	run_global_pipeline(t_glob_pipe **cmds_start, t_env *env,
	int non_int_fd)
{
	t_glob_pipe	*temp_cmd;
	int			prev_pipe;

	prev_pipe = -1;
	temp_cmd = *cmds_start;
	pipeline_cycle(temp_cmd, &prev_pipe, env, non_int_fd);
	wait_background_processes();
}
