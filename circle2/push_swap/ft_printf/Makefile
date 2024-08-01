# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 17:45:15 by donghank          #+#    #+#              #
#    Updated: 2024/06/13 11:25:15 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRCP = ft_printf_address.c \
       ft_printf_nbr.c \
       ft_printf_strings.c

SRCS_PRINTF = ft_printf.c
SRCS = $(SRCP) $(SRCS_PRINTF)

OBJS = $(SRCS:.c=.o)

HEAD = .

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

all: $(NAME)

%.o: %.c
	@if ! $(CC) $(CFLAGS) -c -I $(HEAD) $< -o $@ 2> error.txt; then \
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
	@cp $(LIBFT_AR) .
	@if ! $(AR) $@ $^ libft.a >> error.txt 2>&1; then \
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
	@$(RM) $(NAME) libft.a
	@echo "$(YELLOW)Removed: $(NAME) libft.a$(RESET)"

re: fclean all

main: main.o $(NAME) $(LIBFT_AR)
	@if ! $(CC) $(CFLAGS) -o main main.o $(NAME) $(LIBFT_AR) >> error.txt 2>&1; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Error creating main executable$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: main executable$(RESET)"

main.o: main.c
	@if ! $(CC) $(CFLAGS) -c main.c -o main.o 2> error.txt; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Unable to create the object file for main.c$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi

mainfclean :
	rm -rf main.o
	rm -rf main

.PHONY: all clean fclean re
