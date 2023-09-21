# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: molla <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/25 10:19:56 by molla             #+#    #+#              #
#    Updated: 2023/04/24 17:38:02 by molla            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror #-pthread -fsanitize=thread -g3
SRCS		= philo.c
SRCSBONUS	= 
OBJS		= ${SRCS:.c=.o}
OBJSBONUS	= ${SRCSBONUS:.c=.o}
NAME		= philo

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} $^ -o $@

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

clean :
	rm -f ${OBJS} ${OBJSBONUS}

fclean : clean
	rm -f ${NAME}

re : fclean all

bonus : ${OBJSBONUS}
	${CC} ${CFLAGS} $^ -o ${NAME}

.PHONY : all clean fclean re