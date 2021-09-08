# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/23 16:37:33 by tpouget           #+#    #+#              #
#    Updated: 2021/09/08 19:25:42 by tpouget          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

SOURCEFILES		=	$(wildcard src/*.c)

HEADERS			=	$(wildcard inc/*.h)

OBJECTFILES		=	$(patsubst %.c, obj/%.o, $(SOURCEFILES))
	
CFLAGS			=	-Wall -Wextra -g3 -pedantic -Iinc 

CC	  		  	=	clang

SANITIZER		=	-fsanitize=address

NAME			=	pipex

#	Rules

all:			$(NAME)

$(NAME):		${OBJECTFILES} #libft/libft.a
				${CC} ${SANITIZER} ${OBJECTFILES} -o $@ #-lft -Llibft

obj/%.o:		src/%.c	Makefile inc/${HEADERS} | obj
				${CC} ${CFLAGS} -c $< -o $@
obj:			
				mkdir obj

#libft/libft.a:
#                make -C libft

clean:			
				rm -rf obj

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

.PHONY:			all clean fclean re test 
