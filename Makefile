# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dromanic <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/09 11:13:10 by akolinko          #+#    #+#              #
#    Updated: 2019/09/29 20:57:18 by dromanic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	=	rtv1

CC			= gcc
OBJ_PATH	= objectives
SRC_PATH	= sources
INC_PATH	= includes
LIBS_PATH	= libraries
LIBFT_PATH	= ${LIBS_PATH}/libft

SRC_N		=	double_with_vect3.c \
				draw.c \
				get_next_line.c \
				init.c \
				keys.c \
				ligths.c \
				main.c \
				parsing.c \
				ray_traces.c \
				rotate.c \
				uni_intersection.c \
				utils.c \
				validate_scene.c \
				vect3_utils.c \
				vect3_with_double.c \
				vect3_with_vect3.c

SRC = $(addprefix ${SRC_PATH}/, $(SRC_N))

OBJ = $(addprefix ./$(OBJ_PATH)/, $(SRC_N:.c=.o))

LIBS = ${LIBFT_PATH}/libft.a

INC		=	-I $(INC_PATH)/												\
			-I $(LIBS_PATH)/Frameworks/SDL2.framework/Versions/A/Headers/\
			-I $(LIBFT_PATH)/

LIBKEY	=	-F $(LIBS_PATH)/											\
			-rpath $(LIBS_PATH)/										\
			-framework SDL2

objdir:
	mkdir -p $(OBJ_PATH)

all: objdir
	@make -C $(LIBFT_PATH)/
	@make $(NAME)

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
	@/bin/rm -f $(OBJ)

fclean: clean
	@make fclean -C $(LIBFT_PATH)/
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
