/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/25 16:28:17 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
				handle_errors(&cmds, splitted, NULL);
				free_doub_array(env->environ);
				exit_error("malloc");
            }
            if (access(full_path, X_OK) == 0)
            {
				free_doub_array(splitted);
                execve(full_path, cmds->args, env->environ);
				free_doub_array(env->environ);
                exit_error("");
            }
            free(full_path);
            i++;
        }
        free_doub_array(splitted);
    }
    write(2, cmds->name, ft_strlen(cmds->name));
    write(2, ": command not found\n", 20);
	free_glob_pipe(&cmds);
	free_doub_array(env->environ);
	rl_clear_history();
    exit(127);
}
