# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/23 16:37:33 by tpouget           #+#    #+#              #
#    Updated: 2021/09/13 20:40:06 by bajaba           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

NAME			=	pipex

CC	  		  	=	clang

CFLAGS			=	-Wall -Wextra -g3 -pedantic -Iinc -Isrc -Wno-unused-function

SANITIZER		=	-fsanitize=address

SOURCES			=	$(notdir $(wildcard src/*.c))

INC/HEADERS		=	$(wildcard inc/*.h)

OBJ/OBJECTS		=	$(filter-out obj/main.o obj/test.o, $(patsubst %.c, obj/%.o, $(SOURCES)))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS} obj/main.o libft/libft.a
				${CC} ${SANITIZER} obj/main.o ${OBJ/OBJECTS} -o $@ -lft -Llibft

test:			${OBJ/OBJECTS} obj/test.o libft/libft.a
				${CC} ${SANITIZER} obj/test.o ${OBJ/OBJECTS} -o $@ -lcriterion -lft -Llibft
				./test

obj/%.o:		src/%.c	${INC/HEADERS} Makefile | obj
				${CC} ${CFLAGS} -c $< -o $@
obj:			
				mkdir obj

libft/libft.a:
				make -C libft

clean:			
				rm -rf obj
				make -C libft clean

fclean:			clean
				rm -rf $(NAME)
				rm -rf build
				rm -rf test
				make -C libft fclean

re:				fclean all

.PHONY:			all clean fclean re test 
