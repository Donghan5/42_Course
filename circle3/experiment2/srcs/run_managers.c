/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_managers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:12:25 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 20:27:18 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Tracking and waiting for children processes.
	Wait != 0:
		Waiting and updating status of all tracked children,
	Wait == 0:
		Adding given pid to the tracking list.
*/
void	children_manager(int pid, t_env *env, int wait, int reset)
{
	static pid_t	*child_pids;
	static int		child_count;
	int				i;
	int				status;

	if (!reset && !wait)
	{
		child_count++;
		child_pids = realloc(child_pids, child_count * sizeof(pid_t));
		child_pids[child_count - 1] = pid;
		return ;
	}
	i = 0;
	while (!reset && i < child_count)
	{
		status = 0;
		waitpid(child_pids[i], &status, 0);
		if ((i == child_count - 1) && WIFEXITED(status))
			env->sts = WEXITSTATUS(status);
		else if (i == child_count - 1)
			env->sts = 1;
		i++;
	}
	return (free(child_pids), child_pids = NULL, child_count = 0, (void)0);
}

/*
	Mode:
	1 - skipping,
	2 - reset
*/
t_glob_pipe	*skipper(t_glob_pipe *t, t_env *e, int mode, int set_priority)
{
	static int		skip_next;
	static int		priority;

	if (mode == 2)
		return (skip_next = 0, priority = 0, NULL);
	if (set_priority > -1)
		return (priority = set_priority, NULL);
	if (mode == 1)
	{
		if (skip_next == 1 && t->priority == priority)
			t = t->next;
		while (skip_next == 2 && t)
		{
			if (t->priority <= priority
				&& (t->previous && t->previous->op == AND))
				break ;
			t = t->next;
		}
		return (skip_next = 0, t);
	}
	if (t->op == AND && e->sts != 0)
		skip_next = 1;
	if (t->op == OR && e->sts == 0)
		skip_next = 2;
	return (NULL);
}
