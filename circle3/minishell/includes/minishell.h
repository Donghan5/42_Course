/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghan <donghan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 01:02:57 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/28 17:03:01 by donghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// header files
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <errno.h>
# include <dirent.h>
# include <sys/stat.h>

// custom header files
# include "../Libft/libft.h"

// defines redirections and directions
enum e_operator
{
	PIPE = 1,
	AND,
	OR,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND_OUT,
	HERE_DOC,
	REDIRECT_EXPECTED,
	REDIR_PIPE,
	NO_EXEC_PIPE
};

// defines run or not
# define RUN 1
# define NOT_RUN 0

// defines quotes check
# define NOT_CLOSED 0
# define CLOSED 1

// define success and fail
# define SUCCESS 0
# define FAIL 1

// define signal
# define STDOUT 1
# define STDIN 0

// define to env_utils2
# define UPDATED 1
# define NOT_UPDATED 0

// define buffer to use in gnl func
# define BUFFER_SIZE 256

// error msgs
# define ALLOC_ERROR "Allocate fail"
# define UPDATE_ERROR "Update fail"
# define UNSET_NOT_IDENTIFY "unset: not a valid identifier"
# define FORK_ERROR "fork error"

// define for shlvl.c files
# define NON_VALID 1
# define VALID 0

// define numbers tool
# define NOT_FOUND -1

// struct of global pipe lines
typedef struct	s_glob_pipe
{
	int					redir_io[2];
	int					pipe_fds[2];
	int					files_to_close[1024];
	int					close_count;
	int					is_exec_ignore;
	char				*name;
	char				**args;
	int					operator;
	struct s_glob_pipe	*next;
	struct s_glob_pipe	*previous;
}				t_glob_pipe;

typedef struct	s_free_exit
{
	t_glob_pipe	*glob_pipe;
	char		**doub;
	char		**doub_two;
	char		string;
	char		string_two;
	char		error_message;
	int			is_exit;
}				t_free_exit;

// name key=value
typedef struct s_name_value
{
	char				*name;
	char				*value;
	struct s_name_value	*next;

}				t_name_value;

// struct of the global environment
typedef struct s_env
{
	char			**environ;
	t_name_value	*environ_name_value;
	int				status;
}				t_env;

// pre_parsing.c
char			***pre_parsing(char *line, t_env *env);

// tokenizer.c
char			**tokenizer(char const *s);

// run_global_pipeline.c
void		    run_global_pipeline(t_glob_pipe *cmds_start, t_env *env);

// free.c
void			free_triple_tokens(char ***arr);
int				handle_errors(t_glob_pipe **glob_pipe, char **doub_arr, char *err_msg);
int				handle_errors_tokens(char **doub_arr, char ***triple_tokens, char *err_msg);
void			free_glob_pipe(t_glob_pipe **glob_pipe);

// ft_get.c
char			*get_value_for_name(t_name_value *arr, char *name);
char			*replace_home_tilde(char *cwd);
char			*get_hostname(void);
char			*get_prompt(void);

// cd.c
void			cd_check(t_glob_pipe *cmd, t_env *env);

// pwd.c
void			pwd_check(t_glob_pipe *cmd, int *status);

// builtins.c
int				builtin_check(t_glob_pipe *cmd);
void			builtin_run(t_env *env, t_glob_pipe *cmd);

// execute.c
void			search_path_and_run(t_glob_pipe *glob_pipe, t_env *env);
void			close_fds(t_glob_pipe *glob_pipe);

// fill.c
int				fill_args(char ***tokens, t_glob_pipe *glob_pipe, int n, int start_index);
int				fill_operator(t_glob_pipe *glob_pipe, char *word);
int				fill_operator_token(t_glob_pipe *glob_pipe, char **token);

// ft_exit.c
void			normal_exit_check(t_glob_pipe *cmd, t_env *env);
void			exit_error(char *perror_message);
int				str_is_alnum(char *str);

// parsing.c
int				parse(t_glob_pipe **glob_pipe, char ***tokens);
void			parse_env(t_env *env, char **environ);
int 			is_operator_token(char **token);
int 			is_redirect(char **token);
int 			is_operator(char *token);
int				is_tokens_error(char **current_token, char **next_token, int i);
int				compound_token_error(char **token, char **next_token, int i, t_glob_pipe **glob_pipe);

// utils.c
void			smart_print_err(char *msg);
void			print_arr(char **arr);
void			free_doub_array(char **arr);
char			*triple_strjoin(char *s1, char *s2, char *s3);
char			*get_next_line(int fd);
void			three_time_free(char *s1, char *s2, char *s3);

// header.c
void			header(void);

// ft_whitespace.c
int				is_quote(int c);
int				ft_iswhitespace(int c);

// quote.c
int				check_unclosed_quote(char *str, char quote);
int				double_quote_cnt(char *str, int *size, t_env *env);
int				single_quote_cnt(char *str, int *size);

// signal.c
void			handle_signal(int signo);
void			set_signal(void);

// env_utils.c
int				size_env_value(char *str, int size, char **envp);
int				size_env_key(char *str);
char			*get_key_from_env(char *env_str);
int				env_cnt(char *str, int *size, t_env *env);
int				get_env_parse_len(char *str, t_env *env);

// env_utils2.c
void			create_new_env_var(char *tok_str, t_env *env);
char			*swap_new_env_var(t_env *env, char *var_name, char *new_var);
int				update_new_env_var(char *var, char *new_val, t_env *env);
void			env_add_front(t_name_value **env_node, t_name_value *new_node);
t_name_value	*new_env_node(char *content);

// shlvl.c
void			increment_shell_level(t_env *env);

// expander_utils.c
int				env_copy_cnt(char *src, char **dest, t_env *env);
int				double_quote_copy_cnt(char *src, char **dest, t_env *env);
int				single_quote_copy_cnt(char *src, char **dest);
void			copy_strings(char *input, char *dest, t_env *env);

// expander.c
char			*expander(char *cmd, t_env *env);

// prepare_pipeline.c
int				prepare_pipeline(t_glob_pipe *glob_pipe, t_env *env);

// export_utils.c
int				env_list_size(t_name_value *env_node);
char			**env_lst_to_array(t_name_value *env_node);
void			sort_env_array(char **env_arr);
int				print_export(t_env *env);

// export_utils2.c
int				update_env_array(t_env *env, char *key_value);
int				ft_export(t_glob_pipe *cmd, t_env *env);
int				find_key_index(char **env_arr, char *key);
int				size_environ(t_env	*env);

// export.c
void			export(t_glob_pipe *cmd, t_env *env);

// unset.c
int				unset(t_glob_pipe *cmd, t_env *env);
void			unset_check(t_glob_pipe *cmd, t_env *env);

// unset_utils.c
char			*key_duplicate(t_glob_pipe *cmd);
int				getenv_key(char *src, char **key);
char			*getenv_value(char *key, char **envp);

// echo.c
int				echo(t_glob_pipe *cmd, t_env *env);
void			echo_check(t_glob_pipe *cmd, t_env *env);

// env_tool.c
void			add_new_environ(t_env *env, char *name, char *value);
t_name_value	*new_node_value(void);
char			*ft_getenv(const char *name, t_env *env);
int 			update_environ(t_env *env, char *key_value);

#endif
