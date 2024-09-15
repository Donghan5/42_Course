SRC = main.c srcs/ft_whitesplit.c srcs/ft_whitespace.c srcs/check.c srcs/header.c \
	srcs/excute.c srcs/fill.c srcs/ft_exit.c srcs/ft_get.c srcs/parsing.c \
	srcs/pipe.c srcs/utils.c srcs/signal.c srcs/expander/quote.c srcs/expander/expander.c \
	srcs/env/env_utils.c srcs/env/env_utils2.c srcs/env/shlvl.c srcs/env/init_env.c srcs/prepare_pipeline.c \
	srcs/builtin/export.c srcs/builtin/export_utils.c srcs/builtin/export_utils2.c srcs/builtin/unset.c
OBJ = $(SRC:.c=.o)
CFLAGS = -g3 #-Wall -Wextra -Werror
DEPS = $(SRC:.c=.d)
NAME = minishell
LIBFT = Libft/libft.a
CC = cc

all: $(NAME)

bonus: $(NAME)

$(LIBFT):
	$(MAKE) -C Libft bonus

$(NAME): $(LIBFT) $(OBJ)
	cc $(OBJ) -Iincludes -lreadline -L./Libft -lft -o $(NAME)

%.o: %.c
	cc -c -MMD -MP -Iincludes -I./Libft $(CFLAGS) $< -o $@

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
