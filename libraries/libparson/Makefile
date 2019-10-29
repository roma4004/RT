# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/11 18:39:31 by ykopiika          #+#    #+#              #
#    Updated: 2019/10/29 16:07:07 by ykopiika         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libparson.a
HEADER = parson.h
SOURCE = parson.c\
OBJ = $(SOURCE:.c=.o)
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

%.o: %.c
	@gcc $(FLAGS) -c $< -o $@ -I $(HEADER)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
