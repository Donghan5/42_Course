SRC = main.c
SRC += calc_intersections/intersect_cylinder.c calc_intersections/intersect_plane.c\
	calc_intersections/intersect_sphere.c  calc_intersections/intersect_cone.c
SRC += calc_light/calc_diffuse_spec.c calc_light/check_shadow.c
SRC += info_block/info_block.c info_block/print_info_block.c
SRC += io/key_actions.c io/mouse_actions.c io/update_movement.c
SRC += parsing/init_shape.c parsing/init.c parsing/parse_tool.c parsing/parse_utils.c parsing/parsing.c \
	parsing/process_parse.c parsing/stock_ambient.c parsing/stock_basic.c parsing/stock_cam.c parsing/stock_cone.c \
	parsing/stock_cylinder.c parsing/stock_light.c parsing/stock_plane.c parsing/stock_sphere.c parsing/type.c \
	parsing/free.c parsing/check.c parsing/valid.c
SRC += render/fill_material.c render/frame_render.c render/get_checker_color.c render/render_scene.c render/trace_ray.c
SRC += tools/extra_operations.c tools/free_tools.c tools/ft_atod.c tools/vector_operations.c tools/tools.c

GNL = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

OBJDIR = .objs/
DEPDIR = .deps/
EXTRADIRS = $(DEPDIR)calc_intersections/ $(DEPDIR)calc_light/ $(DEPDIR)get_next_line/\
	$(DEPDIR)info_block/ $(DEPDIR)io/ $(DEPDIR)parsing/ $(DEPDIR)render/ $(DEPDIR)tools/\
	$(OBJDIR)calc_intersections/ $(OBJDIR)calc_light/ $(OBJDIR)get_next_line/\
	$(OBJDIR)info_block/ $(OBJDIR)io/ $(OBJDIR)parsing/ $(OBJDIR)render/ $(OBJDIR)tools/\

OBJ = $(addprefix $(OBJDIR), $(SRC:.c=.o))
OBJ += $(addprefix $(OBJDIR), $(GNL:.c=.o))

DEPS = $(addprefix $(DEPDIR), $(SRC:.c=.d))
DEPS += $(addprefix $(DEPDIR), $(GNL:.c=.d))

CFLAGS = -Wall -Wextra -Werror

NAME = miniRT

minilibx = minilibx-linux/libmlx.a

libft = libft/libft.a

CC = cc

all: $(NAME)

$(OBJDIR) $(DEPDIR) $(EXTRADIRS):
	mkdir -p $@

$(minilibx):
	$(MAKE) -C minilibx-linux

$(libft):
	$(MAKE) -C libft bonus

$(NAME): $(OBJDIR) $(DEPDIR) $(EXTRADIRS) $(minilibx) $(libft) $(OBJ)
	$(CC) $(OBJ) -I. -Lminilibx-linux -lmlx -L/usr/lib -lXext -lX11 -lm -lz -Llibft -lft -o $(NAME)

$(OBJDIR)%.o: %.c
	$(CC) -MMD -MP -I. $(CFLAGS) -MF $(DEPDIR)$*.d -c $< -o $@

-include $(DEPS)

clean:
	$(MAKE) -C minilibx-linux clean
	$(MAKE) -C libft clean
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all
	$(MAKE) -C libft re

.PHONY: all clean fclean re
