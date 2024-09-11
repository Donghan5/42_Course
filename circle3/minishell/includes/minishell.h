/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 01:02:57 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/11 16:07:53 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// header files
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <signal.h>

// custom header files
# include "../Libft/libft.h"

// defines redirections and directions
# define DOUBLE_OUT 1
# define DOUBLE_IN 2
# define OUT 3
# define IN 4
# define PIPE 5
# define DOUBLE_PIPE 6
# define DOUBLE_AND 7

// defines quotes check
# define NOT_CLOSED 0
# define CLOSED 1

// define signal
# define STDOUT 1

// define buffer to use in gnl func
# define BUFFER_SIZE 256

typedef struct s_command
{
	char		*exec_name;
	char		**args;
	int			prev_interaction;
	int			next_interaction;
}				t_command;

typedef struct s_name_value
{
	char	*name;
	char	*value;
}				t_name_value;

typedef struct s_env
{
	char			**environ;
	t_name_value	*environ_name_value;
}				t_env;

// ft_get.c
char	*get_value_for_name(t_name_value *arr, char *name);
char	*replace_home_tilde(char *cwd);
char	*get_prompt(void);

// check.c
int		cd_check(t_command *cmd, t_env *env);
int		buildin_check(t_env *env, t_command *cmd);

// excute.c
void	search_path_and_run(t_command *cmd, t_env *env, int fds[2]);
int		execute_path(char *path, char **args, char **environ, int fds[2]);

// fill.c
void	fill_cmd(char **whitespaced, t_command *cmd, int n, int start_index);
int		fill_interaction(t_command *cmd, char *word);

// ft_exit.c
void	normal_exit_check(t_command *cmd);
void	exit_error(char *perror_message);

// parsing.c
char	**parse(t_command **(*cmds), char *line);
void	parse_env(t_env *env);

// pipe.c
void	setup_pipes(int input_fd, int output_fd);
void	handle_redirection(t_command *cmd, t_command *cmd2);

// utils.c
void	print_arr(char **arr);
void	free_doub_array(void **arr);
char	*triple_strjoin(char *s1, char *s2, char *s3);
char	*get_next_line(int fd);

// ft_whitesplit.c
char	**ft_whitesplit(char const *s);

// ft_whitespace.c
int		ft_iswhitespace(int c);

// quote.c
int		check_unclosed_quote(char *str, char quote);
int		double_quote_cnt(char *str, int *size, char **envp);
int		single_quote_cnt(char *str, int *size);

// signal.c
void	handle_signal(int signo);
void	set_signal(void);

// env_utils.c
int		size_env_value(char *str, int size, char **envp);
int		size_env_key(char *str);
int		env_cnt(char *str, int *size, char **envp);
int		get_env_parse_len(char *str, char **envp);

// expander.c
char	*expander(char *input_str, char **envp);

#endif
