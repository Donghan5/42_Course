/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 13:57:32 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_glob_pipe *glob_pipe)
{
	int	i;

	i = 0;
	while (i < glob_pipe->close_count)
	{
		close(glob_pipe->files_to_close[i]);
		i++;
	}
}

void search_path_and_run(t_glob_pipe *cmds, t_env *env)
{
    char	*path;
    char	**splitted;
    char	*full_path;
    int		i;

    path = getenv("PATH");
    if (access(cmds->name, X_OK) == 0)
        execve(cmds->name, cmds->args, env->environ);
    else
    {
        splitted = ft_split(path, ':');
        i = 0;
        while (splitted && splitted[i])
        {
            full_path = triple_strjoin(splitted[i], "/", cmds->name);
            if (!full_path)
            {
                free_doub_array((void **)splitted);
                exit_error("malloc");
            }
            if (access(full_path, X_OK) == 0)
            {
				free_doub_array((void **)splitted);
                execve(full_path, cmds->args, env->environ);
                free(full_path);
                break;
            }
            free(full_path);
            i++;
        }
        free_doub_array((void **)splitted);
    }
    write(2, cmds->name, ft_strlen(cmds->name));
    write(2, ": command not found\n", 20);
    exit(127);
}
