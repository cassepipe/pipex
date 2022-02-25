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

CC	  		  	=	gcc

#Add -Werror before correction
CFLAGS			=	-Wall -Wextra  -Iinc -g3

LDFLAGS			=	#-static-libasan

SANITIZER		=	#-fsanitize=address

ANALYSER		=	#-fanalyzer

SOURCES			=	free_null_terminated_array_of_arrays.c \
					ft_split.c \
					get_command_path.c \
					getvar.c \
					main.c \
					print_usage_exit.c \
					redirections.c \
					retrieve.c \
					strjoin3.c \
					ft_perror.c \
					utils2.c \
					utils.c 

INC/HEADERS		=	inc/pipex.h

OBJ/OBJECTS		=	$(patsubst %.c, obj/%.o, $(SOURCES))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS}
				@echo "Linking..."
				${CC} ${SANITIZER} ${ANALYSER} ${LDFLAGS} ${OBJ/OBJECTS} -o $@ 

obj/%.o:		src/%.c	${INC/HEADERS} Makefile | obj
				${CC} ${CFLAGS} ${ANALYSER} -c $< -o $@
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
