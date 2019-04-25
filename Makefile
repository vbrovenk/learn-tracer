# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/24 11:10:42 by vbrovenk          #+#    #+#              #
#    Updated: 2018/08/24 11:10:44 by vbrovenk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

INCLUDE = raytray.h
INC_DIR = .

LIBFT_INC = ./libft/includes/
LIBFT = ./libft/libft.a

MLX_FLAGS = -lmlx -framework OpenGL -framework AppKit
UBUNTU_FLAGS = -lm -lmlx -lXext -lX11 -L minilibx -I minilibx
SRC =	main.c \
		actions_with_points.c \
		actions_with_matrix.c \
		rotation.c \
		create_add.c \
		print.c \
		errors.c \
		scan_data.c \
		sphere.c \
		plane.c \
		cylinder.c \
		cone.c \

OBJ = $(addprefix obj/, $(SRC:.c=.o))

CFLAGS = -Wall -Wextra -Werror

OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITE=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

all: $(NAME)

$(NAME): libft/libft.a obj $(OBJ)
	@gcc $(OBJ) $(LIBFT) $(MLX_FLAGS) -o $(NAME)
	@echo "$(PURPLEBOLD)RTv1 $(PURPLE)is ready$(OFF)"

libft/libft.a:
	@make -C libft/

obj/%.o: src/%.c $(INCLUDE)
	@gcc -c $< -o $@  -I $(LIBFT_INC) -I $(INC_DIR)
	@echo "$(PURPLELIGHT)Compiling $(WHITE)$< $(PURPLELIGHT)done$(OFF)"

obj:
	@mkdir obj

clean:
	@make clean -C libft/
	@rm -rf obj

fclean: clean
	@make fclean -C libft/
	@rm -rf $(NAME)

re: fclean all
