/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:27:45 by donghank          #+#    #+#             */
/*   Updated: 2024/08/21 15:33:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

/*---------- includes ----------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

/*---------- struct for pipex ----------*/
typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	char	*path;
	char	*cmd;
	char	**paths;
	char	**cmd_args;
	int		infile;
	int		outfile;
	int		*tube;
	int		tube_count;
	int		here_doc;
	int		start;
	int		limit;
}	t_pipex;

/*---------- parsing_bonus.c ----------*/
char	**get_paths(char **envp);
char	*find_path(t_pipex	*pipex, char **envp);
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index);

/*---------- pipex_bonus.c ----------*/
void	init_pipex(t_pipex *pipex, int fd1, int fd2, int cmd_count);
void	child_process(t_pipex *pipex, char **argv, char **envp, int cmd_index);
void	parent_process(t_pipex *pipex, char **argv, char **envp, int cmd_index);
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index);
void	handle_error_cleanup(t_pipex *pipex, char *msg);

/*---------- utils_bonus.c ----------*/
void	free_cmd_args(char **cmd_args);
void	free_paths(char **paths);
void	handle_error(char *str);
void	cleanup(t_pipex *pipex);
int		ft_strarray_len(char **arr);


/*---------- heredoc_bonus.c ----------*/
void	doing_process(t_pipex *pipex, int argc, char **argv, char **envp);
#endif
