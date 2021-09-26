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

CFLAGS			=	-Wall -Wextra -g3 -pedantic -Iinc 

SANITIZER		=	#-fsanitize=address

SOURCES			=	$(notdir $(wildcard src/*.c))

INC/HEADERS		=	$(wildcard inc/*.h)

OBJ/OBJECTS		=	$(patsubst %.c, obj/%.o, $(SOURCES))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS}
				@echo "Linking..."
				${CC} ${SANITIZER} ${OBJ/OBJECTS} -o $@ 

obj/%.o:		src/%.c	${INC/HEADERS} Makefile | obj
				${CC} ${CFLAGS} -c $< -o $@
obj:			
				mkdir obj

clean:			
				rm -rf obj

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

love:			
				../pipex-tester/run.sh

.PHONY:			all clean fclean re test 
