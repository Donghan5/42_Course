/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_managers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:12:25 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/27 18:33:44 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

/*
	Tracking and waiting for children processes.
	Wait != 0:
		Waiting and updating status of all tracked children,
	Wait == 0:
		Adding given pid to the tracking list.
	@param old: size of the old tracking list.
*/
void	children_manager(int pid, t_env *env, int wait, int reset)
{
	static pid_t	*child_pids;
	static int		child_count;
	int				i;
	int				status;
	size_t			old;

	old = sizeof(pid_t) * child_count;
	if (!reset && !wait)
	{
		child_count++;
		child_pids = ft_realloc(child_pids, old, child_count * sizeof(pid_t));
		return (child_pids[child_count - 1] = pid, (void)0);
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

t_glob_pipe	*skipping_mode(t_glob_pipe *t, int skip_next)
{
	int	priority;
	int	paren_id;

	priority = 0;
	paren_id = 0;
	if (t->previous)
	{
		priority = t->previous->priority;
		paren_id = t->previous->paren_id;
	}
	while (t && skip_next)
	{
		if ((t->priority <= priority
				&& (t->paren_id == paren_id || !t->paren_id || !paren_id))
			&& ((t->previous && t->previous->op == OR && skip_next == 1)
				|| (t->previous && t->previous->op == AND && skip_next == 2)))
			break ;
		t = t->next;
	}
	return (t);
}

/*
	Modes:
		mode == 1 - skipping,
		mode == 2 - reset
		set_priority > -1 - update internal priority.
		mode == 0 and set_priority < 0 - updating skipping mode

	Skipping:
		Checking skip_next and returning t_glob_pipe t command
		after skipping commands which should not be run.
		With skipping mode set it will skip all commands with higher than
		previous "t" priority without preceding AND (skipping mode 2) or OR
		(skipping mode 1), or the whole parenthesis of the same priority.
	Reset:
		Resets internal static ints to 0.
	Update internal priority:
		Sets internal priority to set_priority.
	Updating skipping mode:
		Sets internal skipping mode to 1 if current command has following
		AND (&&) and it wasn't successful.
		Sets internal skipping mode to 2 if current command has following
		OR (||) and it was successful.
*/
t_glob_pipe	*skipper(t_glob_pipe *t, t_env *e, int mode)
{
	static int	skip_next;

	if (mode == 2)
		return (skip_next = 0, NULL);
	if (mode == 1)
	{
		if (skip_next)
			t = skipping_mode(t, skip_next);
		return (skip_next = 0, t);
	}
	if (t && t->op == AND && e->sts != 0)
		skip_next = 1;
	if (t && t->op == OR && e->sts == 0)
		skip_next = 2;
	return (NULL);
}
