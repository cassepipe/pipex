# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/23 16:37:33 by tpouget           #+#    #+#              #
#    Updated: 2021/09/09 01:00:45 by tpouget          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

NAME			=	pipex

CC	  		  	=	clang

CFLAGS			=	-Wall -Wextra -g3 -pedantic -Iinc -Isrc

SANITIZER		=	-fsanitize=address

SOURCES			=	$(notdir $(wildcard src/*.c))

INC/HEADERS		=	$(wildcard inc/*.h)

OBJ/OBJECTS		=	$(patsubst %.c, obj/%.o, $(SOURCES))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS} #libft/libft.a
				${CC} ${SANITIZER} ${OBJ/OBJECTS} -o $@ #-lft -Llibft

obj/%.o:		src/%.c	${INC/HEADERS} Makefile | obj
				${CC} ${CFLAGS} -c $< -o $@
obj:			
				mkdir obj

#libft/libft.a:
#                make -C libft

clean:			
				rm -rf obj

fclean:			clean
				rm -rf $(NAME)
				rm -rf build

re:				fclean all

.PHONY:			all clean fclean re test 
