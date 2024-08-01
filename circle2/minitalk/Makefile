# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/26 14:56:10 by donghank          #+#    #+#              #
#    Updated: 2024/07/26 15:59:37 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LIBFT = ./libft/libft.a
RM = rm -rf

SERVER = server
CLIENT = client

SV_SRCS = srcs/server.c
SV_OBJS = $(SV_SRCS:.c=.o)

CLIENT_SRCS = srcs/client.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

SERVER_BONUS = server_bonus
CLIENT_BONUS = client_bonus

SV_BONUS_SRCS = srcs_bonus/server_bonus.c
SV_BONUS_OBJS = $(SV_BONUS_SRCS:.c=.o)

CLIENT_BONUS_SRCS = srcs_bonus/client_bonus.c
CLIENT_BONUS_OBJS = $(CLIENT_BONUS_SRCS:.c=.o)

all: $(SERVER) $(CLIENT)

$(LIBFT):
	$(MAKE) -C ./libft
$(SERVER): $(SV_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(SV_OBJS) $(LIBFT) -o server
$(CLIENT): $(CLIENT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) $(LIBFT) -o client

$(SERVER_BONUS): $(SV_BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(SV_BONUS_OBJS) $(LIBFT) -o server_bonus
$(CLIENT_BONUS): $(CLIENT_BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CLIENT_BONUS_OBJS) $(LIBFT) -o client_bonus

clean:
	$(MAKE) clean -C ./libft
	$(RM) $(SV_OBJS) $(CLIENT_OBJS)
	$(RM) $(SV_BONUS_OBJS) $(CLIENT_BONUS_OBJS)
fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(SERVER) $(CLIENT)
	$(RM) $(SERVER_BONUS) $(CLIENT_BONUS)
re: fclean all

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)

.PHONY: all clean fclean re bonus
