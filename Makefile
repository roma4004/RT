# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/09 11:13:10 by akolinko          #+#    #+#              #
#    Updated: 2019/10/22 17:26:24 by vtlostiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	=	RT

CC			= gcc -Wall -Wextra -Werror
OBJ_PATH	= objectives
SRC_PATH	= sources
INC_PATH	= includes
LIBS_PATH	= libraries
LIBFT_PATH	= ${LIBS_PATH}/libft

SRC_N		=	color.c \
				double_with_vect3.c \
				draw.c \
				effects.c \
				get_next_line.c \
				init.c \
				key_down_cam_move.c \
				key_down_cam_rotate.c \
				key_mouse.c \
				key_move.c \
				key_reset.c \
				key_up_cam_move.c \
				key_up_cam_rotate.c \
				key_utils.c \
				keys.c \
				keys_mouse.c \
				ligths.c \
				main.c \
				obj_intersection_base.c \
				obj_normal_base.c \
				obj_utils.c \
				parsing.c \
				parsing_caps.c \
				parsing_utils.c \
				parsing_utils_backup.c \
				parsing_utils_setters.c \
				parsing_validate_scene.c \
				ray_reflect_n_refract.c \
				ray_traces.c \
				vec3_rotate.c \
				vec3_utils.c \
				vec3_with_double.c \
				vec3_with_vec3.c

SRC = $(addprefix ${SRC_PATH}/, $(SRC_N))

OBJ = $(addprefix ./$(OBJ_PATH)/, $(SRC_N:.c=.o))

LIBS = ${LIBFT_PATH}/libft.a

INC		=	-I $(INC_PATH)/												\
			-I $(LIBS_PATH)/Frameworks/SDL2.framework/Versions/A/Headers/\
			-I $(LIBFT_PATH)/

LIBKEY	=	-F $(LIBS_PATH)/											\
			-rpath $(LIBS_PATH)/										\
			-framework SDL2

all: objdir $(NAME)
	@make -C $(LIBFT_PATH)/

objdir:
	@mkdir -p $(OBJ_PATH)

$(NAME): LIBS $(OBJ)
	$(CC) $(LIBKEY) $(OBJ) $(LIBS) -o $(NAME)

./$(OBJ_PATH)/%.o : ./$(SRC_PATH)/%.c
	$(CC) $(INC) -c $< -o $@

LIBS:
	@make -C $(LIBFT_PATH)/

norm:
	@norminette ./$(LIBFT_PATH)/*.c
	@norminette ./$(LIBFT_PATH)/*.h
	@norminette ./$(SRC_PATH)/*.c
	@norminette ./$(INC_PATH)/*.h

clean:
	@make clean -C $(LIBFT_PATH)/
	@/bin/rm -rf $(OBJ_PATH)/

fclean: clean
	@make fclean -C $(LIBFT_PATH)/
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
