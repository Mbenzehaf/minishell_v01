# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/22 22:59:22 by mben-zeh          #+#    #+#              #
#    Updated: 2023/03/22 22:59:25 by mben-zeh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

CFLAGS= -Wall -Werror -Wextra 

SRC =   main.c 

OBJ = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJ}
	@$(CC)  $(OBJ) -o ${NAME} 

clean :
	rm -f ${OBJ} 

fclean : clean
	rm -f ${NAME}

re : fclean all

.PHONY: all clean fclean re