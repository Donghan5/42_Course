# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 17:45:15 by donghank          #+#    #+#              #
#    Updated: 2024/06/14 12:37:28 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRCP = ft_printf_address.c \
       ft_printf_nbr.c \
       ft_printf_strings.c

SRCS_PRINTF = ft_printf.c
SRCS = $(SRCP) $(SRCS_PRINTF)

OBJS = $(SRCS:.c=.o)

HEAD_DIR = .

LIBFT_DIR = ./libft/
LIBFT = libft.a
LIBFT_AR = $(addprefix $(LIBFT_DIR), $(LIBFT))

NAME        = libftprintf.a
AR          = ar rcs
RM          = rm -f
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

RESET       = \033[0m
BOLD        = \033[1m
RED         = \033[31m
GREEN       = \033[32m
YELLOW      = \033[33m
BLUE        = \033[34m

all: $(NAME) $(LIBFT_AR)

%.o: %.c
	@if ! $(CC) $(CFLAGS) -c -I $(HEAD_DIR) $< -o $@ 2> error.txt; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Unable to create the object file for $<$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi

$(LIBFT_AR):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_AR)
	@if ! cp $(LIBFT_AR) $(NAME) >> error.txt 2>&1; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Error copying $(LIBFT_AR) to $(NAME)$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi
	@if ! $(AR) $@ $^ >> error.txt 2>&1; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Error creating $(NAME)$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJS) $(LIBFT_AR)) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

clean:
	@make fclean -C $(LIBFT_DIR)
	@$(RM) $(OBJS) error.txt
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Removed: $(words $(OBJS)) object file(s)$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(YELLOW)Removed: $(NAME)$(RESET)"

re: fclean all

.PHONY: all clean fclean re
