SRC = main.c srcs/ft_whitespace.c srcs/builtins.c srcs/header.c \
	srcs/execute.c srcs/fill.c srcs/ft_exit.c srcs/ft_get.c srcs/parsing.c \
	srcs/free.c srcs/utils.c srcs/signal.c srcs/expander/quote.c srcs/expander/expander_utils.c srcs/expander/expander.c \
	srcs/env/env_utils.c srcs/env/env_utils2.c srcs/env/shlvl.c srcs/prepare_pipeline.c \
	srcs/builtin/export.c srcs/builtin/export_utils.c srcs/builtin/export_utils2.c srcs/builtin/unset.c \
	srcs/builtin/cd.c srcs/builtin/pwd.c srcs/builtin/unset_utils.c srcs/builtin/echo.c \
	srcs/run_global_pipeline.c srcs/tokenizer.c srcs/pre_parsing.c srcs/env/env_tool.c \
	srcs/access_checks.c srcs/parsing_fill_args.c srcs/operator_checks.c srcs/is_lex_error.c \
	srcs/run_processes.c srcs/calc_tokens.c srcs/create_new_token.c srcs/builtin/env.c \
	srcs/ft_atoll.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

DEPS = $(SRC:.c=.d)

NAME = minishell

LIBFT = Libft/libft.a

CC = cc

all: $(NAME)

bonus: $(NAME)

$(LIBFT):
	$(MAKE) -C Libft bonus

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -Iincludes -lreadline -L./Libft -lft -o $(NAME)

%.o: %.c
	$(CC) -c -MMD -MP -Iincludes -I./Libft $(CFLAGS) $< -o $@

-include $(DEPS)

clean:
	$(MAKE) -C Libft clean
	rm -f $(OBJ) $(DEPS)

fclean: clean
	$(MAKE) -C Libft fclean
	rm -f $(NAME)

re: fclean all
	$(MAKE) -C Libft re

.PHONY: all bonus clean fclean re
