# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/31 17:09:07 by donghank          #+#    #+#              #
#    Updated: 2024/09/06 10:25:05 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

# linux valgrind and helgrind
CFLAGS = -Wall -Wextra -Werror -g3 -lpthread -Iincludes

# this for the mac m1
# CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread -lpthread -Iincludes

# this for the mac m1 (leak check)
# CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -lpthread -Iincludes

TARGET = philo

SRCS = srcs/init.c srcs/main.c srcs/philo.c srcs/utils.c srcs/philo_utils.c

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

all: $(TARGET)

clean:
	rm -rf $(TARGET)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
