# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/31 17:09:07 by donghank          #+#    #+#              #
#    Updated: 2024/09/01 16:05:27 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3 -lpthread -Iincludes

TARGET = philo

SRCS = srcs/init.c srcs/main.c srcs/philo.c srcs/utils.c

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

all: $(TARGET)

clean:
	rm -rf $(TARGET)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
