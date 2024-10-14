SRC = main.c srcs/ft_whitespace.c srcs/builtins.c srcs/header.c \
	srcs/execute.c srcs/fill.c srcs/ft_exit.c srcs/ft_get.c srcs/parsing.c \
	srcs/free.c srcs/utils.c srcs/signal.c srcs/expander/quote.c srcs/expander/expander_utils.c srcs/expander/expander.c \
	srcs/env/env_utils.c srcs/env/env_utils2.c srcs/env/shlvl.c srcs/prepare_pipeline.c \
	srcs/builtin/export.c srcs/builtin/export_utils.c srcs/builtin/export_utils2.c srcs/builtin/unset.c \
	srcs/builtin/cd.c srcs/builtin/pwd.c srcs/builtin/unset_utils.c srcs/builtin/echo.c \
	srcs/run_global_pipeline.c srcs/tokenizer.c srcs/pre_parsing.c srcs/env/env_tool.c \
	srcs/access_checks.c srcs/parsing_fill_args.c srcs/operator_checks.c srcs/is_lex_error.c \
	srcs/run_processes.c srcs/calc_tokens.c srcs/create_new_token.c srcs/builtin/env.c \
	srcs/ft_atol.c srcs/heredoc.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

DEPS = $(SRC:.c=.d)

NAME = minishell

libft = libft/libft.a

CC = cc

all: $(NAME)

bonus: $(NAME)

$(libft):
	$(MAKE) -C libft bonus

$(NAME): $(libft) $(OBJ)
	$(CC) $(OBJ) -Iincludes -lreadline -L./libft -lft -o $(NAME)

%.o: %.c
	$(CC) -c -MMD -MP -Iincludes -I./libft $(CFLAGS) $< -o $@

-include $(DEPS)

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJ) $(DEPS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all
	$(MAKE) -C libft re

.PHONY: all bonus clean fclean re
