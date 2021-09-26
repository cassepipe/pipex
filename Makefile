# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/23 16:37:33 by tpouget           #+#    #+#              #
#    Updated: 2021/09/15 14:36:55 by bajaba           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

NAME			=	pipex

CC	  		  	=	clang

CFLAGS			=	-Wall -Wextra -g3 -pedantic -Iinc -Isrc -Ilibft -Wno-unused-function

SANITIZER		=	#-fsanitize=address

SOURCES			=	$(notdir $(wildcard src/*.c))

INC/HEADERS		=	$(wildcard inc/*.h)

OBJ/OBJECTS		=	$(filter-out obj/main.o obj/test.o, $(patsubst %.c, obj/%.o, $(SOURCES)))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS} obj/main.o libft/libft.a
				@echo "Linking..."
				${CC} ${SANITIZER} obj/main.o ${OBJ/OBJECTS} -o $@ -lft -Llibft

test:			${OBJ/OBJECTS} obj/test.o libft/libft.a
				${CC} ${SANITIZER} obj/test.o ${OBJ/OBJECTS} -o $@ -lcriterion -lft -Llibft

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

love:			
				make re SANITIZER=""
				../pipex-tester/run.sh

.PHONY:			all clean fclean re test 
