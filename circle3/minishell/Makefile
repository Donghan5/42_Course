SRC = main.c srcs/ft_whitespace.c srcs/builtins.c srcs/header.c \
	srcs/execute.c srcs/fill.c srcs/ft_exit.c srcs/ft_get.c srcs/parsing.c \
	srcs/free.c srcs/utils.c srcs/signal.c srcs/expander/quote.c srcs/expander/expander_utils.c srcs/expander/expander.c \
	srcs/env/env_utils.c srcs/env/env_utils2.c srcs/env/shlvl.c srcs/prepare_pipeline.c \
	srcs/builtin/export.c srcs/builtin/export_utils.c srcs/builtin/export_utils2.c srcs/builtin/unset.c \
	srcs/builtin/cd.c srcs/builtin/pwd.c srcs/builtin/unset_utils.c srcs/builtin/echo.c \
	srcs/run_global_pipeline.c srcs/tokenizer.c srcs/pre_parsing.c srcs/env/env_tool.c \
	srcs/access_checks.c srcs/parsing_fill_args.c srcs/operator_checks.c srcs/is_lex_error.c \
	srcs/run_processes.c srcs/calc_tokens.c srcs/create_new_token.c srcs/builtin/env.c \
	srcs/ft_atol.c srcs/heredoc.c srcs/expander_heredoc.c srcs/exit_utils.c

SRC_BONUS = bonus/main_bonus.c bonus/srcs_bonus/ft_whitespace_bonus.c bonus/srcs_bonus/builtins_bonus.c \
	bonus/srcs_bonus/header_bonus.c bonus/srcs_bonus/execute_bonus.c bonus/srcs_bonus/fill_bonus.c \
	bonus/srcs_bonus/ft_exit_bonus.c bonus/srcs_bonus/ft_get_bonus.c bonus/srcs_bonus/parsing_bonus.c \
	bonus/srcs_bonus/free_bonus.c bonus/srcs_bonus/utils_bonus.c \
	bonus/srcs_bonus/signal_bonus.c bonus/srcs_bonus/expander_bonus/quote_bonus.c \
	bonus/srcs_bonus/expander_bonus/expander_utils_bonus.c bonus/srcs_bonus/expander_bonus/expander_bonus.c \
	bonus/srcs_bonus/env_bonus/env_utils_bonus.c bonus/srcs_bonus/env_bonus/env_utils2_bonus.c \
	bonus/srcs_bonus/env_bonus/shlvl_bonus.c bonus/srcs_bonus/prepare_pipeline_bonus.c \
	bonus/srcs_bonus/builtin_bonus/export_bonus.c bonus/srcs_bonus/builtin_bonus/export_utils_bonus.c \
	bonus/srcs_bonus/builtin_bonus/export_utils2_bonus.c bonus/srcs_bonus/builtin_bonus/unset_bonus.c \
	bonus/srcs_bonus/builtin_bonus/cd_bonus.c bonus/srcs_bonus/builtin_bonus/pwd_bonus.c \
	bonus/srcs_bonus/builtin_bonus/unset_utils_bonus.c bonus/srcs_bonus/builtin_bonus/echo_bonus.c \
	bonus/srcs_bonus/run_global_pipeline_bonus.c bonus/srcs_bonus/tokenizer_bonus.c bonus/srcs_bonus/pre_parsing_bonus.c \
	bonus/srcs_bonus/env_bonus/env_tool_bonus.c bonus/srcs_bonus/access_checks_bonus.c \
	bonus/srcs_bonus/parsing_fill_args_bonus.c bonus/srcs_bonus/operator_checks_bonus.c \
	bonus/srcs_bonus/is_lex_error_bonus.c bonus/srcs_bonus/run_processes_bonus.c \
	bonus/srcs_bonus/calc_tokens_bonus.c bonus/srcs_bonus/create_new_token_bonus.c bonus/srcs_bonus/builtin_bonus/env_bonus.c \
	bonus/srcs_bonus/ft_atol_bonus.c bonus/srcs_bonus/heredoc_bonus.c bonus/srcs_bonus/run_managers_bonus.c \
	bonus/srcs_bonus/expander_heredoc_bonus.c bonus/srcs_bonus/exit_utils_bonus.c bonus/srcs_bonus/ft_realloc_bonus.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

CFLAGS = -g3 #-Wall -Wextra -Werror

DEPS = $(SRC:.c=.d)
DEPS_BONUS = $(SRC_BONUS:.c=.d)

NAME = minishell
BONUS = minishell_bonus

libft = libft/libft.a

CC = cc

all: $(NAME)

bonus: $(BONUS)

$(libft):
	$(MAKE) -C libft bonus

$(NAME): $(libft) $(OBJ)
	$(CC) $(OBJ) -Iincludes -lreadline -L./libft -lft -o $(NAME)

$(BONUS): $(libft) $(OBJ_BONUS)
	$(CC) $(OBJ_BONUS) -Ibonus/includes_bonus -lreadline -L./libft -lft -o $(BONUS)

%.o: %.c
	$(CC) -c -MMD -MP -Iincludes -Ibonus/includes_bonus -I./libft $(CFLAGS) $< -o $@

-include $(DEPS) $(DEPS_BONUS)

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJ) $(DEPS) $(OBJ_BONUS) $(DEPS_BONUS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME) $(BONUS)

re: fclean all
	$(MAKE) -C libft re

.PHONY: all bonus clean fclean re
