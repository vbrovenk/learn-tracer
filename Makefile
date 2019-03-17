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

NAME = tracer
INCLUDE = raytray.h
LIBFT_INC = ./libft/includes/
LIBFT = libft/libft.a
MLX_FLAGS = -lmlx -framework OpenGL -framework AppKit
UBUNTU_FLAGS = -lm -lmlx -lXext -lX11 -L libmlx -I libmlx
SRC =	main.c \
		actions_with_points.c \
		actions_with_matrix.c
		
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror

OFF=\033[0m
RED=\033[31m
REDBOLD=\033[1;31m
GREEN=\033[32m
GREENBOLD=\033[1;32m
YELLOW=\033[33m
YELLOWBOLD=\033[1;33m
PURPLE=\033[35m
PURPLEBOLD=\033[1;35m

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	@gcc $(OBJ) $(LIBFT) $(UBUNTU_FLAGS) -o $(NAME)
	@echo "$(PURPLEBOLD)Tracer is ready"

%.o: %.c $(INCLUDE)
	@gcc -c $< -o $@ $(UBUNTU_FLAGS) -I $(LIBFT_INC)

clean:
	@make clean -C libft/
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C libft/
	@rm -rf $(NAME)

re: fclean all